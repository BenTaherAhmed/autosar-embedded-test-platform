![CI](https://github.com/BenTaherAhmed/autosar-embedded-test-platform/actions/workflows/ci.yml/badge.svg)

# AUTOSAR Embedded Test Platform

Embedded AUTOSAR-oriented test platform demonstrating a professional workflow for **unit testing**, **requirements-to-tests traceability**, and **CI automation** on Windows/MSVC.

This repository is designed as a portfolio-grade project aligned with **automotive software development** practices (AUTOSAR mindset, verification, traceability, and automation).

---

## What this project demonstrates (A → Z)

### A. AUTOSAR-style low-level software (MCAL mock)
The project provides a minimal **AUTOSAR-like MCAL mock** for SPI:
- `src/mcal_mock/spi.c`
- `src/mcal_mock/spi.h`

It exposes a small API inspired by AUTOSAR driver design:
- initialization/deinitialization
- transmit / receive / transmit-receive
- parameter checks and deterministic behavior (useful for verification)

This mock enables testing the behavior of low-level interfaces without real hardware.

---

### B. Unit testing in C (deterministic + reproducible)
A unit test validates the SPI mock behavior:
- `tests/unit_c/test_spi.c`

The test verifies:
- error handling when the driver is not initialized
- correct initialization state transitions
- deterministic transmit/receive behavior

Tests are executed with **CTest** (CMake testing).

---

### C. Requirements ↔ Tests traceability (ISO 26262 / ASPICE mindset)
The repository includes an automated traceability checker:
- `tools/traceability/trace_check.py`

It verifies:
- every test file contains at least one requirement tag (`REQ: REQ_...`)
- every requirement is covered by at least one test
- a traceability report is generated automatically:
  - `reports/traceability_report.md`

This mimics a lightweight requirements coverage process commonly expected in safety-oriented automotive projects.

---

### D. CI automation on GitHub Actions (Windows/MSVC)
A Windows CI workflow automatically runs on every push and pull request:
- `.github/workflows/ci.yml`

Pipeline includes:
- Python setup + dependencies
- CMake configure
- build (Debug and Release)
- run unit tests with CTest
- run traceability checks and upload report artifact
- run static analysis (cppcheck)

---

### E. Static analysis (cppcheck)
A basic static analysis stage is included:
- `tools/lint/run_cppcheck.ps1`

It demonstrates:
- early detection of suspicious patterns (style/performance/portability)
- integration of analysis in CI

---

## Repository structure

```text
.github/workflows/     # CI pipelines (GitHub Actions)
src/                   # Embedded C code (MCAL mock + future drivers)
tests/                 # Unit tests (C / future Python/CAPL examples)
tools/                 # Tooling: traceability, linting, automation helpers
requirements/          # Requirements definition (IDs, descriptions)
reports/               # Generated reports (traceability, future test reports)
docs/                  # Documentation (how to run, architecture notes)