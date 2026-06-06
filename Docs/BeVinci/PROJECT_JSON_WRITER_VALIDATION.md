# BeVinci Project JSON Writer Validation

This note records the validation checkpoint after the first production use of
`BeJsonWriter` in `Editor/Project_Json.cpp`.

## Scope

The first production use only replaces the `medo` project header block.

No effect parameter serialization was changed.

No add-on serialization was changed.

No project file format redesign was intended.

## Required validation

Before converting more save blocks, BeVinci must verify that project files saved
after this change still parse correctly.

Minimum validation:

1. Start BeVinci.
2. Create or open a minimal project.
3. Save the project as a `.medo` file.
4. Reopen the saved project.
5. Confirm that the project header is accepted.
6. Confirm that RapidJSON parses the file without errors.
7. Confirm that whitespace differences do not affect loading.

## Reason

The build proves that the writer adapter compiles and links.

It does not prove that the produced project file is accepted by the existing
loader.

## Validation result

The first embedded writer output was tested with RapidJSON parsing.

The original `PrettyWriter` based adapter produced valid JSON, but its indentation
was not suitable for embedding small generated objects inside the existing
hand-written project writer.

`BeJsonWriter` now uses RapidJSON `Writer` instead of `PrettyWriter`.

This keeps embedded blocks compact while still using RapidJSON for correct JSON
string escaping and value emission.

## Next migration rule

Only after successful validation should the next small save block be converted.

Recommended next target after validation:

    resolution

Do not convert:

    sources
    clips
    effects
    notes
    tracks

until primitive top-level blocks have been proven safe.
