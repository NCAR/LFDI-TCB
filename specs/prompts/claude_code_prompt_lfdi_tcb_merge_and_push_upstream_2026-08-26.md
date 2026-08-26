# Claude Code Prompt: Merge and Push LFDI-TCB to NCAR Upstream — 2026-08-26

## Objective
Complete the LFDI-TCB reconciliation identified in the prior audit
(2026-08-26 remote-migration prompt). Confirmed history:
- Local `main` has 3 commits NCAR's `upstream/main` doesn't: `fcbc5a2`,
  `7fabd03` (2026-08-25 audit), `d4280de` (prompt-save housekeeping) —
  all non-code/housekeeping.
- `upstream/main` has 3 commits local doesn't: `b4973ab` (merge PR #8),
  `6c2dc90` (`Core/Src/BipolarOutput.c`, +2 lines), `d1a03fd` (trivial).
- Both branches share a common ancestor at `37ea56c`.
- The user has reviewed the `BipolarOutput.c` diff and approved merging.

Merge the two histories and push the result to `NCAR/LFDI-TCB` directly
via the existing `upstream` remote — no fork, no remote renaming needed.

## Instructions

### 1. Save this prompt into the repo
```
cd LFDI-TCB   # adjust path if needed
mkdir -p specs/prompts
```
Write the full, verbatim text of this prompt document to:
`specs/prompts/claude_code_prompt_lfdi_tcb_merge_and_push_upstream_2026-08-26.md`

Confirm git works (resolve the binary path yourself if needed, as in
prior prompts).
```
git add specs/prompts/claude_code_prompt_lfdi_tcb_merge_and_push_upstream_2026-08-26.md
git commit -m "Add merge-and-push prompt"
```

### 2. Re-verify state before touching anything
```
git fetch upstream
git log upstream/main..main --oneline
git log main..upstream/main --oneline
```
Confirm this still matches the summary above (3 commits each way,
`BipolarOutput.c` the only real code change). If it has changed — e.g.
more commits now appear on `upstream/main` than previously reported —
stop and report the difference rather than proceeding on stale
assumptions.

### 3. Merge
```
git merge upstream/main -m "Merge NCAR upstream/main (BipolarOutput.c update + PR #8)"
```
- If this completes with no conflicts (expected, per the prior audit's
  assessment that the changed files don't overlap), proceed to Step 4.
- If it reports conflicts, STOP. Do not attempt to auto-resolve them —
  run `git status` to show which files conflict, report that back
  verbatim, and do not commit or push a conflicted merge.

### 4. Verify the merge result
```
git log --oneline -8
git diff upstream/main..main --stat
```
Confirm `BipolarOutput.c`'s change is now present in local history and
nothing looks unexpected (e.g. no unrelated files touched).

### 5. Push to NCAR directly via upstream
```
git push upstream main
```
- Success → proceed to Step 6.
- Fails with a permission error → this means the user doesn't have
  direct write access to `NCAR/LFDI-TCB` even though `upstream` fetches
  fine (read access via origin fetch is not the same as write access).
  Report this plainly — it needs an NCAR org admin to grant write
  access. Don't attempt to push to the old fork instead as a
  workaround; that would defeat the point of this migration.

### 6. Reconcile the `origin` remote for future clarity
Since `upstream` now IS the actively-used NCAR repo and the fork is
being retired:
```
git remote -v
```
Report what `origin` currently points to (expected: the personal fork,
`mitchem890/LFDI-TCB.git`). Do not delete or rename it automatically —
just report it, and ask the user whether they'd like `origin` repointed
to `NCAR/LFDI-TCB` directly in a future prompt (a one-line
`git remote set-url` change), now that the merge/push is proven to work
via `upstream`. Leave remotes as-is for this prompt.

### 7. Update PIPELINE_STATUS.md
Note: `upstream/main` (NCAR/LFDI-TCB) merged in on 2026-08-26, including
the `BipolarOutput.c` update from PR #8; local history and NCAR are now
in sync as of commit <new merge commit hash>.
```
git add PIPELINE_STATUS.md
git commit -m "Document merge with NCAR upstream/main"
git push upstream main
```

## Final report
Plain language: did the merge complete cleanly, did the push to NCAR
succeed, and what (if anything) is left — specifically call out whether
`origin` still points at the retired fork and needs a follow-up cleanup
prompt.
