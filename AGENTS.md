# bbb.noise

Max/MSP/Jitter external suite for procedural noise (Perlin, Simplex, Value, Cellular, fBM, Turbulence, Ridged, Domain Warp, Curl).

## Project Status

**Planning phase.** No source code yet. `PLAN.md` is the authoritative design document — read it in full before any implementation work.

## Architecture

Three object tiers sharing a common C++ evaluator layer:

| Tier | Prefix | Example | Rate |
|------|--------|---------|------|
| Max event-rate | `bbb.noise.*` | `bbb.noise.perlin` | bang/float |
| MSP signal-rate | `bbb.noise.*~` | `bbb.noise.perlin~` | audio signal |
| Jitter matrix | `jit.bbb.noise.*` | `jit.bbb.noise.perlin` | matrix |

All-in-one objects (`bbb.noise`, `bbb.noise~`, `jit.bbb.noise`) select algorithm via `@basis`.

### Shared evaluator

Noise math lives in a shared C/C++ layer (`bbb_noise_evaluator`). Object wrappers handle only Max attribute parsing, inlet routing, type conversion, and output. Do not put noise algorithms in wrapper code.

## Build System

Uses `bbb_add_external()` from `cmake/bbb_external.cmake` (provided by the `max-external` skill). Standard flow:

```bash
mkdir -p build && cd build
cmake ..
cmake --build .
```

Outputs: `externals/*.mxo` (macOS) / `externals/*.mxe64` (Windows).

Adding a new external:
1. Create `source/projects/<name>/<name>.cpp` and `CMakeLists.txt`
2. Root CMake auto-discovers subdirectories — no root CMake changes needed
3. Add `.mxo` to `package-info.json` filelist

## Conventions

### Naming

| Layer | Format | Example |
|-------|--------|---------|
| Directory/file | `bbb.noise.perlin` (dots) | `source/projects/bbb.noise.perlin/` |
| C++ class | `bbb_noise_perlin` (underscores) | `class bbb_noise_perlin` |
| MIN_EXTERNAL macro | Same as class name | `MIN_EXTERNAL(bbb_noise_perlin)` |

### Design Principles

- **Deterministic by default**: `@seed 0`, same seed + same input = same output
- **`@input_dim` controls arity** (1-4), not the object name
- **Coordinate transform**: `p = coord * scale + offset` (consistent everywhere)
- **Double-precision** for Max event-rate coordinates internally

## Skills (loaded as git submodules)

| Skill | When to Use |
|-------|-------------|
| `max-external` | Creating/modifying C++ externals, CMake config, build issues |
| `max-external-githubactions` | Setting up CI/CD workflows |
| `max-patgen` | Generating `.maxpat` / `.maxhelp` files |

**Always load the relevant skill** before implementation work. Each skill contains critical pitfalls (especially `max-external/docs/pitfalls.md`).

## Key Gotchas (min-api)

- Attributes are set **after** constructor — use `m_init_timer.delay(0)` for deferred init
- Outlet output from worker threads silently fails — use `c74::min::queue<>` to deliver to main thread
- `enum_map` for dropdown attributes, not `range` + `style::enum_index`
- `cout`/`cerr` are member variables, not `std::cout`
- `std::filesystem` unavailable (deployment target 10.11) — use `c74::min::path`
- See `.agents/skills/max-external/docs/pitfalls.md` for the full list

## Next Step

Write `SPEC.md` from `PLAN.md` with normative MUST/SHOULD/MAY language (PLAN.md §15).
