# Add-on Parameter Writer Interface Design

This document defines the intended migration direction for effect add-on
parameter serialization.

It does not implement the interface yet.

## Current state

Effect parameters are currently serialized through:

```cpp
virtual bool SaveParameters(FILE *file, MediaEffect *media_effect) = 0;
````

This keeps every add-on responsible for manual JSON formatting.

## Target direction

Add a writer-based interface in parallel:

```cpp
virtual bool SaveParameters(BeJsonWriter &writer, MediaEffect *media_effect);
```

The caller opens the `parameters` JSON object.

Each add-on writes only its own parameter keys and values.

The add-on must not write:

* opening or closing braces
* commas between sibling objects
* indentation
* raw JSON strings
* direct `FILE*` output

## Compatibility rule

The existing `FILE*` interface remains during migration.

The writer-based interface must be introduced without breaking existing add-ons.

## Migration order

1. Fade
2. Wipe
3. Equaliser
4. IIRFilter
5. ChromaKey

## First implementation candidate

Fade is the first candidate because its parameter payload contains only:

* `direction`: unsigned integer

Expected writer form:

```cpp
writer.Key("direction");
writer.Uint(data->direction);
```

## Validation rule

Each migrated add-on must be validated against its existing `LoadParameters()`
implementation.

A migration is only accepted when:

* the project builds
* the add-on payload parses as JSON
* the existing reader accepts the generated payload
* the old `FILE*` path is still available during transition

## Design boundary

Do not combine this interface work with unrelated project JSON cleanup.

This is an add-on contract migration.
