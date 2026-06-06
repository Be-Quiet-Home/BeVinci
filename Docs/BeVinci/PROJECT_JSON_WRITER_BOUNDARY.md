# Project JSON Writer Boundary

BeVinci now uses `BeJsonWriter` for the project JSON fields owned directly by
`Editor/Project_Json.cpp`.

## Migrated project fields

The following top-level save blocks are generated through `BeJsonWriter`:

- `medo`
- `resolution`
- `sources`
- `clips`
- `notes`
- `tracks`
- `session`

The `effects` block has been partially hardened:

- `vendor` is written through `BeJsonWriter`
- `name` is written through `BeJsonWriter`

These two fields were the immediate string-escaping risk inside the effect
envelope.

## Current boundary

The effect parameter payload is still written through the legacy add-on
interface:

```cpp
EffectNode::SaveParameters(FILE *file, MediaEffect *media_effect)
````

This is an intentional boundary.

The parameter payload is not owned only by `Editor/Project_Json.cpp`. It is
provided by effect add-ons:

* ChromaKey
* Equaliser
* Fade
* IIRFilter
* Wipe

Changing this path is an add-on interface migration, not a local project JSON
cleanup.

## Rule

Do not convert `effects.parameters` opportunistically.

A future migration should introduce an explicit writer-based parameter interface,
for example:

```cpp
SaveParameters(BeJsonWriter &writer, MediaEffect *media_effect)
```

or an equivalent adapter layer.

The old `FILE *` path should remain until each add-on has been migrated and
validated separately.

## Reason

The project JSON writer migration was intended to remove local manual JSON
formatting and string escaping risks in `Project_Json.cpp`.

The add-on parameter path is a separate contract. Treat it as a new phase, not
as leftover cleanup.
