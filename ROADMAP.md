# BeVinci Roadmap

BeVinci is a Haiku-native video editing workstation based on Medo.

The project follows a conservative fork strategy:

1. Preserve the original Medo foundation.
2. Establish a clean BeVinci source and build baseline.
3. Harden the existing editor before adding large features.
4. Document current capabilities and extension points.
5. Extend BeVinci step by step toward workstation-grade video editing.

## Current baseline

BeVinci currently tracks the Medo R1.0.0-Beta1-5.1 release line plus the
minimal fixes and hardening needed for a clean Haiku R1/beta5 build.

The current clean build baseline is tagged as:

    baseline-build-clean-0010

This baseline confirms that:

- the main `Medo` executable builds and links,
- the bundled add-ons build and link,
- known non-fatal compiler warnings from the initial baseline audit have been
  resolved,
- the build-noise audit has been updated to match the current state.

## Completed baseline hardening

### Phase 0001: Baseline documentation

- Documented upstream origin.
- Documented the Haiku R1/beta5 build path.
- Added source hygiene and fork policy notes.
- Avoided functional changes.

### Phase 0002: Source hygiene audit

- Audited line endings, BOMs, and text/binary classification.
- Documented source hygiene exceptions.
- Avoided accidental normalization churn.

### Phase 0003: Source hygiene fixes

- Fixed selected source hygiene defects.
- Left sensitive CRLF/BOM files untouched for dedicated later work.

### Phase 0004: Build noise audit

- Documented non-fatal build output.
- Classified warnings and cleanup candidates before changing code.

### Phase 0005: Quiet clean scripts

- Reduced cosmetic `clean_all` noise.
- Kept build behavior unchanged.

### Phase 0006: Haiku add-on export macro

- Replaced ignored foreign `__declspec(dllexport)` usage at the add-on boundary.
- Kept the add-on ABI surface explicit and Haiku-clean.

### Phase 0007: Add-on warning flag alignment

- Aligned add-on compiler warning handling with the main build.
- Kept Actor source code unchanged.

### Phase 0008: Equaliser bind2nd replacement

- Replaced deprecated `std::bind2nd` usage in the Equaliser helper code.
- Preserved the original mathematical operation with equivalent lambdas.

### Phase 0009: Deterministic interference size

- Replaced warning suppression with an explicit GCC parameter:

      --param=destructive-interference-size=64

- Kept the existing Actor alignment design intact.

### Phase 0010: Build noise audit update

- Updated the build-noise audit after the completed fixes.
- Documented the remaining deferred source hygiene exception.

## Near-term direction

The next work should stay conservative and documentation-driven.

Possible next phases:

- document the current feature and capability map,
- identify existing add-on/plugin boundaries,
- document project file behavior and recovery risks,
- run runtime smoke tests on the clean baseline,
- inspect import/export paths before changing architecture.

## Long-term workstation direction

Possible long-term areas:

- proxy workflow,
- render queue,
- improved audio workflow,
- color and grading tools,
- better effect organization,
- documented plugin SDK,
- project recovery and diagnostics.

DaVinci Resolve may be used as a capability atlas, but BeVinci is not a clone
target. The goal is to grow Medo's Haiku-native foundation into a stronger,
better documented, more robust workstation-style editor.
