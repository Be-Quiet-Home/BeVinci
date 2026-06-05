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

The main application binary is currently created as:

    Medo

The original binary name may be renamed later as part of the BeVinci fork
transition.

## Baseline

The current BeVinci baseline is based on:

    Medo R1.0.0-Beta1-5.1
    + minimal FreeType pointer conversion fix

The newer upstream ffmpeg/libavformat rewrite is intentionally not used as the
foundation for BeVinci.

## Notes

BeVinci keeps the original Medo build structure for now.

The first goal is to establish a stable, reproducible Haiku R1/beta5 build
baseline before introducing larger architectural changes.
