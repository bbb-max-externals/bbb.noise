# bbb.noise

Procedural noise externals for Max/MSP/Jitter.

## Objects

### Event-Rate (Max messages)

| Object | Description |
|--------|-------------|
| `bbb.noise.perlin` | Perlin gradient noise (1D–4D) |
| `bbb.noise.simplex` | Simplex noise (1D–4D) |
| `bbb.noise.value` | Value/lattice noise (1D–4D) |
| `bbb.noise.cellular` | Cellular/Worley noise (F1, F2, F2-F1, ID) |
| `bbb.noise.fbm` | Fractal Brownian Motion |
| `bbb.noise.turbulence` | Turbulence (abs-sum fractal) |
| `bbb.noise.ridged` | Ridged multifractal |
| `bbb.noise` | All-in-one (select algorithm via `@basis`) |

### Signal-Rate (MSP audio)

| Object | Description |
|--------|-------------|
| `bbb.noise.perlin~` | Perlin noise (signal I/O, MC support) |
| `bbb.noise.simplex~` | Simplex noise (signal I/O, MC support) |
| `bbb.noise.value~` | Value noise (signal I/O, MC support) |
| `bbb.noise.cellular~` | Cellular noise (signal I/O, MC support) |
| `bbb.noise.fbm~` | fBM (signal I/O, MC support) |
| `bbb.noise.turbulence~` | Turbulence (signal I/O, MC support) |
| `bbb.noise.ridged~` | Ridged (signal I/O, MC support) |
| `bbb.noise~` | All-in-one MSP (signal I/O, MC support) |

### Jitter (Matrix) — *planned*

`jit.bbb.noise.*` — matrix-rate noise generation.

## Common Attributes

| Attribute | Type | Default | Range | Description |
|-----------|------|---------|-------|-------------|
| `input_dim` | int | 2 | 1–4 | Number of coordinate dimensions |
| `signed` | int | 0 | 0/1 | Bipolar (1) or unipolar (0) output |
| `seed` | int | 0 | any | Random seed (deterministic) |
| `scale` | float | 1.0 | any | Coordinate scale factor |
| `offset` | float | 0.0 | any | Coordinate offset |

### Fractal-only

| Attribute | Type | Default | Range | Description |
|-----------|------|---------|-------|-------------|
| `source` | symbol | simplex | perlin/simplex/value | Basis noise type |
| `octaves` | int | 4 | 1–8 | Number of fractal layers |
| `gain` | float | 0.5 | 0–1 | Amplitude decay per octave |
| `lacunarity` | float | 2.0 | >0 | Frequency multiplier per octave |

### Cellular-only

| Attribute | Type | Default | Range | Description |
|-----------|------|---------|-------|-------------|
| `cellular_mode` | symbol | f1 | f1/f2/f2minusf1/id | Distance function |

### All-in-one only

| Attribute | Type | Default | Range | Description |
|-----------|------|---------|-------|-------------|
| `basis` | symbol | perlin | perlin/simplex/value/cellular/fbm/turbulence/ridged | Noise algorithm |

## Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

Output: `externals/*.mxo` (macOS) / `externals/*.mxe64` (Windows).

### Requirements

- CMake 3.19+
- C++17 compiler
- macOS: Xcode 10+ (universal binary: x86_64 + arm64)
- Windows: Visual Studio 2019+ (pending)

## Dependencies

| Library | License | Purpose |
|---------|---------|---------|
| [min-api](https://github.com/Cycling74/min-api) | MIT | Max/MSP external C++ API |
| [max-sdk-base](https://github.com/Cycling74/max-sdk-base) | MIT | Max SDK base (included via min-api) |
| [bbb-noise](https://github.com/2bbb/bbb-noise) | MIT | Header-only noise algorithms (Perlin, Simplex, Value, Cellular, Fractal) |
| [readerwriterqueue](https://github.com/cameron314/readerwriterqueue) | BSD-2-Clause | Lock-free queue (included via min-api) |

## License

MIT — see [LICENSE](./LICENSE).
