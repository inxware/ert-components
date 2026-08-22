#!/usr/bin/env python3
"""publish_set.py — the set of ert-components paths published to the public mirror.

SCOPE — the ert-components half of the publish only. ert-contrib-middleware,
ert-build-support, ert-kernels and apps are handled by publish_release.sh.

Input : publish_set.txt, an allow-list of platform targets, and the
        platform configs it reaches.
Output: only --sync writes; every other mode reports.

  --check   validate the allow-list and closure. Exit 1 on failure. Run by
            ci_run_preflight on every regression.
  --plan    print the derived path set.
  --diff    rsync dry-run against the mirror, per path.
  --deps    list the ert-build-support / ert-contrib-middleware directories the
            published set requires.
  --merge   open meld on each hand-merge path that differs, one at a time.
  --sync    WRITES. rsync --delete into the mirror, so paths outside the closure
            are removed as well as added. Requires --check to pass and --yes,
            With --tag, writes a git tag both sides. Never commits or pushes.

The published set is *derived*, not enumerated: listing a platform pulls in
everything it needs, so the list cannot drift out of step with the build.

Only the platforms you actually want published are listed; the base platforms
they include are added automatically.

Derivation, per listed platform:
  1. the target itself, plus every platform it reaches through
     `include ./target/platform/<base>/config.mk` transitively — without these
     `./configure <target>` fails on the mirror
  2. target/os-arch/<EHS_OS>-<EHS_ARCH>/, plus any os-arch directory that one
     includes (the *_ALL trees)
  3. every target/devman-configs/*.mk it includes
  4. a fixed structural set (Common/, HAL, build scripts, docs, ...)

Paths in publish_set_merge.list are never overwritten by --sync: the internal
and public copies of those files are maintained separately and are expected to
differ permanently. --sync skips and lists them; --merge walks them in meld.
"""

import argparse
import os
import re
import subprocess
import sys

# Minimal palette. Colour marks severity so the lines needing a human stand out;
# disabled when stdout is not a tty or NO_COLOR is set, so redirected output and
# CI logs stay plain.
_C = sys.stdout.isatty() and not os.environ.get("NO_COLOR")
RED = "\033[91m" if _C else ""
YEL = "\033[93m" if _C else ""
GRN = "\033[92m" if _C else ""
OFF = "\033[0m" if _C else ""

# Customer and product names that must never reach the public mirror. Matched
# against every platform directory and devman-config in the derived closure.
#
# A safety net, not the selector: the allow-list (publish_set.txt) decides what
# is copied. Removing a name here publishes nothing by itself, it only stops that
# name being blocked outright.
#
# devman-config files carry a server domain, name, protocol, username, ssh port
# and product — no credentials — so a devman name only belongs here when the
# customer relationship itself is confidential.
PRIVATE = re.compile(
    r"hrdc|ambifier|adnoc|quensus|caravan|willerby|heatrod|moodsonic"
    r"|sysboiler|revolver|supervisor|nibe|android6|q_box"
    r"|devman-tsa|inx_hri|inx_hrc|patrick|sandbox",
    re.I,
)

# Structural paths published regardless of target selection.
FIXED = [
    "configure", "Makefile", "deps.mk", "makedep.sh",
    "README.md", "LICENSE.md", "CONTRIBUTING.md", "DEVELOPING.md", "BUILDING.md",
    "licenses/",
    "Common/Components/", "Common/Ehs/", "Common/HAL/", "Common/KAPI/",
    "Common/SandboxComp/",
    "target/platform/platform.mk", "target/platform/sbom.mk",
    "target/Component-HAL/",
    "target/envbuildscripts/", "target/envtree/",
    "scripts/",
    "docs/",
    "SystemTests/",
]

MERGE_LIST = "scripts/git-utilities/publish_set_merge.list"

INC_PLATFORM = re.compile(r"include\s+\./target/platform/([^/]+)/config\.mk")
INC_DEVMAN = re.compile(r"include\s+\./target/devman-configs/([^\s]+\.mk)")
INC_OSARCH = re.compile(r"os-arch/([A-Za-z0-9_.-]+)/")
# deps.mk reaches sibling os-arch trees by relative path, e.g.
#   target_types.h : ../base_full/base_types.h
# These must be followed or shared trees like base_full, which every
# gnu/android/qnx/macos target needs, are left out of the mirror.
INC_OSARCH_REL = re.compile(r"\.\./([A-Za-z0-9_.-]+)/")
VAR = re.compile(r"^\s*([A-Z_0-9]+)\s*[:?]?=\s*(.*?)\s*$")


def read_vars(path):
    """Variable assignments from a config.mk, make comment semantics applied."""
    out = {}
    if not os.path.exists(path):
        return out
    for line in open(path, errors="ignore"):
        line = line.split("#")[0].rstrip()
        m = VAR.match(line)
        if m:
            out[m.group(1)] = m.group(2)
    return out


def platform_closure(root, listed):
    """The listed platforms plus every platform they include, transitively."""
    seen, queue = set(), list(listed)
    while queue:
        p = queue.pop()
        if p in seen:
            continue
        seen.add(p)
        cfg = os.path.join(root, "target/platform", p, "config.mk")
        if os.path.exists(cfg):
            queue += INC_PLATFORM.findall(open(cfg, errors="ignore").read())
    return seen


def osarch_closure(root, platforms):
    """os-arch directories reached by the platforms, plus their *_ALL includes."""
    found, queue = set(), []
    for p in platforms:
        env = {}
        for q in platform_closure(root, [p]):          # inherited EHS_OS/EHS_ARCH
            env.update(read_vars(os.path.join(root, "target/platform", q, "config.mk")))
        if "EHS_OS" in env and "EHS_ARCH" in env:
            queue.append(f"{env['EHS_OS']}-{env['EHS_ARCH']}")
    while queue:
        d = queue.pop()
        if d in found:
            continue
        found.add(d)
        base = os.path.join(root, "target/os-arch", d)
        if not os.path.isdir(base):
            continue
        for mk in ("toolchain.mk", "target.mk", "config.mk", "deps.mk"):
            f = os.path.join(base, mk)
            if not os.path.exists(f):
                continue
            text = open(f, errors="ignore").read()
            queue += INC_OSARCH.findall(text)
            queue += INC_OSARCH_REL.findall(text)
    return {d for d in found if os.path.isdir(os.path.join(root, "target/os-arch", d))}


def devman_closure(root, platforms):
    out = set()
    for p in platforms:
        cfg = os.path.join(root, "target/platform", p, "config.mk")
        if os.path.exists(cfg):
            out |= set(INC_DEVMAN.findall(open(cfg, errors="ignore").read()))
    return out


def derive(root, list_file):
    listed = [
        ln.strip() for ln in open(list_file)
        if ln.strip() and not ln.lstrip().startswith("#")
    ]
    platforms = platform_closure(root, listed)
    return listed, platforms, osarch_closure(root, platforms), devman_closure(root, platforms)


def check(root, listed, platforms, osarchs, devmans):
    errs, warns = [], []
    for lf in listed:
        if not os.path.isdir(os.path.join(root, "target/platform", lf)):
            errs.append(f"allow-list names a platform that does not exist: {lf}")
    for p in sorted(platforms):
        if PRIVATE.search(p):
            errs.append(f"private name reached through the include closure: target/platform/{p}")
    for d in sorted(devmans):
        if PRIVATE.search(d):
            errs.append(f"private devman config reached through the closure: target/devman-configs/{d}")
        elif not os.path.exists(os.path.join(root, "target/devman-configs", d)):
            warns.append(f"devman config referenced but missing: {d}")
    for d in sorted(osarchs):
        if PRIVATE.search(d):
            errs.append(f"private os-arch reached through the closure: target/os-arch/{d}")
    return errs, warns


def read_merge_list(root):
    """Paths that must be hand-merged rather than overwritten."""
    f = os.path.join(root, MERGE_LIST)
    if not os.path.exists(f):
        return []
    return [ln.strip() for ln in open(f)
            if ln.strip() and not ln.lstrip().startswith("#")]


def is_merge_path(rel, merge):
    """True if rel is, or sits under, a hand-merge entry."""
    r = rel.rstrip("/")
    return any(r == m.rstrip("/") or r.startswith(m.rstrip("/") + "/") for m in merge)


def differs(src, dst):
    if not os.path.exists(dst):
        return True
    cmd = ["diff", "-rq", src, dst] if os.path.isdir(src) else ["diff", "-q", src, dst]
    return subprocess.run(cmd, capture_output=True).returncode != 0


def derive_keys(env):
    """The keys platform.mk composes from one platform's resolved config.

    The single source of truth for the composition rule. check_key_agreement.py
    imports this rather than deriving its own, because two independent copies of
    this rule drifting apart is exactly the bug class being chased: EHS-kernel
    and ert-contrib-middleware each compose the key their own way, and three
    faults from that surfaced in one day (see check_key_agreement.py's header).
    Adding a third copy here would be repeating it.

    EHS_GNU_OS_VERSION is the pre-rename spelling of EHS_TARGET_LIB_VARIANT and
    is still live in EHS-kernel, so both are accepted.

    Returns None when the config does not resolve an OS and arch — an abstract
    base fragment rather than a real target.
    """
    if "EHS_OS" not in env or "EHS_ARCH" not in env:
        return None
    ga = env.get("EHS_GNU_ARCH") or env["EHS_ARCH"]
    go = env.get("EHS_GNU_OS") or env["EHS_OS"]
    variant = env.get("EHS_TARGET_LIB_VARIANT", env.get("EHS_GNU_OS_VERSION", ""))
    goa = f"{ga}-{go}{variant}"
    tn = env.get("TOOLCHAIN_NAME")
    cv = env.get("COMPONENT_VARIANT")

    # Mirrors platform.mk, including its asymmetry: the COMPONENT_VARIANT branch
    # drops a HOST toolchain from the key, the no-variant branch keeps it. So a
    # HOST target with no variant asks for "<goa>-HOST", which never exists.
    # Unifying the two branches here would stop matching the build.
    if cv:
        contrib = f"{goa}_{cv}" + (f"-{tn}" if tn and tn != "HOST" else "")
    elif tn:
        contrib = f"{goa}-{tn}"
    else:
        contrib = goa

    tp = env.get("TOOLCHAIN_PATH") or (
        None if tn == "HOST" else (f"x86_64/{tn}" if tn else f"x86_64/{goa}"))

    return {"support": goa, "kernel": goa, "contrib": contrib, "toolchain": tp}


def resolve_env(root, platform):
    """One platform's config, flattened through its whole include closure."""
    env = {}
    for q in sorted(platform_closure(root, [platform])):
        env.update(read_vars(os.path.join(root, "target/platform", q, "config.mk")))
    return env


# Never published, even inside a directory that is. rsync --exclude patterns,
# applied to every path in the plan.
#
# docs/ ships whole, so an internal note placed under it would be published
# without anything else objecting. Working notes carry customer names.
#
# PATTERN RULES — get these wrong and the exclusion silently does nothing.
# Each plan entry is its own rsync transfer, so patterns are matched against the
# path relative to THAT entry, not to the repo root:
#   * no slash          -> matches the final component at any depth ("*.orig")
#   * slash, unanchored -> matches the END of the relative path ("CI/projects/")
#   * leading slash     -> anchored to the transfer root ("/reports/")
# So a repo-root-style path like "SystemTests/CI/projects/" never matches: when
# SystemTests/ is the transfer root the relative path is only "CI/projects".
# That exact mistake published the customer CI projects to the mirror.
NEVER_PUBLISH = [
    "_working_notes/",
    "__pycache__/",
    ".pytest_cache/",
    # Internal audit and operations notes. They live under docs/ for the people
    # who need them, but name customers and describe the publishing process
    # itself, so they must not travel with docs/.
    "target-libs-naming-audit.md",
    "community-github-mirror.md",
    # NXP RedLib os-arch tree. Carries customer-named linker scripts, and no
    # published platform needs it — nxp_arm was taken off publish_set.txt with
    # this. Both must change together: a published platform whose os-arch tree
    # is excluded cannot configure on the mirror.
    "nxp-redlib-freertos-arm/",
    # Customer CI projects: Jenkins jobs, device deployment and jlink flash
    # scripts, named per customer. SystemTests/ is published as a whole tree, so
    # without these they would travel with it. Relative to that transfer root the
    # path is "CI/projects", which is what the pattern must say.
    "CI/projects/",
    # Test-run reports. Timestamped history of internal regression and app
    # sweeps; the external-apps ones list every customer app by name. Kept in the
    # internal repo for history, never published. Anchored so it cannot match
    # target/envtree/.../build/reports in another transfer.
    "/reports/",
    # Internal regression lists. Both name customer targets, and the mirror does
    # not need them: community.txt and smoke.txt are the public-facing lists and
    # no workflow references these.
    "published.txt",
    "known-broken-keys.txt",
]


def deps(root, platforms):
    """Dependency directories the published platforms require, per repo.

    Mirrors the composition in target/platform/platform.mk:
      kernel / libc  ert-build-support/support_libs/target_libs/<EHS_GNU_OS_ARCH>
      toolchain      ert-build-support/toolchains/<TOOLCHAIN_PATH>
      contrib        ert-contrib-middleware/target_libs/<COMPONENT_BASE_TECHNOLOGIES>

    Only the listed platforms are asked for, but each is resolved through its
    include closure
    so inherited EHS_OS / EHS_ARCH / TOOLCHAIN_NAME are picked up.
    """
    support, toolchains, contrib = set(), set(), set()
    for p in platforms:
        k = derive_keys(resolve_env(root, p))
        if not k:
            continue
        support.add(k["support"])
        contrib.add(k["contrib"])
        if k["toolchain"]:
            toolchains.add(k["toolchain"])
    return support, toolchains, contrib


def paths(platforms, osarchs, devmans):
    out = list(FIXED)
    out += [f"target/platform/{p}/" for p in sorted(platforms)]
    out += [f"target/os-arch/{d}/" for d in sorted(osarchs)]
    out += [f"target/devman-configs/{d}" for d in sorted(devmans)]
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--source", default=".")
    ap.add_argument("--dest", default="../../inxware-github/ert-components")
    ap.add_argument("--list", dest="list_file",
                    default="scripts/git-utilities/publish_set.txt")
    g = ap.add_mutually_exclusive_group()
    for m in ("check", "plan", "diff", "merge", "sync", "deps"):
        g.add_argument(f"--{m}", action="store_true")
    ap.add_argument("--yes", action="store_true", help="required with --sync")
    ap.add_argument("--tag", help="with --sync: tag both repos with this name")
    ap.add_argument("--porcelain", action="store_true",
                    help="with --deps: tab-separated repo/subdir/name, for scripts")
    a = ap.parse_args()

    root = os.path.abspath(a.source)
    lst = os.path.join(root, a.list_file)
    if not os.path.exists(lst):
        sys.exit(f"allow-list not found: {lst}")

    listed, platforms, osarchs, devmans = derive(root, lst)
    errs, warns = check(root, listed, platforms, osarchs, devmans)

    quiet = a.porcelain and a.deps
    if quiet:
        if errs:
            for e in errs:
                print(f"ERROR\t{e}", file=sys.stderr)
            return 1
        sup, tc, con = deps(root, platforms)
        for n in sorted(sup):
            print(f"ert-build-support\tsupport_libs/target_libs\t{n}")
        for n in sorted(tc):
            print(f"ert-build-support\ttoolchains\t{n}")
        for n in sorted(con):
            print(f"ert-contrib-middleware\ttarget_libs\t{n}")
        return 0

    print(f"\nAllow-list      : {a.list_file}")
    print(f"  platforms listed  : {len(listed)}")
    print(f"  after includes    : {len(platforms)} platforms "
          f"(+{len(platforms) - len(listed)} base platforms pulled in)")
    print(f"  os-arch dirs      : {len(osarchs)}")
    print(f"  devman configs    : {len(devmans)}")
    for w in warns:
        print(f"  WARN  {w}")
    for e in errs:
        print(f"  {RED}ERROR {e}{OFF}")
    if errs:
        print("\nRefusing to continue.\n")
        return 1
    print(f"  {GRN}checks passed{OFF}")
    merge = read_merge_list(root)
    print(f"  hand-merge paths: {len(merge)} (never overwritten — see {MERGE_LIST})")

    if a.deps:
        sup, tc, con = deps(root, platforms)
        if a.porcelain:
            # <repo>\t<subdir>\t<name> — consumed by publish_release.sh step 4.
            for n in sorted(sup):
                print(f"ert-build-support\tsupport_libs/target_libs\t{n}")
            for n in sorted(tc):
                print(f"ert-build-support\ttoolchains\t{n}")
            for n in sorted(con):
                print(f"ert-contrib-middleware\ttarget_libs\t{n}")
            return 0
        BS = os.path.abspath(os.path.join(root, "../ert-build-support"))
        CM = os.path.abspath(os.path.join(root, "../ert-contrib-middleware"))
        print()
        for label, items, base, sub in (
                ("ert-build-support  support_libs/target_libs", sup, BS, "support_libs/target_libs"),
                ("ert-build-support  toolchains",               tc,  BS, "toolchains"),
                ("ert-contrib-middleware  target_libs",         con, CM, "target_libs")):
            print(f"  {label}  ({len(items)})")
            for d in sorted(items):
                mark = "" if os.path.isdir(os.path.join(base, sub, d)) else "   MISSING"
                print(f"      {sub}/{d}{mark}")
            print()
        return 0

    if a.check or not (a.plan or a.diff or a.merge or a.sync):
        print()
        return 0

    plan = paths(platforms, osarchs, devmans)
    if a.plan:
        print()
        for p in plan:
            print(f"  {p}")
        print(f"\n  {len(plan)} paths\n")
        return 0

    dest = os.path.abspath(os.path.join(root, a.dest))
    if not os.path.isdir(dest):
        sys.exit(f"mirror not found: {dest}")
    # --merge: walk the hand-merge paths in meld, one at a time.
    if a.merge:
        import shutil
        if not shutil.which("meld"):
            sys.exit("meld is not installed or not on PATH")
        opened = same = 0
        for rel in merge:
            src, dst = os.path.join(root, rel), os.path.join(dest, rel)
            if not os.path.exists(src.rstrip("/")):
                print(f"  SKIP (absent in source) {rel}")
                continue
            if not os.path.exists(dst.rstrip("/")):
                print(f"  NEW in source, nothing to merge against: {rel}")
                continue
            if differs(src, dst):
                opened += 1
                print(f"  [{opened}] meld {rel}")
                subprocess.run(["meld", src, dst])   # blocks until closed
            else:
                same += 1
        print(f"\n  {opened} reviewed, {same} already identical\n")
        return 0

    dry = [] if a.sync else ["-n"]
    if a.sync and not a.yes:
        sys.exit("--sync requires --yes. Run --diff first and read it.")

    # Refuse to sync while a hand-merge path still differs. --sync skips those
    # paths, and a note at the end of 100+ lines of rsync output is easy to miss:
    # the mirror would silently keep its old copy of a file we know has changed.
    changed = 0
    review = []
    for rel in plan:
        src = os.path.join(root, rel)
        if not os.path.exists(src.rstrip("/")):
            print(f"  SKIP (absent in source) {rel}")
            continue
        if is_merge_path(rel, merge):
            if differs(src, os.path.join(dest, rel)):
                review.append(rel)
            continue
        r = subprocess.run(
            ["rsync", "-a", "--mkpath", "--itemize-changes", "--delete",
             *[f"--exclude={x}" for x in NEVER_PUBLISH], *dry,
             src, os.path.join(dest, rel)],
            capture_output=True, text=True)
        lines = [l for l in r.stdout.splitlines()
                 if l and not l.startswith((".", "sent ", "total ", "sending "))]
        if lines:
            changed += 1
            print(f"  {'CHANGED' if a.sync else 'WOULD CHANGE'} {rel}")
            for l in lines[:10]:
                print(f"      {l}")
            if len(lines) > 10:
                print(f"      ... {len(lines) - 10} more")
    # Provenance is a TAG in both repos, not a file. A dotfile in the mirror is
    # invisible in `git log`, invisible in the GitHub UI, and will be forgotten;
    # `merge-to-github-<n>` shows up in --decorate on both sides and is what an
    # operator will actually reach for.
    if a.sync and a.tag:
        for repo, label in ((root, "source"), (dest, "mirror")):
            r = subprocess.run(["git", "-C", repo, "tag", "-f", a.tag],
                               capture_output=True, text=True)
            print(f"  tagged {label}: {a.tag}"
                  if r.returncode == 0 else f"  TAG FAILED ({label}): {r.stderr.strip()}")

    print(f"\n  {changed} path(s) {'updated' if a.sync else 'would change'}")
    if review:
        # Expected, not a warning: these paths are on the hand-merge list
        # because the internal and public copies are maintained separately.
        print(f"\n  {len(review)} hand-merge path(s) differ, left untouched "
              f"(expected — maintained separately):")
        for r in review:
            print(f"      {r}")
        print("      Review them with:  publish_set.py --merge")
    print()
    return 0


if __name__ == "__main__":
    sys.exit(main())
