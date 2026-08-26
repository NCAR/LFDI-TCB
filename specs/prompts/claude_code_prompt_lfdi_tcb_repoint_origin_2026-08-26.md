# Claude Code Prompt: Repoint LFDI-TCB Origin to NCAR — 2026-08-26

## Objective
`LFDI-TCB`'s `origin` remote still points at the retired personal fork
`mitchem890/LFDI-TCB.git`. The `upstream` remote (`NCAR/LFDI-TCB.git`)
is now confirmed to work for both fetch and push, and local `main` is
fully merged and in sync with it (merge commit `90e5245`, followed by
`bbc6c48`). Repoint `origin` to `NCAR/LFDI-TCB` directly so it's the
primary remote going forward, and retire the fork reference.

## Instructions

### 1. Save this prompt into the repo
```
cd LFDI-TCB   # adjust path if needed
mkdir -p specs/prompts
```
Write the full, verbatim text of this prompt document to:
`specs/prompts/claude_code_prompt_lfdi_tcb_repoint_origin_2026-08-26.md`

Confirm git works (resolve the binary path yourself if needed, as in
prior prompts).
```
git add specs/prompts/claude_code_prompt_lfdi_tcb_repoint_origin_2026-08-26.md
git commit -m "Add repoint-origin prompt"
```

### 2. Confirm current state before changing anything
```
git remote -v
git fetch upstream
git log upstream/main..main --oneline
git log main..upstream/main --oneline
```
Both `log` commands should return nothing (fully in sync, per the prior
merge). If either shows commits, something has changed since the last
prompt — stop and report exactly what, rather than proceeding.

### 3. Repoint origin
```
git remote set-url origin https://github.com/NCAR/LFDI-TCB.git
git remote -v
```
Confirm `origin` now shows the NCAR URL for both fetch and push.

### 4. Clean up the now-redundant `upstream` remote
`origin` now points at the same NCAR URL that `upstream` already did —
`upstream` is a harmless duplicate at this point. Remove it to avoid any
future confusion about which remote is authoritative:
```
git remote remove upstream
```

Note: Step 3 overwrote `origin`'s URL in place, so there is no longer
any remote pointing at the personal fork `mitchem890/LFDI-TCB.git`
either. That's expected and fine. If you'd specifically like a named
reference to the old fork kept around for future lookup, you can
optionally add one back:
```
git remote add mitchem890-fork-retired https://github.com/mitchem890/LFDI-TCB.git
```
This is entirely optional — skip it unless it seems useful.

### 5. Verify a fetch and dry push work against the new origin
```
git fetch origin
git push origin main --dry-run
```
Confirm both succeed without errors.

### 6. Update PIPELINE_STATUS.md
Note: `origin` repointed from the retired `mitchem890/LFDI-TCB` fork to
`NCAR/LFDI-TCB` directly on 2026-08-26; this is now the sole/primary
remote.
```
git add PIPELINE_STATUS.md
git commit -m "Repoint origin to NCAR/LFDI-TCB, retire personal fork remote"
git push origin main
```

## Final report
Plain language: confirm `origin` now points at `NCAR/LFDI-TCB`, whether
the old fork remote was kept (named) or removed, and that a normal
`git push` now goes straight to NCAR with no fork or upstream juggling
required.
