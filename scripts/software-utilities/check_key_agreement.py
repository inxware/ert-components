#!/usr/bin/env python3
"""check_key_agreement.py — do the kernel producer and consumer agree on keys?

ert-components and EHS-kernel each compose EHS_GNU_OS_ARCH from their own
config, and nothing checks the two agree. When they drift, a target builds
cleanly against a kernel archive nobody rebuilds.

check_target_keys.sh asks "does every consumed key exist?". This asks the other
half:

    consumed - produced   ert-components needs a kernel nobody builds
    produced - consumed   a build writes where nothing reads

A key appearing in both lists means a misspelling, not something missing.

Also compares ert-components against ert-contrib-middleware, where the producer
has no target list to read and the built target_libs/ directories are the only
statement of what exists.

Key derivation is imported from publish_set.py because it follows the
`include ./target/platform/<base>/config.mk` chain. fb_platform_matrix.py has a
config parser too but deliberately does not follow includes, so any target
inheriting EHS_OS / EHS_ARCH from a parent would resolve wrongly.

Usage:
    check_key_agreement.py [--kernel-repo PATH] [--quiet]

Exit 0 when the two sides agree, 1 when they do not, 0 with a notice when
EHS-kernel is not checked out.
"""

import argparse
import importlib.util
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "../.."))


def _load_publish_set():
    path = os.path.join(ROOT, "scripts/git-utilities/publish_set.py")
    spec = importlib.util.spec_from_file_location("publish_set", path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def keys_for(ps, repo, platforms, which="kernel"):
    """Resolved key per platform, following each one's include chain.

    The composition itself is publish_set.derive_keys — deliberately not
    reimplemented here, since a second copy drifting from the first is the fault
    this script exists to catch.
    """
    out = {}
    for p in platforms:
        k = ps.derive_keys(ps.resolve_env(repo, p))
        if k and k[which]:
            out.setdefault(k[which], []).append(p)
    return out


def contrib_dirs(contrib_repo):
    """What ert-contrib-middleware has actually built, by directory name.

    There is no equivalent of make_all_targets.sh to read: contrib's output name
    is composed at build time by
    inx_build_scripts/source-scripts/inx-xbuilder-source-me.sh, whose rule does
    not match platform.mk's. The directory listing is therefore the only honest
    statement of what that producer emits.
    """
    d = os.path.join(contrib_repo, "target_libs")
    if not os.path.isdir(d):
        return None
    return sorted(n for n in os.listdir(d) if os.path.isdir(os.path.join(d, n)))


def kernel_targets(kernel_repo):
    """The targets EHS-kernel actually builds — its make_all_targets.sh TARGETS.

    Read from the sweep list rather than from target/platform/, because a
    platform directory that is not in TARGETS produces nothing. win_x86_unity was
    exactly that case.
    """
    f = os.path.join(kernel_repo, "make_all_targets.sh")
    if not os.path.exists(f):
        return None
    body = open(f, errors="ignore").read()
    m = re.search(r"^TARGETS=\((.*?)^\)", body, re.S | re.M)
    if not m:
        return None
    return [ln.strip() for ln in m.group(1).splitlines()
            if ln.strip() and not ln.strip().startswith("#")]


def report(label, consumed, produced, producer, quiet):
    """One producer/consumer pair. Returns the number of disagreements."""
    missing = sorted(set(consumed) - set(produced))
    orphan = sorted(set(produced) - set(consumed))

    if not quiet:
        print(f"\n\033[1m{label}\033[0m")
        print(f"  consumed {len(consumed)} key(s) by ert-components targets")
        print(f"  produced {len(produced)} key(s) by {producer}")

    if missing and not quiet:
        print(f"\n  NEEDED BUT NEVER BUILT ({len(missing)}) — "
              f"ert-components asks for these; {producer} emits none of them:")
        for k in missing:
            users = consumed[k]
            shown = ", ".join(users[:3]) + (f" +{len(users) - 3} more" if len(users) > 3 else "")
            print(f"      {k:<52} {shown}")

    if orphan and not quiet:
        print(f"\n  BUILT BUT NEVER READ ({len(orphan)}) — "
              f"{producer} emits these; nothing consumes them:")
        for k in orphan:
            src = produced[k]
            print(f"      {k:<52} {', '.join(src) if isinstance(src, list) else ''}")

    # A near-miss pair is the misspelling signature: one side has a key the other
    # spells slightly differently. Surfacing it turns two vague lists into a fix.
    #
    # The test has to be strict. A plain "share the first 8 characters" rule
    # pairs every arm-linux-* key with every other one and buries the real
    # matches: on the contrib set it produced 17 pairs, none of them useful.
    # Requiring either separator-insensitive equality or a common prefix that is
    # both long in absolute terms and a majority of the shorter key keeps it to
    # the genuine near-misses.
    def norm(x):
        return x.replace("-", "").replace("_", "").replace(".", "")

    def near(m, o):
        if norm(m) == norm(o):
            return True
        n = len(os.path.commonprefix([m, o]))
        return n >= 12 and n >= 0.6 * min(len(m), len(o))

    pairs = [(m, o) for m in missing for o in orphan if near(m, o)]
    if pairs and not quiet:
        print("\n  LIKELY MISSPELLINGS — same target, two spellings:")
        for m, o in pairs[:12]:
            print(f"      consumed {m}")
            print(f"      produced {o}")
        if len(pairs) > 12:
            print(f"      ... and {len(pairs) - 12} more")

    if not (missing or orphan) and not quiet:
        print("  producer and consumer agree.")
    return len(missing) + len(orphan)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--kernel-repo", default=os.path.join(ROOT, "../EHS-kernel"))
    ap.add_argument("--contrib-repo", default=os.path.join(ROOT, "../ert-contrib-middleware"))
    ap.add_argument("--what", choices=("kernel", "contrib", "all"), default="all")
    ap.add_argument("--quiet", action="store_true")
    ap.add_argument("--summary", action="store_true",
                    help="one line, always exit 0 — for the CI pre-flight")
    a = ap.parse_args()

    ps = _load_publish_set()
    consumers = [d for d in sorted(os.listdir(os.path.join(ROOT, "target/platform")))
                 if os.path.isdir(os.path.join(ROOT, "target/platform", d))]

    bad = {}

    if a.what in ("kernel", "all"):
        kroot = os.path.abspath(a.kernel_repo)
        if not os.path.isdir(os.path.join(kroot, "target/platform")):
            if not a.summary:
                print(f"  EHS-kernel not checked out at {kroot} — kernel keys not checked.")
        else:
            ktargets = kernel_targets(kroot)
            if ktargets is None:
                print("  could not read TARGETS from EHS-kernel/make_all_targets.sh",
                      file=sys.stderr)
                return 1
            bad["kernel"] = report(
                "KERNEL — ert-components vs EHS-kernel",
                keys_for(ps, ROOT, consumers, "kernel"),
                keys_for(ps, kroot, ktargets, "kernel"),
                f"{len(ktargets)} EHS-kernel targets", a.quiet or a.summary)

    if a.what in ("contrib", "all"):
        croot = os.path.abspath(a.contrib_repo)
        dirs = contrib_dirs(croot)
        if dirs is None:
            if not a.summary:
                print(f"  ert-contrib-middleware not checked out at {croot} — "
                      "contrib keys not checked.")
        else:
            bad["contrib"] = report(
                "CONTRIB — ert-components vs ert-contrib-middleware",
                keys_for(ps, ROOT, consumers, "contrib"),
                {d: [] for d in dirs},
                f"{len(dirs)} built target_libs directories", a.quiet or a.summary)

    if a.summary:
        parts = [f"{k} {v}" for k, v in sorted(bad.items()) if v]
        if parts:
            print("  key agreement: " + ", ".join(f"{n} disagreement(s) in {k}"
                                                  for k, n in
                                                  ((p.split()[0], int(p.split()[1]))
                                                   for p in parts))
                  + " (scripts/software-utilities/check_key_agreement.py)")
        else:
            print("  key agreement: producer and consumer agree")
        return 0

    total = sum(bad.values())
    if total:
        print("")
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
