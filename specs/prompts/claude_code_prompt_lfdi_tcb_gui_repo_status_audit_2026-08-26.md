# Claude Code Prompt: LFDI-TCB and LFDI-GUI Repository Status Audit — 2026-08-26

## Objective
Audit the two remaining LFDI software repos — `LFDI-TCB` and `LFDI-GUI`
— in a single pass. You are running inside a VS Code multi-root
workspace that also contains `LFDI_API` (already audited on 2026-08-25;
do not re-audit it, but its findings below are relevant context). Treat
`LFDI-TCB` and `LFDI-GUI` as **fully independent git repositories** —
separate remotes, separate commit history, separate `PIPELINE_STATUS.md`,
separate everything. Do not mix their commits together or push one from
inside the other's working copy.

## Background Context

### From the LFDI Operators Manual (updated 2026-02-24)
Three repos are named in the manual's Appendix:
- `LFDI-TCB` — TCB embedded firmware (C, STM32CubeIDE, STM32F427VIT6 MCU,
  "SUPER LOOP" architecture in `main.c`, LUTs in `StageDefinitions.h`)
- `LFDI-API` — Python API for serial/RS-232 commanding of the TCB
- `LFDI_Calibration` — listed, purpose not otherwise detailed in the manual

The manual also describes a LabVIEW-built `LFDI Demonstration GUI.exe`
as the GUI, run from a local `.exe` path — it does **not** mention a
`LFDI-GUI` repo by that name at all.

### From the 2026-08-25 `LFDI-API` audit (already completed)
`LFDI-API` turned out to be much broader than "a serial API" — it also
contains calibration code, LUT generation, PID tuning, data
processing/plotting, and its own Flask-based web GUI. That means it may
functionally overlap with `LFDI_Calibration` and/or `LFDI-GUI` — the
manual's clean three/four-repo split may not reflect what was actually
built. `LFDI-API` was last committed 2024-10-30 (dormant since).

**A key question this audit should try to answer**: are `LFDI-TCB` and
`LFDI-GUI` genuinely separate, actively-scoped codebases, or does one or
both duplicate/overlap functionality already found in `LFDI-API`? Look
for shared code, copy-pasted modules, README cross-references, or
matching function/file names as evidence either way.

## Instructions — repeat this full sequence once for `LFDI-TCB`, then
## once for `LFDI-GUI`. Complete one repo fully before starting the other.

### 0. Locate and confirm the repo
From the workspace root, identify the folder for this repo (expected
sibling folders to `LFDI_API`, likely named `LFDI-TCB` and `LFDI-GUI`).
`cd` into it and confirm identity:
```
git remote -v
```
If the folder name or remote doesn't match what's expected, note that
clearly rather than guessing — report the mismatch and pause before
proceeding on that repo.

### 1. Save this prompt into the repo
```
mkdir -p specs/prompts
```
Write the full, verbatim text of this prompt document to:
`specs/prompts/claude_code_prompt_lfdi_tcb_gui_repo_status_audit_2026-08-26.md`

Confirm git works (`git --version`; if not found, resolve the binary
path yourself — on this machine it has previously been found at
`C:\Users\iguser\AppData\Local\GitHubDesktop\app-*\resources\app\git\cmd\git.exe`,
though the exact version folder may have changed).

```
git add specs/prompts/claude_code_prompt_lfdi_tcb_gui_repo_status_audit_2026-08-26.md
git commit -m "Add repo status audit prompt"
```

### 2. Orient yourself
```
cat PIPELINE_STATUS.md 2>/dev/null || echo "No PIPELINE_STATUS.md found — will create one."
git log --oneline -20
git branch -a
```

### 3. Inventory the repo
- Full directory tree (exclude build artifacts, `.git`, vendored libs)
- Primary language(s) and toolchain
- Entry point(s)
- Dependency manifests
- README(s) and any in-repo architecture docs — summarize their stated
  purpose in your own words

### 4. Assess actual state vs. documented state
- Does the code match what the manual describes for this repo (see
  Background Context)? Note matches, mismatches, anything described that
  you can't find
- Last commit date, commit frequency/recency — active, frozen, or
  abandoned?
- Open branches suggesting unmerged work
- TODOs, FIXMEs, commented-out blocks, placeholder functions
- Tests present, and whether they currently pass (run if safe and quick;
  no hardware-in-the-loop tests)
- Whether the repo builds/imports cleanly (report errors verbatim,
  don't fix them)
- **Overlap check**: compare against what you know of `LFDI-API`'s
  contents (see Background Context) — flag any file/function names,
  README text, or functionality that looks duplicated or copy-pasted
  between this repo and `LFDI-API`

### 5. Write the report
`specs/LFDI_REPO_STATUS_<reponame>_2026-08-26.md`, containing:
- Repo identity confirmation
- Inventory summary
- State assessment: **Working / Documented**, **Present but Unclear**,
  **Missing or Stale**
- Cross-repo overlap findings (from Step 4)
- Prioritized "Recommended Next Steps" (3–7 items)

Do not modify source code in this pass — audit only.

### 6. Close out this repo
Update this repo's own `PIPELINE_STATUS.md` with a one-line summary, then:
```
git add specs/LFDI_REPO_STATUS_<reponame>_2026-08-26.md PIPELINE_STATUS.md
git commit -m "Add repo status audit — 2026-08-26"
```

### 7. Attempt to push this repo
```
git push
```
- Success → note it and move to the next repo.
- Fails with an auth/SSO-style error → do not retry repeatedly, do not
  force-push, do not touch credentials or the remote URL. Just note this
  repo has pending local commits that need a manual push (same pattern
  as the `LFDI-API` situation from 2026-08-25/26), and move on to the
  next repo — don't let one repo's push failure block auditing the other.

## After both repos are done
Report back in plain language, per repo:
- What it actually is / does, vs. what the manual says
- Current state (active / dormant / abandoned) and since when
- Any overlap found with `LFDI-API`
- Whether the push succeeded or is pending manual action
- Top recommended next steps

Keep this summary comparative and readable — the user will use it to
decide how to reconcile the manual's repo-boundary description with what
actually exists across all three (now four, counting `LFDI_Calibration`
if it turns out to be real and separate) repos.
