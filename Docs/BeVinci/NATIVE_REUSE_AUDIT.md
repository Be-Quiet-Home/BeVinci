# BeVinci Native and Reuse Audit

This audit records the first review of native Haiku usage, existing dependency
usage, and hand-rolled mechanisms.

## Scope

This audit is not a warning cleanup pass.

The goal is to classify mechanisms before hardening them.

Before cleaning code, BeVinci must ask:

    Is this the right mechanism?

## Policy

Preferred order:

1. Native Haiku APIs and Kits.
2. Existing project dependencies.
3. Small proven libraries with clear provenance.
4. Custom code only when justified.

Rejected by default:

- mega-frameworks,
- hidden runtimes,
- foreign platform abstractions,
- imported UI/toolkit worldviews,
- large dependency stacks for small local problems.

Every byte must justify itself.

## JSON

### Finding

RapidJSON is already present in the codebase.

It is used on the read/parse side in areas such as:

    Editor/EffectsManager_Plugin.cpp
    Editor/LanguageJson.cpp
    Editor/Project_Json.cpp
    Editor/SettingsWindow.cpp

Effect and add-on loading also uses:

    LoadParameters(const rapidjson::Value&, MediaEffect*)

This is a positive finding. BeVinci does not need a second JSON library unless a
strong reason is found.

### Concern

The write/save side still uses a hand-written stream interface:

    SaveParameters(FILE *file, MediaEffect *media_effect)

This interface appears across many effects and add-ons.

Project and settings writing also use repeated C-style output patterns such as:

    sprintf(...)
    fwrite(buffer, strlen(buffer), 1, file)

This is a risk for JSON correctness, especially for strings such as:

    filenames
    labels
    tags
    notes
    vendor names
    effect names
    text effect content

These values require correct JSON escaping.

### Classification

    Editor/Project_Json.cpp:
        replace/harden

    EffectNode::SaveParameters(FILE*, ...):
        replace/harden

    Effects/* SaveParameters:
        harden/standardize

    AddOns/* SaveParameters:
        harden/standardize

    SettingsWindow JSON:
        inspect/harden

### Preferred direction

Do not add another JSON dependency by default.

First investigate whether existing RapidJSON usage can be extended to the writer
side.

The long-term direction should be a structured JSON save path, not prettier
hand-written JSON text.

## Storage and filesystem access

### Finding

Haiku storage APIs are used broadly, including:

    BPath
    BEntry
    BDirectory
    BFilePanel
    BNode

This is positive. The codebase is not just using POSIX file paths everywhere.

### Concern

C/POSIX file APIs are still present in important save/load paths:

    fopen
    fread
    fwrite
    FILE*

This is not automatically wrong. It becomes a review target when it overlaps
with project files, settings files, or format generation.

### Classification

    BPath / BEntry / BDirectory usage:
        keep

    BFilePanel integration:
        keep

    Project JSON fwrite path:
        replace/harden

    Settings JSON fwrite path:
        inspect/harden

    FileUtility raw file loading:
        inspect

## Threading and messaging

### Finding

Haiku messaging is used widely:

    BMessage
    BMessenger
    PostMessage
    MessageReceived

This is positive and consistent with Haiku application structure.

### Concern

BeVinci also contains a custom Yarra Actor / WorkThread layer.

This is not automatically wrong. Rendering, preview generation, audio thumbnail
generation, and export work may require background execution.

However, the actor layer must be treated as a system integration mechanism, not
as a replacement for Haiku's messaging model.

### Classification

    UI BMessage / MessageReceived paths:
        keep

    RenderActor:
        inspect/harden

    AudioThumbnailActor:
        inspect/harden

    PreviewActor:
        inspect/harden

    ExportMedia_MediaKit WorkThread:
        harden/replace-with-actor candidate

    Export_ffmpeg WorkThread:
        quarantine/harden

    Yarra Actor / WorkThread core:
        inspect deeply

## External and borrowed code

### Finding

Some subsystems appear to carry larger borrowed or foreign-origin code areas.

Initial examples:

    AddOns/IIRFilter/fxobjects.*
    Yarra/*
    ffmpeg export path

These areas may be useful and justified, but they must not become unquestioned
architectural foundation.

### Classification

    IIRFilter/fxobjects:
        quarantine/provenance

    Yarra rendering/actor support:
        inspect/justify

    ffmpeg export:
        quarantine/harden

    MediaKit export:
        prefer where functionally sufficient

## Initial subsystem table

| Subsystem | Initial classification | Reason |
| --- | --- | --- |
| Project JSON save | replace/harden | RapidJSON exists, writer path is hand-built |
| Project JSON load | keep/harden | Uses RapidJSON |
| Settings JSON | inspect/harden | Small but hand-written output |
| Effect SaveParameters | harden/standardize | FILE-based JSON emission |
| Effect LoadParameters | keep/harden | RapidJSON-based |
| Haiku storage API use | keep | Native APIs are present |
| Raw FILE* project output | replace/harden | Format safety risk |
| UI messaging | keep | Haiku-native |
| RenderActor | inspect/harden | May be justified for rendering |
| Export MediaKit | harden | Native export path, but threading needs review |
| Export ffmpeg | quarantine/harden | Foreign dependency and export complexity |
| IIRFilter/fxobjects | quarantine/provenance | Large DSP code with warning surface |

## Next step

Do not fix warnings blindly.

The next technical phase should focus on the project JSON save path and answer:

    Can BeVinci extend its existing RapidJSON use to structured writing?

If yes, do that before polishing hand-written JSON output.

If no, document why the hand-written writer remains and harden it explicitly.
