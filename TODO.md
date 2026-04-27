# TODO.md — bbb.noise v1 Implementation Tasks

## Phase 0: Noise Library (deps/bbb-noise)

- [ ] Create repo skeleton (`bbb/noise.hpp`, `bbb/noise/common.hpp`)
- [ ] Implement `common.hpp` — `permutation_table`, `fade`, `lerp`, `grad1-4`, `floor`, hash helpers
- [ ] Implement `perlin.hpp` — Perlin noise 1D–4D
- [ ] Implement `simplex.hpp` — Simplex noise 1D–4D (reference: Gustavson simplexnoise1234)
- [ ] Implement `value.hpp` — Value noise 1D–4D
- [ ] Implement `cellular.hpp` — Cellular/Worley F1/F2/F2-F1/ID 1D–4D
- [ ] Implement `fractal.hpp` — fBM, Turbulence, Ridged layering
- [ ] Implement `evaluator.hpp` — High-level `evaluator` class with `config` struct
- [ ] Wire `noise.hpp` — include all headers
- [ ] Push to `git@github.com:2bbb/bbb-noise.git`

## Phase 1: Project Foundation

- [ ] Root `CMakeLists.txt` (bbb_external.cmake, auto-discover projects)
- [ ] `cmake/bbb_external.cmake` — copy from max-external skill
- [ ] `deps/min-api/` — git submodule init
- [ ] `source/bbb/CMakeLists.txt` — interface library pointing to `deps/bbb-noise/`
- [ ] `package-info.json`

## Phase 2: First External (Proof of Concept)

- [ ] `source/projects/bbb.noise.perlin/bbb.noise.perlin.cpp`
- [ ] `source/projects/bbb.noise.perlin/CMakeLists.txt`
- [ ] Build `externals/bbb.noise.perlin.mxo` on macOS
- [ ] Test in Max: basic float input → float output

## Phase 3: Basic Max Event-Rate (3 more)

- [ ] `bbb.noise.simplex`
- [ ] `bbb.noise.value`
- [ ] `bbb.noise.cellular`

## Phase 4: Basic MSP Signal-Rate (4)

- [ ] `bbb.noise.perlin~`
- [ ] `bbb.noise.simplex~`
- [ ] `bbb.noise.value~`
- [ ] `bbb.noise.cellular~`

## Phase 5: Basic Jitter (4)

- [ ] `jit.bbb.noise.perlin`
- [ ] `jit.bbb.noise.simplex`
- [ ] `jit.bbb.noise.value`
- [ ] `jit.bbb.noise.cellular`

## Phase 6: Fractal Max Event-Rate (3)

- [ ] `bbb.noise.fbm`
- [ ] `bbb.noise.turbulence`
- [ ] `bbb.noise.ridged`

## Phase 7: Fractal MSP Signal-Rate (3)

- [ ] `bbb.noise.fbm~`
- [ ] `bbb.noise.turbulence~`
- [ ] `bbb.noise.ridged~`

## Phase 8: Fractal Jitter (3)

- [ ] `jit.bbb.noise.fbm`
- [ ] `jit.bbb.noise.turbulence`
- [ ] `jit.bbb.noise.ridged`

## Phase 9: All-in-One (3)

- [ ] `bbb.noise` (event-rate, @basis dispatch)
- [ ] `bbb.noise~` (signal-rate, @basis dispatch)
- [ ] `jit.bbb.noise` (Jitter, @basis dispatch)

## Phase 10: Polish

- [ ] Help patches (.maxhelp) for all 24 objects
- [ ] Edge case testing (extreme coords, seed 0, all input_dim values)
- [ ] Windows build
- [ ] CI (GitHub Actions)
