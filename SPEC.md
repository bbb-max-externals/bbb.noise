# SPEC.md — bbb.noise v1 Development Specification

Normative specification for implementation. PLAN.md is the design document; this is the build spec.

MUST = required. SHOULD = recommended. MAY = optional.

## 1. Scope

24 objects total (see PLAN.md §10.2). macOS build first, Windows later.

## 2. Architecture

```
┌──────────────────────────────────────────────────┐
│  source/bbb/  (static library: bbb_noise_eval)    │
│  Custom C++ noise library (1D–4D, all algorithms) │
├────────────┬────────────┬────────────────────────┤
│ Max wrappers│MSP wrappers│ Jitter wrappers         │
│ (min-api)  │(min-api    │ (raw Jitter C API)      │
│            │ mc_operator)│                         │
└────────────┴────────────┴────────────────────────┘
```

- **Custom noise library**: Pure C++17, no external dependencies. All algorithms (Perlin, Simplex, Value, Cellular) in all dimensions (1D–4D) implemented from scratch. Double precision throughout.
- Shared evaluator = C++17 static library, linked into every .mxo
- Max/MSP wrappers = min-api C++ (event-rate, sample/mc_operator)
- Jitter wrappers = raw Jitter C API (t_jit_object, jit_attr, jit_mop)
- Each .mxo is independent; no shared dynamic libraries at runtime

## 3. File Structure

```
bbb.noise/                          # Max external project
├── CMakeLists.txt
├── cmake/
│   └── bbb_external.cmake          # from max-external skill
├── deps/
│   ├── min-api/                    # git submodule (includes max-sdk-base)
│   └── bbb-noise/                  # git submodule (noise library)
│       └── bbb/
│           ├── noise.hpp           # all-include
│           └── noise/
│               ├── common.hpp      # PermutationTable, fade, lerp, hash, grad
│               ├── perlin.hpp      # Perlin 1D–4D
│               ├── simplex.hpp     # Simplex 1D–4D
│               ├── value.hpp       # Value 1D–4D
│               ├── cellular.hpp    # Cellular/Worley 1D–4D
│               ├── fractal.hpp     # fBM, Turbulence, Ridged
│               └── evaluator.hpp   # High-level Evaluator class
├── source/
│   └── projects/
│       ├── bbb.noise.perlin/
│       │   ├── bbb.noise.perlin.cpp
│       │   └── CMakeLists.txt
│       └── ... (same pattern × 24)
├── externals/                      # build output
├── help/                           # .maxhelp files
├── package-info.json
├── PLAN.md
├── SPEC.md
└── TODO.md
```

## 4. Noise Library API (bbb::noise)

The noise library lives in `deps/bbb-noise/` as a header-only C++ library.
Namespace: `bbb::noise`. Include: `#include <bbb/noise.hpp>`.

### 4.1 Types

```cpp
namespace bbb::noise {

enum class type : int {
    perlin = 0,
    simplex,
    value,
    cellular,
};

enum class cellular_mode : int {
    f1 = 0,
    f2,
    f2_minus_f1,
    id,
};

enum class fractal_type : int {
    none = 0,
    fbm,
    turbulence,
    ridged,
};

struct config {
    type noise_type        = type::perlin;
    int seed               = 0;
    cellular_mode cell_mode = cellular_mode::f1;

    // Fractal
    fractal_type fractal   = fractal_type::none;
    type source            = type::simplex;   // @source for fractal
    int octaves            = 4;
    double gain            = 0.5;
    double lacunarity      = 2.0;
};

} // namespace bbb::noise
```

### 4.2 Evaluator class

```cpp
namespace bbb::noise {

class evaluator {
public:
    evaluator() = default;
    explicit evaluator(const config &cfg);

    void configure(const config &cfg);
    const config &get_config() const;

    // Evaluate noise. Returns raw value in approximately [-1, 1].
    // For cellular modes, range varies (see §4.4).
    // Coordinate transform (scale/offset) is NOT applied here —
    // callers apply it before passing coords.
    double eval(double x) const;
    double eval(double x, double y) const;
    double eval(double x, double y, double z) const;
    double eval(double x, double y, double z, double w) const;

private:
    config config_;
    detail::permutation_table perm_;
    detail::permutation_table source_perm_;  // for fractal source
};

} // namespace bbb::noise
```

### 4.3 Detail namespace

`bbb::noise::detail` contains internal implementation:

- `permutation_table` — seeded 256-entry table, doubled for wrapping
- `fade(t)` — improved Perlin fade: 6t⁵ - 15t⁴ + 10t³
- `lerp(a, b, t)` — linear interpolation
- `grad(hash, x, ...)` — gradient dot products per dimension
- Individual noise functions: `perlin(perm, x, y)`, `simplex(perm, x, y, z)`, etc.

All noise functions take `const permutation_table &` as first arg.
All return `double`. All use `double` internally for coordinate math.

### 4.4 Output ranges

| Algorithm | Raw output range |
|-----------|-----------------|
| Perlin | approximately [-1, 1] |
| Simplex | approximately [-1, 1] |
| Value | approximately [-1, 1] |
| Cellular F1 | [0, ~1.5] (distance) |
| Cellular F2 | [0, ~2.0] |
| Cellular F2-F1 | [0, ~1.0] |
| Cellular ID | [0, 1] (hash-based) |
| Fractal FBm | approximately [-1, 1] |
| Fractal Turbulence | [0, ~1] |
| Fractal Ridged | [-1, 1] |

Caller (Max/MSP/Jitter wrapper) is responsible for normalization to [0,1] or [-1,1] based on `@signed`.

### 4.5 Determinism

Same seed + same input MUST produce same output, always.
Permutation table initialization from seed MUST be deterministic.
No use of `rand()`, `std::mt19937`, or any non-seeded randomness.

## 5. Common Attributes

All attributes MUST use min-api `c74::min::attribute` for Max/MSP, `jit_attr_*` for Jitter.

### 5.1 Attribute table

| Attribute | Type | Default | Range | All objects |
|-----------|------|---------|-------|-------------|
| `input_dim` | int | 2 | 1–4 | Yes |
| `signed` | bool | 0 | 0/1 | Yes |
| `seed` | int | 0 | any int | Yes |
| `random_seed` | bool | 0 | 0/1 | Yes |
| `scale` | list | 1.0 | 1–4 floats | Yes |
| `offset` | list | 0.0 | 1–4 floats | Yes |
| `source` | symbol | simplex | perlin/simplex/value | Fractal only |
| `octaves` | int | 4 | 1–8 | Fractal only |
| `gain` | float | 0.5 | 0.0–1.0 | Fractal only |
| `lacunarity` | float | 2.0 | >0 | Fractal only |
| `cellular_mode` | enum | f1 | f1/f2/f2minusf1/id | Cellular only |
| `basis` | symbol | (per-algorithm) | perlin/simplex/value/cellular/fbm/turbulence/ridged | All-in-one only |
| `mc_mode` | enum | voices | voices/coords | MSP only |
| `dim` | list | 320 240 | int list | Jitter only |
| `type` | symbol | float32 | char/float32/float64 | Jitter only |
| `planecount` | int | 1 | 1–32 | Jitter only |
| `coord` | enum | norm | pixel/norm/centered | Jitter only |
| `time` | float | 0.0 | any float | Jitter only |
| `planeoffset` | float | 17.0 | any float | Jitter only |

### 5.2 @scale and @offset

MUST accept either a single float (broadcast to all axes) or a list of 1–4 floats.
If fewer values than `@input_dim`, remaining axes use last specified value.
If more values than `@input_dim`, extra values are stored but unused.

## 6. Max Event-Rate Object Spec

### 6.1 Base class: `bbb_noise_object`

All event-rate noise objects SHOULD inherit from a common base or use a shared mixin that provides:

- 4 proxy inlets (y, z, w)
- Stored coordinates: `double x, y, z, w`
- Common attribute declarations
- Evaluator instance

### 6.2 Inlets

| Inlet | Index | Type | Behavior |
|-------|-------|------|----------|
| 1 (left) | 0 | float/list/bang/messages | Primary inlet (see below) |
| 2 | 1 | float | Set y, no output |
| 3 | 2 | float | Set z, no output |
| 4 | 3 | float | Set w, no output |

### 6.3 Inlet 1 messages

| Message | Args | Behavior |
|---------|------|----------|
| (float) | x | Set x, output result |
| (list) | x y z w... | Set coords from list, output result |
| bang | — | Output using stored coords |
| x | float | Set x, no output |
| y | float | Set y, no output |
| z | float | Set z, no output |
| w | float | Set w, no output |
| coord | list | Set x/y/z/w from list, output |
| seed | int | Set seed |
| randomize | — | Randomize seed |

### 6.4 Output

Single outlet, outputs float (double precision from evaluator, cast to Max float atom).

### 6.5 Class naming

```
Directory/file: bbb.noise.perlin
C++ class:      bbb_noise_perlin
MIN_EXTERNAL:   bbb_noise_perlin
```

## 7. MSP Signal-Rate Object Spec

### 7.1 Base class: `bbb_noise_tilde_object`

All MSP noise objects MUST use `c74::min::mc_operator<>` as base (for MC support).

### 7.2 Inlets

| Inlet | Index | Type | Signal? |
|-------|-------|------|---------|
| 1 (left) | 0 | x signal + messages | Yes |
| 2 | 1 | y signal/float | Yes |
| 3 | 2 | z signal/float | Yes |
| 4 | 3 | w signal/float | Yes |

### 7.3 Processing

In the `operator()(audio_bundle& in, audio_bundle& out)` override:

For each sample frame `i`:
1. Read coordinate components from signal inputs
2. Apply coord * scale + offset
3. Evaluate noise
4. Normalize output (signed/unsigned)
5. Write to output

Float (non-signal) inputs to coordinate inlets MUST be treated as constant signals.

### 7.4 MC modes

**`@mc_mode voices` (default):**
- Each MC channel is an independent voice
- Coordinate signals are taken from corresponding channels
- If one inlet is mono and another MC, mono is broadcast
- Output channel count = max(input channel counts)

**`@mc_mode coords`:**
- Inlet 1's MC channels are interpreted as coordinate components
- `@input_dim 2` → ch1=x, ch2=y; `@input_dim 3` → ch1-3; `@input_dim 4` → ch1-4
- Missing channels use stored coordinate defaults
- Extra channels ignored
- Output is mono (single channel)

### 7.5 Class naming

```
Directory/file: bbb.noise.perlin~
C++ class:      bbb_noise_perlin_tilde
MIN_EXTERNAL:   bbb_noise_perlin_tilde
```

## 8. Jitter Object Spec

### 8.1 API approach

Jitter objects MUST use the raw Jitter C API:
- `jit_class_new()` for registration
- `jit_mop_new(0, 1)` for output-only MOP (no input matrices)
- `jit_attr_*` for attributes
- `matrix_calc` method for matrix generation

Jitter wrapper files are C++ but call Jitter C API directly. They include the shared evaluator via `bbb_noise_evaluator.hpp`.

### 8.2 Inlets

| Inlet | Index | Type | Behavior |
|-------|-------|------|----------|
| 1 (left) | 0 | bang/jit_matrix/messages | Trigger output |
| 2 | 1 | float | x override |
| 3 | 2 | float | y override |
| 4 | 3 | float | z override |
| 5 | 4 | float | w override |

### 8.3 Matrix generation

On `bang` or `jit_matrix` message:

1. Get/create output matrix with `@dim`, `@type`, `@planecount`
2. Lock matrix data
3. Iterate over all cells (i, j, k):
   a. Compute base coordinates from `@coord` mode (pixel/norm/centered)
   b. Apply coordinate overrides (persistent, from proxy inlets or x/y/z/w messages)
   c. Set w = @time (if not overridden)
   d. Apply coord * scale + offset
   e. For each plane p: evaluate noise at (x, y, z, w + p * @planeoffset)
   f. Normalize and store in matrix
4. Unlock and output matrix

### 8.4 Coordinate modes

| Mode | x | y | z | w |
|------|---|---|---|---|
| pixel | i | j | k | @time |
| norm | i/(W-1) | j/(H-1) | k/(D-1) | @time |
| centered | remap(i,0,W-1,-1,1) | remap(j,0,H-1,-1,1) | remap(k,0,D-1,-1,1) | @time |

Division by zero protection: MUST use `max(dim-1, 1)` for norm mode.

### 8.5 Output type behavior

| @type | @signed=0 | @signed=1 |
|-------|-----------|-----------|
| char | [0, 255] | [0, 255] (always unsigned in storage) |
| float32 | [0.0, 1.0] | [-1.0, 1.0] |
| float64 | [0.0, 1.0] | [-1.0, 1.0] |

For char output: `uint8_t(std::clamp(normalized * 255.0 + 0.5, 0.0, 255.0))`

### 8.6 Coordinate overrides

Overrides are persistent until cleared:

```max
clear_override x   -- reset x to grid-generated coordinate
clear_override y
clear_override z
clear_override w
clear_overrides    -- reset all to grid-generated
```

### 8.7 Class naming

```
Directory/file: jit.bbb.noise.perlin
C++ class:      jit_bbb_noise_perlin
```

Note: `jit.` prefix uses underscore in C++ class name, not `jit_bbb_noise_perlin` → `jit_bbb_noise_perlin`.
Registration uses `jit_class_new("jit.bbb.noise.perlin", ...)`.

## 9. All-in-One Object Spec

### 9.1 @basis dispatch

All-in-one objects (`bbb.noise`, `bbb.noise~`, `jit.bbb.noise`) use `@basis` to select the algorithm at runtime.

On `@basis` change:
1. Create/reconfigure the evaluator with the selected noise type
2. For fractal basis values (fbm, turbulence, ridged): enable fractal mode with appropriate FractalMode

### 9.2 @basis values and mapping

| @basis | NoiseType | fractal_enabled | FractalMode |
|--------|-----------|-----------------|-------------|
| perlin | Perlin | false | — |
| simplex | Simplex | false | — |
| value | Value | false | — |
| cellular | Cellular | false | — |
| fbm | (source) | true | FBm |
| turbulence | (source) | true | Turbulence |
| ridged | (source) | true | Ridged |

### 9.3 Additional attributes

All-in-one objects MUST expose `@basis` attribute.
All-in-one objects MUST also expose all algorithm-specific attributes (octaves, gain, lacunarity, cellular_mode, source).
Attributes that don't apply to the current @basis MUST be accepted but ignored.

## 10. Error Handling

- Invalid `@input_dim` values MUST be clamped to [1, 4].
- Invalid `@octaves` values MUST be clamped to [1, 8].
- Negative `@scale` or `@lacunarity` values MUST be accepted (they produce valid results).
- Invalid `@basis` values MUST fall back to the default (perlin for all-in-one).
- `cout` error messages for non-fatal issues (invalid attribute values).
- No crashes on any input combination.

## 11. Build Configuration

### 11.1 CMake structure

Root `CMakeLists.txt`:
- `cmake_minimum_required(VERSION 3.19)`
- Includes `cmake/bbb_external.cmake`
- `add_subdirectory(source/bbb)` — builds static library wrapping the header-only lib
- Auto-discovers `source/projects/*/` via SUBDIRLIST macro

`source/bbb/CMakeLists.txt`:
- Interface library pointing to `deps/bbb-noise/` headers
- `target_include_directories(bbb_noise_eval INTERFACE ${CMAKE_SOURCE_DIR}/deps)`
- C++17, no min-api dependency

Each project `CMakeLists.txt`:
- `bbb_add_external(DEPS bbb_noise_eval)`

### 11.2 Noise library dependency

Header-only library at `deps/bbb-noise/` (git submodule: `git@github.com:2bbb/bbb-noise.git`).
Include path: `#include <bbb/noise.hpp>` (include dir = `deps/`).
No compiled components — all `inline` in headers.

### 11.3 Jitter SDK headers

Jitter headers (`jit.common.h`, `jit.lib.h`, etc.) come from the local Max installation.
`bbb_add_external()` already links against `JitterAPI` framework (macOS) / `jitlib.lib` (Windows).
Header search paths MUST include the Jitter SDK location.

On macOS: Headers are in `JitterAPI.framework/Headers`, accessible via framework import.
`#include <JitterAPI/JitterAPI.h>` or equivalent.

### 11.4 Platform targets

| Platform | Output | Architecture |
|----------|--------|-------------|
| macOS | .mxo (bundle) | Universal: x86_64 + arm64 |
| Windows | .mxe64 (DLL) | x64 |

### 11.5 Build commands

```bash
# macOS
mkdir -p build && cd build
cmake ..
cmake --build .

# Windows (from developer command prompt)
mkdir build && cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

## 12. Implementation Order

See TODO.md for detailed task breakdown. Phases:

1. **Foundation** — CMake, deps, noise library, evaluator wrapper, bbb.noise.perlin (proof of concept)
2. **Basic Max** — simplex, value, cellular (event-rate)
3. **Basic MSP** — perlin~, simplex~, value~, cellular~ (signal-rate + MC)
4. **Basic Jitter** — jit.bbb.noise.{perlin, simplex, value, cellular}
5. **Fractal Max** — fbm, turbulence, ridged (event-rate)
6. **Fractal MSP** — fbm~, turbulence~, ridged~ (signal-rate + MC)
7. **Fractal Jitter** — jit.bbb.noise.{fbm, turbulence, ridged}
8. **All-in-one** — bbb.noise, bbb.noise~, jit.bbb.noise
9. **Help patches** — .maxhelp for all 24 objects
10. **Polish** — testing, edge cases, documentation
