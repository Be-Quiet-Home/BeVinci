# BeVinci Build Noise Audit

This audit documents non-fatal build output observed on the current BeVinci
baseline. It does not change source code or build scripts.

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

## Observed build noise

### clean_all rm messages

Several clean scripts call `rm` for optional generated files without checking
whether those files exist first.

Observed examples:

    rm: cannot remove 'CMakeCache.txt': No such file or directory
    rm: cannot remove 'cmake_install.cmake': No such file or directory
    rm: cannot remove 'Makefile': No such file or directory
    rm: cannot remove 'CMakeLists.txt.user': No such file or directory

Classification:

    Cosmetic build noise.

Possible later fix:

    Use `rm -f` for generated files that may or may not exist.

Risk:

    Low, but should be handled in a dedicated build-script cleanup phase.

### std::hardware_destructive_interference_size warning

The Actor layer uses:

    std::hardware_destructive_interference_size

GCC warns that this value can vary between compiler versions or CPU tuning
options.

Classification:

    Portability / ABI stability warning.

Possible later fixes:

    Define a BeVinci-owned cache line constant.
    Or pass a stable compiler parameter.
    Or deliberately suppress the warning if the current design is accepted.

Risk:

    Medium. This touches layout/alignment assumptions and should not be changed
    casually.

### __declspec(dllexport) ignored

Several add-on headers declare exported factory functions using:

    __declspec(dllexport)

GCC on Haiku warns that this attribute is ignored.

Classification:

    Cross-platform residue / harmless export annotation on Haiku.

Possible later fix:

    Replace with a BeVinci export macro that resolves cleanly on Haiku.

Risk:

    Low, but this touches the add-on ABI surface and should be handled in a
    dedicated add-on boundary cleanup.

### std::bind2nd deprecated

The Equaliser add-on uses code that triggers warnings for deprecated
`std::bind2nd`.

Classification:

    Legacy C++ warning in Equaliser DSP/helper code.

Possible later fix:

    Replace `std::bind2nd` with `std::bind`, a lambda, or an explicit helper.

Risk:

    Medium. This code appears DSP/math-related and should be covered by a build
    and runtime smoke test before and after changes.

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
