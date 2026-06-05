# Building BeVinci on Haiku

BeVinci targets the regular Haiku R1/beta5 development environment and the
corresponding HaikuPorts package set.

## Required packages

Install the development dependencies:

    pkgman update
    pkgman install git ffmpeg_devel freetype_devel cmake
    pkgman install cmd:jam

## Build

The primary build path is Jam:

    ./clean_all
    jam -j4
    ./build_addons
    ./setup_attributes

For baseline verification, the core build check is:

    ./clean_all
    jam -j4
    ./build_addons

The main application binary is currently created as:

    Medo

The original binary name may be renamed later as part of the BeVinci fork
transition.

The bundled add-ons currently built by `./build_addons` are:

    ChromaKey.so
    Equaliser.so
    Fade.so
    IIRFilter.so
    Wipe.so

## Baseline

The current BeVinci baseline is based on:

    Medo R1.0.0-Beta1-5.1
    + minimal FreeType pointer conversion fix
    + BeVinci baseline hardening through phase 0010

The current clean build baseline is tagged as:

    baseline-build-clean-0010

The newer upstream ffmpeg/libavformat rewrite is intentionally not used as the
foundation for BeVinci.

## Compiler policy

The build keeps the original Jam structure for now.

The Actor layer still uses:

    std::hardware_destructive_interference_size

To keep GCC's value deterministic for this build, BeVinci passes:

    --param=destructive-interference-size=64

This is preferred over suppressing the related warning because it fixes the
compiler parameter explicitly instead of hiding the diagnostic.

## Notes

BeVinci keeps the original Medo build structure for now.

The first goal was to establish a stable, reproducible Haiku R1/beta5 build
baseline before introducing larger architectural changes. That baseline is now
available as `baseline-build-clean-0010`.

Future build-system changes should remain small, phase-based, and verified by a
full clean build plus add-on build.
