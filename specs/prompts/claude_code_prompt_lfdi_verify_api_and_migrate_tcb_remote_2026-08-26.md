# Claude Code Prompt: Verify LFDI-API Push + Migrate LFDI-TCB Off Personal Fork — 2026-08-26

## Objective
Two independent tasks, run in this order, in a VS Code multi-root
workspace containing `LFDI_API`, `LFDI-TCB`, and `LFDI-GUI` as sibling
folders.

**Task A**: Sanity-check that `LFDI_API` is actually fully pushed (it
was previously confirmed caught up at `fdd4ba8` on 2026-08-26, but a
later audit summary re-flagged it as having a pending SSO push issue —
that may be stale/incorrect, so re-verify rather than assume either way).

**Task B**: `LFDI-TCB`'s `origin` remote currently points to a personal
fork, `mitchem890/LFDI-TCB.git`. Mitch has left the organization. The
user wants a **complete separation from that fork** and to maintain only
`NCAR/LFDI-TCB` going forward. This requires care — do not force-push,
do not rewrite history, and do not silently drop commits. If local
history and NCAR's history have diverged in incompatible ways, stop and
report the specifics rather than guessing at a resolution.

## Task A: Verify LFDI-API

```
cd LFDI_API   # adjust path if the workspace folder name differs
git fetch
git log origin/main..main --oneline
git log main..origin/main --oneline
```
- If both commands return nothing, `LFDI_API` is genuinely fully synced
  — report this plainly and move on to Task B.
- If the first command shows commits, there ARE unpushed local commits
  after all — report exactly what they are (don't push yet without
  telling the user what you're about to push).

Do not modify `LFDI_API` further in this prompt beyond this check.

## Task B: Migrate LFDI-TCB off the personal fork

```
cd ../LFDI-TCB   # adjust path if needed
```

### 1. Save this prompt into the repo
```
mkdir -p specs/prompts
```
Write the full, verbatim text of this prompt document to:
`specs/prompts/claude_code_prompt_lfdi_verify_api_and_migrate_tcb_remote_2026-08-26.md`

Confirm git works (resolve the binary path yourself if needed, as in
prior prompts).
```
git add specs/prompts/claude_code_prompt_lfdi_verify_api_and_migrate_tcb_remote_2026-08-26.md
git commit -m "Add TCB remote migration prompt"
```

### 2. Diagnose current state
```
git remote -v
git log --oneline -10
```
Confirm `origin` currently points to `mitchem890/LFDI-TCB.git` (or
whatever it actually shows — report exactly what's there).

### 3. Add the NCAR upstream as a new remote and compare histories
```
git remote add ncar-upstream https://github.com/NCAR/LFDI-TCB.git
git fetch ncar-upstream
git log ncar-upstream/main..main --oneline
git log main..ncar-upstream/main --oneline
```
These two commands tell you the actual relationship between local
history and the real NCAR repo:
- **First command** = commits that exist locally but not on NCAR's main
  (this should include at minimum the 2 local-only commits from the
  2026-08-26 audit, plus possibly more if the fork itself had diverged
  from NCAR).
- **Second command** = commits that exist on NCAR's main but not
  locally (this would mean NCAR's repo has moved on since the fork was
  created/last synced, e.g. other contributors' work).

### 4. Decide how to proceed, based on what Step 3 showed
- **If the second command (NCAR-ahead) returns nothing** — local history
  is a clean superset of NCAR's, meaning a fast-forward push is safe.
  Proceed to Step 5.
- **If the second command returns commits** — histories have diverged;
  NCAR's repo has commits this local copy doesn't have. Do NOT attempt
  to merge, rebase, or push anything automatically. Stop here and
  report exactly what's on each side (commit hashes, messages, authors,
  dates) so the user can decide how to reconcile it — this is a human
  judgment call, not a mechanical one.

### 5. Repoint origin to NCAR and push (only if Step 4 confirmed it's safe)
```
git remote rename origin mitchem890-fork-archive
git remote rename ncar-upstream origin
git push origin main
```
This keeps the old fork remote around locally (renamed, not deleted) as
a reference in case anything needs to be double-checked later, while
`origin` now correctly points at `NCAR/LFDI-TCB`.

- If the push succeeds, great — proceed to Step 6.
- If it fails with a permission error, the user's GitHub account may not
  have direct write access to `NCAR/LFDI-TCB` (separate from the fork
  access they had). Report this plainly — this needs an NCAR org admin
  to grant write access, or the user goes through a fork→PR flow
  instead. Don't attempt workarounds.

### 6. Update PIPELINE_STATUS.md
Note in `LFDI-TCB`'s `PIPELINE_STATUS.md`: the remote was migrated from
`mitchem890/LFDI-TCB` (personal fork, owner has left the organization)
to `NCAR/LFDI-TCB` directly on 2026-08-26, and whether the push
succeeded.
```
git add PIPELINE_STATUS.md
git commit -m "Document remote migration from personal fork to NCAR/LFDI-TCB"
git push
```

### 7. What this prompt does NOT do
It does not delete or archive the `mitchem890/LFDI-TCB` fork on GitHub
itself — that's a GitHub-side action (only Mitch's account or a GitHub
org admin with appropriate permissions can do that) and out of scope
here. Note this plainly in your final report as something the user may
want to follow up on separately with whoever administers the NCAR org.

## Final report
Plain language, covering: Task A's result (genuinely synced or not),
and Task B's result — whether the migration completed cleanly, hit a
divergence requiring the user's judgment, or hit a permissions wall.
Don't paste raw git output unless something looks genuinely unexpected.
