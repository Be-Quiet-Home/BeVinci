# BeVinci Project JSON Save Path Audit

This audit reviews whether the project JSON save path can move away from
hand-written `sprintf` / `fwrite` output and toward structured RapidJSON writing.

## Scope

Branch:

    phase-0014-project-json-save-path

This phase does not change production code.

The goal is to determine whether BeVinci already has the right JSON component
available and how invasive a safe migration would be.

## Findings

### RapidJSON writer support is available

RapidJSON writer headers are present in the source tree:

    rapidjson/writer.h
    rapidjson/prettywriter.h
    rapidjson/stringbuffer.h

A syntax-only compiler probe using:

    rapidjson::StringBuffer
    rapidjson::Writer<rapidjson::StringBuffer>

compiled successfully with the current include setup.

This means BeVinci does not need a second JSON library for structured JSON
writing.

### RapidJSON reader usage already exists

The codebase already uses RapidJSON on the read/parse side, including:

    Editor/EffectsManager_Plugin.cpp
    Editor/LanguageJson.cpp
    Editor/Project_Json.cpp
    Editor/SettingsWindow.cpp

Effect and add-on loading uses the existing interface:

    LoadParameters(const rapidjson::Value&, MediaEffect*)

This is a strong reason to extend existing RapidJSON use rather than introduce
another JSON dependency.

### Writer usage is not currently used

The scan did not find existing production use of:

    rapidjson::Writer
    rapidjson::PrettyWriter
    rapidjson::StringBuffer

The writer side is therefore available but not yet integrated.

## Current save path

The project save path in:

    Editor/Project_Json.cpp

uses repeated C-style output patterns:

    sprintf(...)
    fwrite(buffer, strlen(buffer), 1, file)

String-sensitive fields include:

    source labels
    source filenames
    clip tags
    effect vendor names
    effect names
    note text
    text effect content
    add-on parameters

These require correct JSON escaping.

Manual string replacement such as newline replacement is not sufficient as a
general JSON string escaping strategy.

## Cross-cutting interface issue

The current effect save interface is:

    SaveParameters(FILE *file, MediaEffect *media_effect)

This interface is declared in:

    Editor/EffectNode.h

and implemented across many effects and add-ons.

This makes an immediate one-shot replacement risky. The problem is not only in
`Editor/Project_Json.cpp`; the FILE-based writer interface is part of the effect
serialization contract.

## Classification

    RapidJSON writer availability:
        keep/use

    Adding another JSON dependency:
        reject by default

    Project_Json.cpp hand-written output:
        replace/harden

    EffectNode::SaveParameters(FILE*, ...):
        replace/harden

    Effects/* SaveParameters:
        harden/standardize

    AddOns/* SaveParameters:
        harden/standardize

## Preferred migration direction

Do not polish the hand-written JSON writer as the long-term solution.

Preferred direction:

1. Keep the existing RapidJSON dependency.
2. Introduce a small BeVinci JSON writer adapter.
3. Move project-level JSON writing to structured writer calls first.
4. Keep the old FILE-based SaveParameters path temporarily if needed.
5. Add a new structured parameter writer path beside the old one.
6. Migrate effects and add-ons gradually.
7. Remove the old FILE-based parameter writer only after all users are migrated.

## Possible adapter shape

A minimal adapter should hide RapidJSON details from effect implementations while
still producing valid JSON.

Possible direction:

    class BeJsonWriter
    {
    public:
        void StartObject();
        void EndObject();
        void StartArray();
        void EndArray();

        void Key(const char *name);
        void String(const char *value);
        void Bool(bool value);
        void Int(int value);
        void Uint(unsigned int value);
        void Int64(int64 value);
        void Double(double value);
    };

This keeps effect code small and avoids spreading RapidJSON template details
through all effect classes.

## Compatibility strategy

The migration should preserve the existing project file structure unless there is
a documented reason to change it.

The first safe step should be output-equivalent JSON generation, not a project
format redesign.

Recommended checks for later implementation:

- save a small project with old writer,
- save equivalent project with new writer,
- parse both with RapidJSON,
- compare semantic structure, not whitespace,
- test string escaping with quotes, backslashes, newlines, and non-ASCII text.

## Recommended next technical phase

The next phase should not touch all effects.

Recommended next phase:

    phase-0015-json-writer-adapter-prototype

Goal:

    Add a tiny internal JSON writer adapter and prove that Project_Json.cpp can
    use it for a small, isolated part of the project save output.

Suggested first target:

    medo header
    resolution block
    session block

Avoid effect parameters in the first production patch.

Effect SaveParameters migration should be a later phase after the adapter has
proved itself.
