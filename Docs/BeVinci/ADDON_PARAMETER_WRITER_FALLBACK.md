# Add-on Parameter Writer Fallback

This document records the active transition behavior for effect add-on parameter
serialization.

## Current behavior

`Editor/Project_Json.cpp` now uses a two-step save path for
`effects.parameters`.

For effects with parameter data:

1. Create a `BeJsonWriter`.
2. Open a JSON object.
3. Ask the effect node to write parameters through the writer interface.
4. If the writer interface returns `true`, write the generated object.
5. If it returns `false`, fall back to the legacy `FILE*` interface.

Conceptually:

```cpp
BeJsonWriter parameter_writer;
parameter_writer.StartObject();

const bool wrote_parameters =
    effect_node->SaveParameters(parameter_writer, media_effect);

parameter_writer.EndObject();

if (wrote_parameters)
    write parameter_writer.Data();
else
    effect_node->SaveParameters(file, media_effect);
````

## Active migration state

The following add-on currently implements the writer-based parameter path:

* Fade

The following add-ons still use the fallback path:

* ChromaKey
* Equaliser
* IIRFilter
* Wipe

## Compatibility rule

The legacy `FILE*` path remains the compatibility path until all bundled add-ons
have migrated.

An add-on participates in the new writer path only when its writer overload
returns `true`.

## Add-on writer contract

When implementing:

```cpp
SaveParameters(BeJsonWriter &writer, MediaEffect *media_effect)
```

the add-on must assume that the caller has already opened the `parameters`
object.

The add-on must write only its own parameter keys and values.

The add-on must not write:

* `{`
* `}`
* commas
* indentation
* raw JSON text
* direct `FILE*` output

## Current first migrated add-on

Fade writes:

```cpp
writer.Key("direction");
writer.Uint(data->direction);
```

and returns `true`.

## Purpose

This fallback design allows BeVinci to migrate one add-on at a time without
breaking project saving for add-ons that still use the legacy parameter
serializer.

## Rule

Do not remove the fallback path until every bundled add-on has a validated
writer-based serializer.

