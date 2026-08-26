# LFDI-TCB Repository Status Audit — 2026-08-26

## 1. Repo Identity Confirmation

- Path: `C:\Users\iguser\Documents\GitHub\LFDI-TCB`
- `git remote -v`:
  - `origin` = https://github.com/mitchem890/LFDI-TCB.git (personal fork, fetch+push)
  - `upstream` = https://github.com/NCAR/LFDI-TCB.git (fetch+push)
- Folder name and remote name both match the manual's `LFDI-TCB` entry. No mismatch.
- **Note on remote topology**: unlike the sibling repos audited elsewhere (which push
  straight to an NCAR org repo), this repo's `origin` is a personal fork
  (`mitchem890/LFDI-TCB`), with `upstream` pointing at the NCAR org repo. A `git push`
  here goes to the fork, not directly to NCAR — it may or may not hit the same
  SSO/403 wall that blocked the `LFDI-API` push on 2026-08-25/26.
- Working tree was clean at start of audit (`nothing to commit, working tree clean`).

## 2. Inventory Summary

**Type**: STM32CubeIDE embedded firmware project (C), for an STM32F427VIT6 MCU.

**Top-level structure**:
- `.cproject`, `.project`, `.mxproject`, `LFDI Tuning Control Board.ioc` — STM32CubeIDE/CubeMX
  project files
- `STM32F427VITX_FLASH.ld` / `_RAM.ld` — linker scripts
- `Core/Inc`, `Core/Src` — application source (21 headers, 20 source files)
- `Core/Startup` — startup assembly
- `Drivers/CMSIS`, `Drivers/STM32F4xx_HAL_Driver` — vendor HAL/CMSIS libraries
- `Middlewares/ST/STM32_USB_Device_Library` — ST USB CDC middleware (USB-serial comms,
  the counterpart to `LFDI-API`'s serial link)
- `USB_DEVICE/App`, `USB_DEVICE/Target` — USB device class glue
- `Debug/`, `Release/` — build output directories (checked into the repo; build artifacts)
- `README.md` — architecture documentation
- No Python, no `requirements.txt`, no Flask app, no HTML templates anywhere in the tree.

**Entry point**: `Core/Src/main.c` — confirmed a `//SUPER LOOP!!!!` comment immediately
preceding `while (1)` at line 316 (994-line file), exactly as the manual describes.

**LUTs**: `Core/Inc/StageDefinitions.h` exists, 251 lines, exactly as the manual describes.

**Core modules** (object-oriented C, per the README's own description): `TuningControlBoard`
(top-level struct aggregating everything), `Compensator` (LCVR tuning via LUT),
`Controller`/`Heater`/`HeaterController`/`PID` (heater control loop), `DAC`/`DAC_Unit_Test`
(DAC8718 driver + hardware-in-the-loop self-tests), `TMP117` (temp sensor driver),
`GPIO`, `BipolarOutput` (FLC drive), `CameraController` (stubbed/future), `CurrentSensor`,
`UI` (serial command-line UI over USB CDC), `stringfifo` (string FIFO for UI I/O).

**README.md**: describes the firmware's object-oriented struct hierarchy
(`sTuningControlBoard` → `sCompensator`/`sController`/`sGPIO`/`sBipolarOutput` →
`sDAC`/`sTMP117`/`sPID`) in detail, matching the actual code. No cross-references to
`LFDI-API`, `LFDI-GUI`, or `LFDI_Calibration` found anywhere in the README or source
comments.

**Dependency manifests**: none in the Python/npm sense — dependencies are the vendored
ST HAL/CMSIS/USB middleware checked into `Drivers/` and `Middlewares/`, standard for an
STM32CubeIDE project.

## 3. State Assessment

**Working / Documented** (matches the manual):
- Language/toolchain (C, STM32CubeIDE, STM32F427VIT6) — confirmed via `.cproject`/`.ioc`
  and linker scripts.
- "SUPER LOOP" architecture in `main.c` — confirmed verbatim.
- LUTs in `StageDefinitions.h` — confirmed, present and populated.
- Recent commit history is dominated by hardware-feature work (current sensing, bipolar
  output, GPIO, PID tuning-on/off) consistent with active TCB firmware development, not
  drift into unrelated scope.

**Present but Unclear**:
- Three additional remote branches with unmerged or partially-merged work:
  `origin/QHC_KSO_Final` (last commit 2024-04-18, by Damon Burke — appears to be a fork
  for a different instrument/site, "KSO", not obviously mergeable to main as-is),
  `origin/HeaterControllerUpdate` (2024-04-02, fully merged into main — no diff against
  main), and `origin/Working` (2023-01-29, one unmerged commit "Updated UI" not on main).
  Also a local-only branch `QHC_KSO_Final` mirroring the remote. These represent
  possibly-abandoned side branches worth a maintainer review.
- Hardware-in-the-loop unit tests exist (`DAC_Unit_Test.c/.h`, `unit_test_*` functions for
  DAC8718 register defaults) but require physical SPI-connected hardware to run — not
  executed per audit constraints, and no CI/host-side test harness exists to run them
  without hardware.
- Build was not attempted: this machine has no ARM/STM32 toolchain (`arm-none-eabi-gcc`
  not found) and no STM32CubeIDE installation, so "does it build" could not be verified
  in this pass.
- 5 TODO comments found (`CameraController.c`, `TuningControlBoard.c`, `UI.c` x3) —
  minor, none block core functionality (e.g., SD card save not implemented, camera
  control noted as future work).

**Missing or Stale**: nothing found that the manual claims but the repo lacks.

**Activity**: last substantive commit before this audit was 2026-02-22 (~6 months ago,
"Fixed Bipolar output"), following a burst of active development in Feb 2026 (current
sensing, GPIO/bipolar menus, bug fixes). 104 total commits since project start
(2022-12-15). This is markedly more recent/active than `LFDI-API`, which has been
dormant since 2024-10-30 — `LFDI-TCB` is the healthiest of the LFDI repos audited so far
in terms of recency.

## 4. Cross-Repo Overlap Findings (vs. `LFDI-API`)

**No overlap found.** This repo is pure embedded C firmware:
- No Python files anywhere in the tree.
- No Flask app, no HTML templates, no web GUI code.
- No calibration scripts, LUT-generation scripts, or standalone PID-tuning tooling in
  Python — the PID logic here (`PID.c`/`PID.h`) is a small embedded C struct
  (`sPID`: `LastP[256]`, `DeltaT`, `Ep/Ei/Ed/Effort`, `IntegratorCount`) used for
  closed-loop heater control on-device. This is a different artifact from whatever
  Python-side PID tuning `LFDI-API` contains (that audit described "PID tuning" as
  part of its scope) — no shared filenames, no shared code, and it's architecturally
  expected that firmware would have its own on-device PID loop separate from a host-side
  tuning tool. Worth a follow-up check of whether `LFDI-API`'s PID tuning code
  actually talks to *this* struct's parameters over serial, but there's no code
  duplication.
- The LUTs in `StageDefinitions.h` are C arrays compiled into firmware; if `LFDI-API`
  generates or contains equivalent LUT data (as its 2026-08-25 audit suggested with "LUT
  generation"), that would be a data-format overlap worth checking cross-repo, but no
  literal file or code duplication exists — `LFDI-API` audit is out of scope here to
  re-verify.
- README and code comments contain zero references to `LFDI-API`, `LFDI-GUI`, or
  `LFDI_Calibration`.

**Conclusion**: `LFDI-TCB` genuinely appears to be a separate, tightly-scoped embedded
firmware codebase, consistent with the manual's description. It does not exhibit the
scope creep seen in `LFDI-API`.

## 5. Recommended Next Steps

1. Review and either merge or explicitly retire the three stale branches
   (`origin/QHC_KSO_Final`, `origin/Working`, local `QHC_KSO_Final`) — determine if the
   "KSO" branch represents a fork for a different site/instrument that should live in its
   own repo, and whether `Working`'s "Updated UI" commit (2023-01-29) is still wanted.
2. Confirm whether `origin/HeaterControllerUpdate` (fully merged into main, per this
   audit) can be deleted as a stale/completed branch.
3. Manually verify `git push` reached the fork (`origin`) successfully (see push result
   below); if the audit commits need to reach NCAR's `upstream`, a PR from the fork will
   be required — pushing to `origin` alone does not update `upstream/main`.
4. Set up (or document) a way to build this project outside STM32CubeIDE (e.g., a
   documented CLI/Makefile build path) so future audits or CI can verify compilation
   without requiring the full IDE installed on the auditing machine.
5. Address the 5 outstanding TODOs, particularly the unimplemented SD-card
   `SaveConfig()` call in `UI.c` and the incomplete `ShowAllGPIO` call — both are
   flagged directly in code as incomplete.
6. Cross-check with the `LFDI-API` and `LFDI_Calibration` maintainers whether the LUT
   values in `StageDefinitions.h` are hand-maintained here or generated/mirrored from
   `LFDI-API`'s LUT-generation code — if the latter, establish a single source of truth
   to avoid divergence between the two repos' copies of the same calibration data.
7. Since this is the most actively maintained of the LFDI repos (last real activity
   2026-02-22, vs. `LFDI-API`'s 2024-10-30), consider using it as the reference point
   for reconciling the Operators Manual's repo-boundary description against actual
   repo scope.
