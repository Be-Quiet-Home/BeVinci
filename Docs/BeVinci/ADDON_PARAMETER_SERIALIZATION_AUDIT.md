# Add-on Parameter Serialization Audit

This document records the current `EffectNode::SaveParameters(FILE*, ...)`
boundary after the project-owned JSON writer migration.

## Current interface

Effect parameter payloads are still written through:

```cpp
virtual bool SaveParameters(FILE *file, MediaEffect *media_effect) = 0;
````

This interface is declared in:

* `Editor/EffectNode.h`

and implemented by five bundled add-ons.

## Implementations

### ChromaKey

File:

* `AddOns/ChromaKey/Effect_ChromaKey.cpp`

Written fields:

* `enabled_0` ... `enabled_n`
* `threshold_0` ... `threshold_n`
* `smoothing_0` ... `smoothing_n`
* `colour_0` ... `colour_n`

Field types:

* bool
* float
* RGBA integer arrays

Notes:

* No string parameters.
* Uses indexed key names.
* Manages commas manually inside the loop.

### Equaliser

File:

* `AddOns/Equaliser/Effect_Equaliser.cpp`

Written fields:

* `gain`
* `filter`

Field types:

* float array
* integer

Notes:

* No string parameters.
* Writes an array manually.
* Manages comma placement manually.

### Fade

File:

* `AddOns/Fade/Effect_Fade.cpp`

Written fields:

* `direction`

Field types:

* unsigned integer

Notes:

* No string parameters.
* Very small parameter payload.

### IIRFilter

File:

* `AddOns/IIRFilter/Effect_IIRFilter.cpp`

Written fields:

* `filters`
* `algorithm`

Field types:

* float array
* unsigned integer

Notes:

* No string parameters.
* Similar shape to Equaliser.
* Manages array formatting manually.

### Wipe

File:

* `AddOns/Wipe/Effect_Wipe.cpp`

Written fields:

* `direction`
* `swap`

Field types:

* unsigned integer
* bool

Notes:

* No string parameters.
* Small parameter payload.

## Risk classification

The current add-on parameter path is not primarily a string escaping risk.

Current risks are:

* manual JSON formatting
* manual comma placement
* repeated `sprintf` / `fwrite` patterns
* fixed local buffers
* `FILE*` as serialization contract
* no shared writer abstraction for effect parameters

## Boundary

The project-owned JSON fields in `Editor/Project_Json.cpp` have been migrated or
hardened with `BeJsonWriter`.

The add-on parameter payload remains intentionally outside that migration.

This boundary is deliberate because `effects.parameters` is not only a local
`Project_Json.cpp` concern. It is an add-on interface contract.

## Recommended migration direction

Do not replace `SaveParameters(FILE*, ...)` directly in one large step.

Preferred path:

1. Add a writer-based parameter interface in parallel.
2. Keep the existing `FILE*` path during transition.
3. Migrate one add-on at a time.
4. Validate each add-on payload against the existing `LoadParameters()` reader.
5. Remove or deprecate the `FILE*` path only after all bundled add-ons are
   migrated.

Possible future interface:

```cpp
virtual bool SaveParameters(BeJsonWriter &writer, MediaEffect *media_effect);
```

The writer should receive an already-open JSON object for the parameter payload.
Each add-on should write only its own parameter keys and values.

## Suggested migration order

Recommended order from lowest to highest complexity:

1. Fade
2. Wipe
3. Equaliser
4. IIRFilter
5. ChromaKey

Reasoning:

* Fade has one unsigned integer field.
* Wipe has one unsigned integer and one bool.
* Equaliser and IIRFilter have numeric arrays.
* ChromaKey has repeated indexed groups and RGBA arrays.

## Rule

Treat add-on parameter serialization as a separate migration project.

Do not mix it with unrelated `Project_Json.cpp` cleanup.

