# BeVinci Code and System Quality Audit

This audit records the first baseline code and system quality review after the
clean build baseline.

## Scope

Branch:

    phase-0012-code-quality-audit

Reference baseline:

    baseline-build-clean-0010

The goal was not to change code. The goal was to determine whether the current
Medo-derived BeVinci codebase is suitable as a project foundation, and where the
next hardening or replacement work should focus.

## Summary verdict

BeVinci is suitable as a project foundation under stewardship conditions.

It is not pedantic-clean.

It is not yet a fully audited Haiku-conformant architecture.

The codebase appears to be a functional, Haiku-native, historically grown
application. It builds, links, uses Haiku/BeOS APIs heavily, and has a visible
effect/add-on structure. It also contains legacy C/C++ hygiene issues and
hand-rolled mechanisms that need review before larger architectural expansion.

Current assessment:

    suitable with conditions

## Quality policy

Code quality in BeVinci is not limited to compiler warnings.

For this fork, quality means:

- use Haiku-native APIs where they are the natural system solution,
- use existing project dependencies before adding new ones,
- use small proven components only when they clearly replace risky hand-rolled
  code,
- avoid large frameworks, foreign platform abstractions, hidden runtimes, and
  monolithic dependency stacks,
- do not clean up code merely to preserve the wrong mechanism.

Every byte must justify itself.

## Dependency admission rules

A new dependency is acceptable only if it is:

- small enough for the problem it solves,
- technically focused,
- buildable on the target Haiku baseline,
- compatible with the project license,
- not tied to Windows, macOS, Linux desktop frameworks, Qt, GTK, Electron, Java,
  or another foreign platform worldview,
- not a mega-framework,
- not a hidden runtime platform,
- not a replacement for Haiku's own application model.

Preferred order:

1. Haiku Kits.
2. Existing project dependencies.
3. HaikuPorts-provided libraries already suitable for the local task.
4. Small C or C++ libraries with clear provenance.

Rejected by default:

- large cross-platform frameworks,
- foreign UI/toolkit layers,
- DLL/import/export compatibility models,
- plugin systems copied from other platforms,
- dependency stacks that solve more than the local problem.

## Build status

The clean build baseline is already documented and tagged as:

    baseline-build-clean-0010

The main executable and bundled add-ons build and link successfully on Haiku
R1/beta5.

## Haiku / Be API surface

The first API-surface scan showed strong use of Haiku/BeOS concepts, including:

    BMessage
    BView
    BWindow
    BLooper
    BHandler
    BMessenger
    BPath
    BEntry
    BDirectory
    BNode
    BTranslationUtils

This is a positive signal. BeVinci is not merely a foreign application with a
thin Haiku wrapper. The codebase visibly works with Haiku/BeOS application
concepts.

This does not yet prove that the APIs are always used correctly. Lifetime,
locking, threading, message ownership, and looper/window boundaries still need a
dedicated Haiku conformance audit.

## Pedantic probe

A representative probe was run with:

    -Wall -Wextra -Wpedantic -fsyntax-only

Selected files covered editor code, actor code, GUI code, effects, and add-ons.

Result:

    compiler errors: 0

This is important. The selected code does not collapse under a stricter syntax
probe.

However, the same probe produced substantial warning output. The codebase is not
pedantic-clean.

Observed warning classes include:

    -Wignored-qualifiers
    -Wunused-parameter
    -Wsign-compare
    -Wdeprecated-copy
    -Wpedantic
    -Wunused-variable
    -Wmissing-field-initializers
    -Wreorder
    -Wdelete-non-virtual-dtor
    -Wclass-memaccess
    -Wformat=

## Warning interpretation

Mostly legacy/interface noise:

- ignored qualifiers on return values,
- unused parameters in virtual hook-style interfaces,
- anonymous structs in math/vector support code.

Targeted review required:

- deleting abstract objects with non-virtual destructors,
- class memory clearing through C functions,
- initializer order mismatches,
- missing field initializers,
- format warnings,
- deprecated copy behavior.

Warnings should not be fixed blindly. First decide whether the affected code is
the right mechanism.

## Reuse / hand-rolled mechanism scan

A first reuse-oriented scan was run for JSON handling, Haiku storage APIs, and
possible hand-rolled system/POSIX/string logic.

The scan produced substantial review material:

    JSON-related code:                    622 lines
    Haiku storage / filesystem API:       635 lines
    possible hand-rolled system logic:    831 lines

These numbers do not mean all matches are defects. They mean these areas need
architectural review before large cleanup work.

## JSON handling assessment

Project JSON handling is a high-priority review area.

The codebase already uses RapidJSON in several parameter-loading paths, but
project saving appears to contain extensive hand-built JSON output through
`sprintf`, especially in:

    Editor/Project_Json.cpp

This is a quality risk because JSON string values must be escaped correctly.
Labels, filenames, tags, notes, vendor names, and effect names may contain
characters that need correct JSON encoding.

The preferred direction is not to make hand-written JSON prettier.

The preferred direction is:

1. Check whether existing RapidJSON usage can be extended.
2. Avoid adding a second JSON library unless there is a strong reason.
3. Keep project-file handling small and explicit.
4. Do not introduce a large serialization framework.
5. Ensure correct escaping and malformed-input behavior.

The immediate question is not:

    How do we make hand-written JSON warning-clean?

The immediate question is:

    Should hand-written project JSON remain at all?

## Native Haiku mechanism assessment

Some internal state or data exchange may be better represented through Haiku
mechanisms such as:

    BMessage
    BArchivable
    BPath
    BEntry
    BDirectory
    BFile
    BNode

This does not automatically mean that BeVinci project files should stop using
JSON. JSON can remain appropriate for an external, inspectable project format.

The distinction should be:

    internal Haiku application state:
        prefer native Haiku mechanisms where appropriate

    external project format:
        JSON is acceptable, but should be generated and parsed through a proven
        JSON component

    low-level file and path handling:
        prefer Haiku storage APIs unless POSIX-level access is clearly justified

## Replacement before cleanup rule

Before cleaning warning-heavy code, BeVinci must ask:

    Is this code the right mechanism?

Classify each subsystem as:

    keep:
        appropriate and system-conformant

    harden:
        appropriate mechanism, but implementation needs cleanup

    replace:
        hand-rolled mechanism should move to a native Haiku API or proven small
        library

    quarantine:
        foreign, oversized, unclear, or high-risk code kept temporarily but not
        used as architectural foundation

## Initial priority areas

High-priority review targets:

    Editor/Project_Json.cpp
    Editor/SettingsWindow.cpp
    Editor/ExportMediaWindow.cpp
    Editor/ExportMedia_ffmpeg.cpp
    Editor/MediaSource.cpp
    Effects/* SaveParameters / LoadParameters
    AddOns/* SaveParameters / LoadParameters
    AddOns/IIRFilter/fxobjects.*

Special attention:

- project file safety,
- JSON escaping,
- import/export robustness,
- ownership and lifetime,
- non-virtual destructor warnings,
- C string formatting,
- Haiku looper/window/message boundaries,
- provenance and size of external or borrowed code.

## Recommended next phases

### Phase 0013: Critical warning triage

Review and classify:

    -Wdelete-non-virtual-dtor
    -Wclass-memaccess
    -Wformat=
    -Wreorder
    -Wmissing-field-initializers
    -Wdeprecated-copy

### Phase 0014: Project file safety audit

Review JSON saving/loading, string escaping, malformed input behavior, and
backward compatibility.

Primary target:

    Editor/Project_Json.cpp

### Phase 0015: Ownership and lifetime audit

Review manual ownership, especially `new`, `delete`, `malloc`, and `free`.

### Phase 0016: Haiku API conformance audit

Review whether BLooper/BHandler/BMessenger/BWindow usage follows Haiku-native
lifetime, threading, locking, and messaging expectations.

### Phase 0017: Native/reuse replacement audit

Review major subsystems for replacement candidates before warning-cleaning them.

## Final note

The current BeVinci codebase is a working Altbau with load-bearing walls. It is
not a clean-room modern C++ codebase.

The correct strategy is stewardship:

    preserve what works,
    document what exists,
    replace what should not remain,
    harden what is worth keeping,
    avoid every unnecessary byte.
