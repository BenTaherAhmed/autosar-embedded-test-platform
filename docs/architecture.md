# Architecture Overview

## 1. Purpose
This repository demonstrates an AUTOSAR-oriented embedded software verification workflow:
- MCAL-like low-level drivers (mocked)
- Deterministic unit tests in C
- Requirements ↔ Tests traceability checks
- CI automation on Windows/MSVC with static analysis

## 2. High-level Architecture (A → Z)
### A) Low-level driver layer (MCAL mock)
- `src/mcal_mock/`
  - `spi.c/.h` : minimal SPI driver API (init, transmit, receive, transmit-receive)
  - `can.c/.h` : minimal CAN driver API (init, send, receive, queue behavior)

The drivers are **hardware-independent** (mocked) to enable repeatable testing without ECU hardware.

### B) Verification layer (Unit Tests)
- `tests/unit_c/`
  - `test_spi.c`
  - `test_can.c`

Tests are deterministic and validate:
- correct state transitions (init / not init)
- expected API return codes
- queue behavior for CAN (TX/RX), overflow and parameter validation

### C) Requirements layer
- `requirements/requirements.yml`

Requirements are identified by unique IDs (e.g., `REQ_SPI_001`, `REQ_CAN_003`)
and describe expected behavior aligned with safety/ASPICE mindset.

### D) Traceability tooling
- `tools/traceability/trace_check.py`

The script enforces:
- Each test file references at least one requirement via `/* REQ: REQ_xxx */`
- Each requirement is covered by at least one test
- A report is generated: `reports/traceability_report.md`

The traceability report is **generated**, not versioned (CI artifact).

### E) CI pipeline
- `.github/workflows/ci.yml`

Pipeline stages (Windows/MSVC):
1. Configure (CMake)
2. Build (Debug)
3. Run unit tests (CTest)
4. Run traceability check (Python)
5. Static analysis (cppcheck)
6. Upload traceability report artifact

## 3. Repository Structure
- `.github/workflows/` : CI pipeline
- `src/` : embedded C sources (MCAL mock)
- `tests/` : verification tests
- `requirements/` : requirements definition
- `tools/` : automation tooling (traceability, lint)
- `reports/` : generated outputs (ignored by git)
- `docs/` : documentation

## 4. How to extend
Next modules to add:
- I2C mock + tests + requirements
- UDS/Diag mock + test harness
- Coverage reporting
- MISRA-focused static analysis ruleset
