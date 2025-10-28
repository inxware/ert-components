# Updating from the original source tree

There are five additional files to remove and recover .git repo to update and cherry-pick, and to 
describe the entire process:

- `INX_README.md` (this file) describes the ".git" deletion and restoration process.
- `reset_git.sh` is to restore the original git repository to the latest commit while keeping local changes.
- `ungit.sh` is to store current git status and delete ".git" folder
   - After just cloned the repository, run `./ungit.sh init`. Note that this should ONLY run when the `.git` directory is in place!
   - Normal execution should be `./ungit,sh`.
- `current_commit` is the current commit hash of the original repository for reference.
- `remote_url` stores the remote origin URL for git repo restoration.
