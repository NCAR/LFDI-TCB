# Claude Code Prompt — LFDI-TCB Onboarding Document

**File:** `specs/prompts/claude_code_prompt_lfdi_tcb_onboarding_doc_2026-08-26.md`
**Repo:** LFDI-TCB
**Date:** 2026-08-26

---

## Step 0 — Orientation (do this first, every session)

```
cat PIPELINE_STATUS.md
```

Read it fully before doing anything else so you know what state the repo is in and whether related work is already in flight.

## Step 1 — Save this prompt into the repo

Before starting the task, save this exact prompt file into the repo and commit it on its own:

- Path: `specs/prompts/claude_code_prompt_lfdi_tcb_onboarding_doc_2026-08-26.md`
- `git add` that file and commit with message: `docs: add prompt for LFDI-TCB onboarding document`

Do not bundle this commit with any of the work below.

---

## Task

Carefully review the **LFDI-TCB** repository (the embedded C firmware running on the Tuning Control Board's STM32F427VIT6 microcontroller) and produce a **short reference document (under 4 pages)** that will help a **new electrical engineer** come up to speed with how the firmware is organized and why it was written the way it was.

This is an onboarding artifact, not a full design spec — bias toward brevity, orientation, and "here's where to look next," not exhaustive documentation.

### 1. Review the repository

Actually read the code — don't infer structure from filenames alone. At minimum, examine:

- Overall folder/module layout (e.g. `Core/Inc`, `Core/Src`, `Core/Startup`, drivers, any vendor/HAL folders, build config)
- `main.c` — initialization sequence and the main super-loop / interrupt structure
- `StageDefinitions.h` (or equivalent) — the LCVR calibration LUT storage and stage definitions
- The serial command parser / protocol implementation (the Main / Controller / Compensator menu structure described in the Operators Manual) — confirm what's actually implemented vs. documented
- PID heater control logic
- LCVR (compensator) tuning/DAC control logic
- Any GPIO / camera-trigger / pulse-sequence generation code
- I2C handling for the TMP117 temperature sensors
- Any recent commit history (last ~6–12 months) that indicates active areas of development or recent fixes worth flagging to a newcomer
- Build/toolchain notes (STM32CubeIDE project structure, `.elf` output, any Makefile/CMake if present)

Cross-reference against (context already available to you, don't re-derive):
- The LFDI Operators Manual's description of the TCB (STM32F427VIT6 + DAC8718SPAG, Main/Controller/Compensator serial menus, temperature-compensated tuning algorithm, interrupt-driven `main()`/SUPER LOOP starting ~line 316)
- The LFDI Repo Relationships narrative, which identifies LFDI-TCB as the **authoritative source of truth** for the serial command protocol, the most actively maintained of the three LFDI repos, and the reference against which LFDI-GUI and LFDI-API's protocol implementations should be judged

Where the code disagrees with the Operators Manual (e.g. commands, menu behavior, LUT indexing, pin/channel counts), note the discrepancy explicitly in the document rather than silently following whichever source seems more authoritative.

### 2. Produce PNG graphics

Generate actual PNG diagrams (not just prose) to include in the document. At minimum:

- A **repo/module structure diagram** — folders and key files, and how they relate (e.g. what `main.c` calls into, where the LUTs live, where the serial parser lives)
- A **firmware execution flow diagram** — startup/initialization → interrupt setup → SUPER LOOP (sample TMP117s → update LCVR compensators → update heater PWM), matching what you actually find in `main.c`

Use whatever tool is appropriate in this environment (e.g. graphviz, matplotlib, or a simple diagramming script) to render these as PNG files. Keep them clean and readable at document scale — this is for a newcomer's first read, not a full schematic.

### 3. Write the document

Format: Markdown or PDF, whichever renders the PNGs cleanly and is easiest for a new hire to read — your choice, note which you picked and why in the commit message.

Length: **strictly under 4 pages** when rendered. This is a "get oriented fast" document, not a manual.

Structure (adapt as needed, but cover all of this):

1. **What this repo is** — one paragraph: what LFDI-TCB does, what hardware it runs on, its role relative to LFDI-GUI and LFDI-API
2. **Repo structure** — the module diagram + a short walkthrough of what lives where
3. **How it runs** — the execution-flow diagram + a short walkthrough of the SUPER LOOP and why it's structured that way (interrupt-driven, timing constraints, etc.)
4. **The serial command protocol** — brief summary of the Main/Controller/Compensator menu structure, and a pointer to where in the code to find the authoritative command list
5. **Where a new EE should look first** — a short, opinionated punch list (e.g. "start in `main.c` line X to see the loop; the LUT format is defined in `StageDefinitions.h`; don't touch X without reading Y first")
6. **Known gaps / discrepancies vs. the Operators Manual** — anything you found in step 1 that a newcomer should be warned about

Save the document and its PNGs under `docs/onboarding/` in the repo (create the folder if it doesn't exist). Use clear filenames, e.g. `docs/onboarding/lfdi_tcb_onboarding_2026-08-26.md` (or `.pdf`) plus `docs/onboarding/img/*.png`.

### 4. Commit the work

`git add` the new document and PNGs, commit with a descriptive message (e.g. `docs: add LFDI-TCB onboarding doc for new EEs`).

---

## Step 5 — Close out (do this last, every session)

1. Update `PIPELINE_STATUS.md` with a short entry describing what was reviewed, what was produced, its location, and any discrepancies flagged against the Operators Manual.
2. `git add PIPELINE_STATUS.md` and commit with message: `chore: update pipeline status after LFDI-TCB onboarding doc`

Do not ask the user to move, copy, or commit any files manually — perform all file placement and git operations yourself within this session.
