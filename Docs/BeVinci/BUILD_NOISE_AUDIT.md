# BeVinci Build Noise Audit

This audit documents non-fatal build output observed during BeVinci baseline
hardening on Haiku R1/beta5.

## Build status

The current baseline builds successfully on Haiku R1/beta5 with the documented
Jam build path:

    ./clean_all
    jam -j4
    ./build_addons

The main executable links successfully as:

    Medo

The following add-ons also link successfully:

    ChromaKey.so
    Equaliser.so
    Fade.so
    IIRFilter.so
    Wipe.so

As of phase 0009, the full build and add-on build are free of the previously
documented non-fatal compiler warnings.

## Resolved build noise

### clean_all rm messages

Earlier clean scripts called `rm` for optional generated files without checking
whether those files existed first.

Observed examples included:

    rm: cannot remove 'CMakeCache.txt': No such file or directory
    rm: cannot remove 'cmake_install.cmake': No such file or directory
    rm: cannot remove 'Makefile': No such file or directory
    rm: cannot remove 'CMakeLists.txt.user': No such file or directory

Resolution:

    The clean scripts now use quiet removal for optional generated files.

Status:

    Resolved in phase 0005.

### std::hardware_destructive_interference_size warning

The Actor layer uses:

    std::hardware_destructive_interference_size

GCC warns that this value can vary between compiler versions or CPU tuning
options unless the value is made explicit.

Resolution:

    The Actor source code remains unchanged.
    The main Jamfile and all add-on Jamfiles now pass a deterministic GCC
    parameter:

        --param=destructive-interference-size=64

Rationale:

    This keeps the existing Actor alignment design intact while making GCC use a
    stable destructive interference size value for this build.

Status:

    Resolved in phase 0009.

### __declspec(dllexport) ignored

Several add-on headers previously declared exported factory functions using:

    __declspec(dllexport)

GCC on Haiku warned that this attribute was ignored.

Resolution:

    The foreign compiler attribute was replaced by a BeVinci add-on export macro
    that resolves cleanly for Haiku builds.

Status:

    Resolved in phase 0006.

### std::bind2nd deprecated

The Equaliser add-on used deprecated C++ helper code:

    std::bind2nd

Resolution:

    The two deprecated `std::bind2nd` calls in the Equaliser helper code were
    replaced with equivalent lambda expressions.

Status:

    Resolved in phase 0008.

## Deferred source hygiene note

The source hygiene audit still reports one UTF-8 BOM in:

    Editor/ExportMedia_ffmpeg.cpp

This file is currently CRLF-only plus BOM. It was intentionally not changed in
the source hygiene defect fix to avoid a large accidental line-ending
normalization diff.

Classification:

    Known source hygiene exception.

Possible later fix:

    Normalize this file in a dedicated commit that explicitly handles both BOM
    and CRLF policy.

Risk:

    Medium, because the file is large and currently sensitive to line-ending
    churn.
