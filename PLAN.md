# bbb.noise Object Suite PLAN

## 1. Goal

`bbb.noise` is a Max/MSP/Jitter object suite for using common procedural noise functions with a simple, predictable API.

The project focuses on:

- Easy access to Perlin, Simplex, Value, Cellular/Worley, fBM, Turbulence, Ridged, Domain Warp, and experimental Curl noise.
- Consistent naming across Max event-rate, MSP signal-rate, and Jitter matrix objects.
- Double-precision coordinate input for Max objects.
- Signal-rate noise generation for MSP objects, including MC support.
- Matrix generation for Jitter objects with simpler ergonomics than `jit.bfg`.
- Deterministic seeding by default.
- Clear output range behavior for float and char outputs.

## 2. Naming

### 2.1 Namespace

All objects use the `bbb.noise` namespace.

### 2.2 Max event-rate objects

Primary objects:

```max
bbb.noise
bbb.noise.perlin
bbb.noise.simplex
bbb.noise.value
bbb.noise.cellular
bbb.noise.fbm
bbb.noise.turbulence
bbb.noise.ridged
bbb.noise.domainwarp
bbb.noise.curl
```

`bbb.noise` is the all-in-one object. It selects the noise basis using `@basis`.

Example:

```max
bbb.noise @basis simplex @input_dim 3
bbb.noise.perlin @input_dim 2
bbb.noise.fbm @basis simplex @octaves 5
```

### 2.3 MSP signal-rate objects

Primary signal-rate objects:

```max
bbb.noise~
bbb.noise.perlin~
bbb.noise.simplex~
bbb.noise.value~
bbb.noise.cellular~
bbb.noise.fbm~
bbb.noise.turbulence~
bbb.noise.ridged~
bbb.noise.domainwarp~
bbb.noise.curl~
```

`bbb.noise~` is the all-in-one signal-rate object. It selects the noise basis using `@basis`.

### 2.4 Jitter matrix objects

Primary Jitter objects:

```max
jit.bbb.noise
jit.bbb.noise.perlin
jit.bbb.noise.simplex
jit.bbb.noise.value
jit.bbb.noise.cellular
jit.bbb.noise.fbm
jit.bbb.noise.turbulence
jit.bbb.noise.ridged
jit.bbb.noise.domainwarp
jit.bbb.noise.curl
```

`jit.bbb.noise` is the all-in-one Jitter object. It selects the noise basis using `@basis`.

## 3. Input dimensionality

Input dimensionality is controlled by `@input_dim`.

Valid values:

```text
1
2
3
4
```

Meaning:

```text
@input_dim 1 -> noise(x)
@input_dim 2 -> noise(x, y)
@input_dim 3 -> noise(x, y, z)
@input_dim 4 -> noise(x, y, z, w)
```

Object names do not encode dimensionality. Avoid primary names such as:

```max
bbb.noise.perlin1
bbb.noise.perlin2
bbb.noise.simplex4
```

Optional aliases may be added later, but the primary API is `@input_dim` based.

Reasoning:

- Keeps the object namespace small.
- Avoids combinatorial growth across algorithms, signal-rate versions, and Jitter versions.
- Separates the algorithm name from the function arity.
- Avoids conflict with Jitter `@dim`, which means matrix dimensions.

## 4. Common attributes

These attributes should be shared across Max, MSP, and Jitter objects where applicable.

```max
@input_dim 1..4
@signed 0/1
@seed <int>
@random_seed 0/1
@scale <float or list[1-4]>
@offset <float or list[1-4]>
@basis <symbol>
```

### 4.1 `@signed`

```max
@signed 0
```

Output range is unipolar.

For Max/MSP float outputs:

```text
[0.0, 1.0]
```

```max
@signed 1
```

Output range is bipolar.

For Max/MSP float outputs:

```text
[-1.0, 1.0]
```

For Jitter `char` output, output storage is always:

```text
[0, 255]
```

`@signed` affects numeric float matrix output, not the stored range of `char` matrices.

### 4.2 `@seed`

```max
@seed 0
```

Default seed is `0`.

Objects are deterministic by default. Reopening the same patch with the same seed should produce the same output.

Supported messages:

```max
seed 12345
randomize
```

### 4.3 `@random_seed`

```max
@random_seed 0
```

Default is deterministic seed behavior.

```max
@random_seed 1
```

Object initializes with a random seed on creation or load.

### 4.4 `@scale`

`@scale` accepts either a scalar or a list of 1-4 values.

Examples:

```max
@scale 0.01
@scale 4. 2.
@scale 1. 1. 1. 0.1
```

Scalar `@scale` applies to all input axes.

### 4.5 `@offset`

`@offset` accepts either a scalar or a list of 1-4 values.

Examples:

```max
@offset 0.
@offset 0.5 0.
@offset 0. 0. 1.25 0.
```

Scalar `@offset` applies to all input axes.

### 4.6 Coordinate transform order

The evaluated coordinate is:

```text
p = coord * scale + offset
```

This is used consistently across Max, MSP, and Jitter objects.

## 5. Coordinate input model

### 5.1 Max event-rate objects

All event-rate Max noise objects expose four coordinate inlets:

```text
inlet 1: x / primary inlet
inlet 2: y
inlet 3: z
inlet 4: w
```

All coordinate inputs are double-precision values internally.

`@input_dim` controls how many coordinates are evaluated. Unused coordinates are accepted and stored, but ignored during evaluation.

#### Inlet 1 behavior

```text
float:
  set x and output

list:
  set x/y/z/w from list and output

bang:
  output using stored x/y/z/w

x <float>:
  set x, no output

y <float>:
  set y, no output

z <float>:
  set z, no output

w <float>:
  set w, no output

coord <list>:
  set x/y/z/w from list and output

seed <int>:
  set seed

randomize:
  randomize seed
```

#### Inlets 2-4 behavior

```text
inlet 2: set y, no output
inlet 3: set z, no output
inlet 4: set w, no output
```

### 5.2 MSP signal-rate objects

All signal-rate objects expose four coordinate inlets:

```text
inlet 1: x signal / primary inlet / messages
inlet 2: y signal or float
inlet 3: z signal or float
inlet 4: w signal or float
```

`@input_dim` controls how many coordinates are evaluated.

For `@input_dim 2`:

```text
out[n] = noise(x[n], y[n])
```

For `@input_dim 4`:

```text
out[n] = noise(x[n], y[n], z[n], w[n])
```

Float inputs to signal inlets are treated as constant signals.

## 6. MC signal handling

MSP signal-rate objects support MC signals through `@mc_mode`.

```max
@mc_mode voices
@mc_mode coords
```

### 6.1 `@mc_mode voices`

Default mode.

Each MC channel is treated as an independent voice. Coordinate components are taken from corresponding channels of the coordinate signal inlets.

For `@input_dim 2`:

```text
out[ch] = noise(x_inlet[ch], y_inlet[ch])
```

For `@input_dim 4`:

```text
out[ch] = noise(x_inlet[ch], y_inlet[ch], z_inlet[ch], w_inlet[ch])
```

If one coordinate inlet is mono and another is MC, the mono signal is broadcast to all MC channels.

### 6.2 `@mc_mode coords`

The MC channels of inlet 1 are interpreted as coordinate components.

For `@input_dim 4`:

```text
inlet 1 ch1 = x
inlet 1 ch2 = y
inlet 1 ch3 = z
inlet 1 ch4 = w
```

Evaluation:

```text
out = noise(x, y, z, w)
```

For `@input_dim 2`, only channels 1 and 2 are used. For `@input_dim 3`, only channels 1-3 are used.

If inlet 1 has fewer channels than `@input_dim`, missing components use stored coordinate defaults. Extra channels are ignored.

The output of `@mc_mode coords` is mono, because the MC channels are interpreted as a single coordinate vector.

Inlets 2-4 are ignored in `@mc_mode coords`.

## 7. Jitter matrix objects

### 7.1 Inlets

Jitter objects expose five inlets:

```text
inlet 1: primary inlet for bang, jit_matrix, and messages
inlet 2: x constant override
inlet 3: y constant override
inlet 4: z constant override
inlet 5: w constant override
```

Generated grid coordinates are used unless a coordinate component is explicitly set through its proxy inlet or through `x`, `y`, `z`, or `w` messages.

Setting `x` or `y` explicitly overrides the generated grid coordinate for that axis. This is useful for slices through higher-dimensional fields, but most 2D texture animation should normally animate `z` or `w`.

### 7.2 Matrix attributes

Jitter objects support standard Jitter-style matrix attributes:

```max
@dim <int...>
@type char/float32/float64
@planecount <int>
```

Supported output types:

```text
char
float32
float64
```

### 7.3 Output type behavior

For `float32` and `float64`:

```text
@signed 0 -> [0.0, 1.0]
@signed 1 -> [-1.0, 1.0]
```

For `char`:

```text
output is always [0, 255]
```

### 7.4 Coordinate modes

Jitter objects support:

```max
@coord pixel
@coord norm
@coord centered
```

#### `@coord pixel`

```text
x = i
y = j
z = k
w = time
```

#### `@coord norm`

```text
x = i / max(width - 1, 1)
y = j / max(height - 1, 1)
z = k / max(depth - 1, 1)
w = time
```

#### `@coord centered`

```text
x = remap(i, 0..width-1, -1..1)
y = remap(j, 0..height-1, -1..1)
z = remap(k, 0..depth-1, -1..1)
w = time
```

Final evaluated coordinate:

```text
p = coord * scale + offset
```

### 7.5 Time

Jitter objects support a `@time` attribute.

```max
@time 0.
```

`@time` is commonly used as the `w` coordinate for animated 2D or 3D noise.

### 7.6 Plane offset

Jitter objects support:

```max
@planeoffset 17.
```

Plane offset is used to decorrelate multiple output planes.

For a 4D noise evaluation:

```text
plane 0: noise(x, y, z, w)
plane 1: noise(x, y, z, w + planeoffset)
plane 2: noise(x, y, z, w + planeoffset * 2)
plane 3: noise(x, y, z, w + planeoffset * 3)
```

## 8. Noise algorithms

### 8.1 Perlin noise

Objects:

```max
bbb.noise.perlin
bbb.noise.perlin~
jit.bbb.noise.perlin
```

Use cases:

- Smooth procedural modulation
- Natural low-frequency variation
- Terrain-like patterns
- Classic gradient noise workflows

### 8.2 Simplex noise

Objects:

```max
bbb.noise.simplex
bbb.noise.simplex~
jit.bbb.noise.simplex
```

Use cases:

- Smooth multidimensional noise
- Better behavior than classic Perlin in higher dimensions
- Animation and procedural texture generation

### 8.3 Value noise

Objects:

```max
bbb.noise.value
bbb.noise.value~
jit.bbb.noise.value
```

Use cases:

- Lightweight smooth random interpolation
- Simple modulation
- Procedural texture base layer

### 8.4 Cellular / Worley noise

Objects:

```max
bbb.noise.cellular
bbb.noise.cellular~
jit.bbb.noise.cellular
```

Use cases:

- Cells
- Cracks
- Voronoi-like structures
- Distance-field style textures

Recommended attribute:

```max
@cellular_mode f1/f2/f2minusf1/id
```

Default:

```max
@cellular_mode f1
```

### 8.5 fBM

Objects:

```max
bbb.noise.fbm
bbb.noise.fbm~
jit.bbb.noise.fbm
```

Default basis:

```max
@basis simplex
```

Attributes:

```max
@basis perlin/simplex/value
@octaves <int>
@gain <float>
@lacunarity <float>
```

Default values:

```max
@octaves 4
@gain 0.5
@lacunarity 2.0
```

### 8.6 Turbulence

Objects:

```max
bbb.noise.turbulence
bbb.noise.turbulence~
jit.bbb.noise.turbulence
```

Default basis:

```max
@basis simplex
```

Use cases:

- Smoke-like modulation
- Distorted natural motion
- Absolute-value fractal patterns

### 8.7 Ridged noise

Objects:

```max
bbb.noise.ridged
bbb.noise.ridged~
jit.bbb.noise.ridged
```

Default basis:

```max
@basis simplex
```

Use cases:

- Mountain ridges
- Veins
- Sharp striations
- High-contrast procedural texture

### 8.8 Domain warp

Objects:

```max
bbb.noise.domainwarp
bbb.noise.domainwarp~
jit.bbb.noise.domainwarp
```

Use cases:

- Warped textures
- Organic motion
- Liquid-like deformation
- Procedural distortion

Potential attributes:

```max
@basis simplex
@warp_basis simplex
@warp_amount 1.0
@warp_scale 1.0
@warp_octaves 1
```

### 8.9 Curl noise

Objects:

```max
bbb.noise.curl
bbb.noise.curl~
jit.bbb.noise.curl
```

Status:

```text
experimental
```

Curl noise produces vector fields rather than scalar noise, so its API may diverge from the scalar noise objects.

Use cases:

- Particle advection
- Flow fields
- Swirling motion
- Vector-field modulation

Initial implementation should be experimental and subject to revision after patching tests.

## 9. All-in-one objects

The all-in-one objects are:

```max
bbb.noise
bbb.noise~
jit.bbb.noise
```

They use `@basis` to select the algorithm.

Example:

```max
bbb.noise @basis perlin @input_dim 2
bbb.noise @basis simplex @input_dim 4
bbb.noise @basis cellular @cellular_mode f2minusf1
bbb.noise @basis fbm @basis simplex @octaves 5
```

Potential `@basis` values:

```text
perlin
simplex
value
cellular
fbm
turbulence
ridged
domainwarp
curl
```

Note: nested use of `@basis` for fractal objects may require clearer naming, for example:

```max
@basis fbm @source simplex
```

or:

```max
@basis fbm @fractal_basis simplex
```

This should be resolved in SPEC.md.

## 10. v1 scope (confirmed)

v1 includes 24 objects across three tiers, with MC support for MSP objects.

### 10.1 Implementation decisions

| Decision | Choice | Rationale |
|----------|--------|-----------|
| Noise backend | FastNoiseLite | Single-header, covers Perlin/Simplex/Value/Cellular + fractal + domain warp |
| 1D noise | 2D with y=0 | FastNoiseLite has no 1D mode |
| 4D noise | Custom implementation | FastNoiseLite supports 2D/3D only; 4D Perlin/Simplex from well-known algorithms |
| MSP class naming | `bbb_noise_perlin_tilde` | `~` → `_tilde` suffix |
| MC support | From v1 | Using `mc_operator` |
| Jitter API | Raw Jitter C API | min-api does not wrap Jitter objects |
| Fractal inner basis | `@source` | Avoids `@basis` conflict on all-in-one objects |

### 10.2 v1 object list (24 objects)

**Basic algorithms — Max event-rate (4):**

```text
bbb.noise.perlin
bbb.noise.simplex
bbb.noise.value
bbb.noise.cellular
```

**Basic algorithms — MSP signal-rate (4):**

```text
bbb.noise.perlin~
bbb.noise.simplex~
bbb.noise.value~
bbb.noise.cellular~
```

**Basic algorithms — Jitter matrix (4):**

```text
jit.bbb.noise.perlin
jit.bbb.noise.simplex
jit.bbb.noise.value
jit.bbb.noise.cellular
```

**Fractal algorithms — Max event-rate (3):**

```text
bbb.noise.fbm
bbb.noise.turbulence
bbb.noise.ridged
```

**Fractal algorithms — MSP signal-rate (3):**

```text
bbb.noise.fbm~
bbb.noise.turbulence~
bbb.noise.ridged~
```

**Fractal algorithms — Jitter matrix (3):**

```text
jit.bbb.noise.fbm
jit.bbb.noise.turbulence
jit.bbb.noise.ridged
```

**All-in-one (3):**

```text
bbb.noise
bbb.noise~
jit.bbb.noise
```

### 10.3 Out of scope for v1

```text
bbb.noise.domainwarp / domainwarp~ / jit.bbb.noise.domainwarp
bbb.noise.curl / curl~ / jit.bbb.noise.curl
Dimension aliases (bbb.noise.perlin1, perlin2, etc.)
```

Domain warp and curl noise are deferred to v2.

### 10.4 v1 attributes

Common (all objects):

```text
@input_dim 1..4
@signed 0/1
@seed <int>
@random_seed 0/1
@scale <float or list[1-4]>
@offset <float or list[1-4]>
```

Fractal-specific (fbm, turbulence, ridged):

```text
@source perlin/simplex/value
@octaves <int>
@gain <float>
@lacunarity <float>
```

Cellular-specific:

```text
@cellular_mode f1/f2/f2minusf1/id
```

All-in-one-specific:

```text
@basis perlin/simplex/value/cellular/fbm/turbulence/ridged
```

Jitter-specific:

```text
@dim <int...>
@type char/float32/float64
@planecount <int>
@coord pixel/norm/centered
@time <float>
@planeoffset <float>
```

MSP-specific:

```text
@mc_mode voices/coords
```

## 11. Resolved design questions

### 11.1 Fractal basis naming — Resolved: `@source`

```max
bbb.noise @basis fbm @source simplex
bbb.noise.fbm @source simplex
```

`@basis` selects the top-level algorithm (on all-in-one objects only).
`@source` selects the inner noise type for fractal objects.

### 11.2 Jitter coordinate overrides — Resolved: persistent

Coordinate overrides (x/y/z/w messages, proxy inlets) are persistent until cleared.

```max
clear_override x
clear_override y
clear_override z
clear_override w
clear_overrides
```

### 11.3 Curl noise — Deferred to v2

Curl noise produces vector output that diverges from the scalar noise API.
Deferred until the scalar API is validated in v1.

### 11.4 Alias objects — Not in v1

Dimension aliases (bbb.noise.perlin2, etc.) not included unless user testing shows demand.

### 11.5 4D noise — Custom implementation

FastNoiseLite provides 2D and 3D only. For `@input_dim 4`:
- 4D Perlin: classic gradient noise with 4D permutation/gradient tables
- 4D Simplex: Stefan Gustavson's OpenSimplex 4D algorithm
- 4D Value: lattice interpolation in 4D
- 4D Cellular: distance to nearest feature points in 4D

For `@input_dim 1`: evaluate 2D noise with y=0.

## 12. Example patches

### 12.1 Event-rate 2D Perlin

```max
bbb.noise.perlin @input_dim 2 @signed 0 @scale 0.01
```

Inputs:

```max
0.25 0.5
x 0.25
y 0.5
bang
```

### 12.2 Event-rate 4D Simplex

```max
bbb.noise.simplex @input_dim 4 @signed 1 @scale 1. 1. 1. 0.1
```

### 12.3 Signal-rate 2D Perlin

```max
bbb.noise.perlin~ @input_dim 2 @signed 1
```

### 12.4 MC voices mode

```max
bbb.noise.simplex~ @input_dim 2 @mc_mode voices
```

Each MC channel is an independent noise voice.

### 12.5 MC coords mode

```max
bbb.noise.simplex~ @input_dim 4 @mc_mode coords
```

Inlet 1 MC channels are interpreted as x/y/z/w.

### 12.6 Jitter 2D Simplex texture

```max
jit.bbb.noise.simplex @dim 640 480 @type float32 @input_dim 2 @coord norm @scale 4. @signed 0
```

### 12.7 Jitter animated 4D Simplex texture

```max
jit.bbb.noise.simplex @dim 640 480 @type float32 @input_dim 4 @coord norm @scale 4. 4. 1. 0.1 @time 0.
```

Animate `@time` or the `w` proxy inlet.

### 12.8 Jitter char Cellular texture

```max
jit.bbb.noise.cellular @dim 320 240 @type char @input_dim 2 @coord centered @scale 8. @planeoffset 17.
```

## 13. Implementation notes

### 13.1 Shared evaluator layer

Implement a shared C/C++ evaluator layer used by:

```text
Max event-rate objects
MSP signal-rate objects
Jitter matrix objects
```

Suggested structure:

```text
bbb_noise_evaluator
bbb_noise_params
bbb_noise_eval_1d
bbb_noise_eval_2d
bbb_noise_eval_3d
bbb_noise_eval_4d
```

### 13.2 Wrappers

Each object wrapper should mostly handle:

- Max attribute parsing
- Inlet/proxy routing
- Type conversion
- Matrix iteration
- Signal vector processing
- MC channel interpretation
- Output normalization

Noise math should live in shared code.

### 13.3 Determinism

Test determinism across:

- Object creation
- Patch reload
- Seed changes
- Matrix regeneration
- Signal vector evaluation

Same seed + same input should produce same output.

### 13.4 Numeric precision

Max event-rate input and output should use double internally.

MSP signal-rate processing may use signal-vector precision depending on Max/MSP API constraints, but object attributes and stored coordinates should use double where practical.

Jitter output supports:

```text
char
float32
float64
```

## 14. Test requirements

### 14.1 Common tests

- `@input_dim` accepts only 1-4.
- `@signed 0` outputs within `[0.0, 1.0]` for float outputs.
- `@signed 1` outputs within `[-1.0, 1.0]` for float outputs.
- `@seed N` is deterministic.
- `randomize` changes the seed.
- Scalar `@scale` applies to all axes.
- List `@scale` applies per-axis values.
- Scalar `@offset` applies to all axes.
- List `@offset` applies per-axis values.
- Coordinate transform is `coord * scale + offset`.

### 14.2 Max event-rate tests

- Float input sets x and outputs.
- List input sets x/y/z/w and outputs.
- `x`, `y`, `z`, `w` messages set stored coordinates without output.
- Inlets 2-4 set y/z/w without output.
- `bang` outputs using stored coordinates.
- Extra list elements are ignored.
- Missing list elements use stored coordinate values.

### 14.3 MSP tests

- Signal-rate output remains in expected range.
- Float inputs to signal inlets behave as constant signals.
- `@mc_mode voices` treats each MC channel independently.
- `@mc_mode coords` treats inlet 1 MC channels as coordinate components.
- `@mc_mode coords` outputs mono.

### 14.4 Jitter tests

- `@type char` outputs values in `[0, 255]`.
- `@type float32` respects `@signed` range.
- `@type float64` respects `@signed` range.
- `@coord pixel`, `@coord norm`, and `@coord centered` produce expected coordinate mapping.
- `@planeoffset` decorrelates planes.
- `@time` affects animated higher-dimensional noise.
- Proxy inlet coordinate overrides behave consistently.

## 15. Next step

See `SPEC.md` for the normative development specification.
See `TODO.md` for the implementation task list.
