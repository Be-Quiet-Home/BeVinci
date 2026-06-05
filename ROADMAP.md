# BeVinci Roadmap

BeVinci is a Haiku-native video editing workstation based on Medo.

The project follows a conservative fork strategy:

1. Preserve the original Medo foundation.
2. Establish a clean BeVinci source and build baseline.
3. Document current capabilities and extension points.
4. Harden the existing editor before adding large features.
5. Extend BeVinci step by step toward workstation-grade video editing.

## Phase 0001: Baseline

- Document upstream origin.
- Document the Haiku R1/beta5 build path.
- Establish source hygiene rules.
- Avoid functional changes.

## Phase 0002: Source hygiene

- Add line ending policy.
- Remove BOMs where appropriate.
- Normalize text files in a separate commit.
- Avoid mixing formatting cleanup with functional changes.

## Phase 0003: Capability map

- Document existing Medo/BeVinci features.
- Document add-on and plugin systems.
- Identify gaps compared to a modern video editing workstation.
- Use DaVinci Resolve only as a capability atlas, not as a clone target.

## Phase 0004: Hardening

- Build reproducibility.
- Runtime diagnostics.
- Project file safety.
- Import/export stability.
- Add-on/plugin boundary checks.

## Phase 0005: Workstation direction

Possible long-term areas:

- Proxy workflow.
- Render queue.
- Improved audio workflow.
- Color and grading tools.
- Better effect organization.
- Documented plugin SDK.
- Project recovery and diagnostics.
