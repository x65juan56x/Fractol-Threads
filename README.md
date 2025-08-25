# Fractol-Threads

Interactive fractal explorer (Mandelbrot, Julia, and Nova) with multi-threaded rendering using MLX42.

## Requirements & Build

- Linux, GCC/Clang
- MLX42 vendored in `libraries/MLX42`
- Link dependencies: `glfw`, `dl`, `pthread`, `m`

Build targets:
- `make` builds MLX42 (via CMake), `ft_printf`, and the `fractol` binary.
- Object files are stored under `obj/` mirroring the source tree.

Clean:
- `make clean` removes `obj/` and cleans `ft_printf`.
- `make fclean` also removes the binary.
- `make re` does a full rebuild.

## Run

- `./fractol mandelbrot`
- `./fractol julia [re] [im]` (defaults to `-0.75 0.11`)
- `./fractol nova`

Default iteration budget per fractal:
- Mandelbrot: 100
- Julia: 150
- Nova: 64

## Controls (keyboard/mouse)

- H: Toggle help overlay
- ESC: Quit
- Arrow keys: Pan (step scales with zoom)
- R: Reset zoom and position
- Mouse wheel: Zoom in/out centered on the cursor
- C: Cycle color scheme (0..9)
- I: Invert colors
- 1 / 2 / 3: Switch fractal (Mandelbrot / Julia / Nova)
- \+ / - (or KP_ADD / KP_SUBTRACT): Increase / decrease iterations (±10% + 1, clamped to `base_iter` minimum)
- S / W: Decrease / increase Nova exponent `nova_p` (integer)
- T / Y: Decrease / increase thread count (1..32) (12 is default)

## High-level architecture

- `src/fractol.c`: Entry point. Parses args, initializes `t_fractol_data`, creates MLX window/image, sets hooks, and starts the loop.
- `include/fractol.h`: Types, constants, prototypes, and the main app state `t_fractol_data`.
- Rendering
  - `src/render_stuff.c`: Computes view/window (`update_view`), splits work into horizontal bands (`draw_range`), and coordinates frame synchronization with the thread pool.
  - `src/fractals/*.c`: Iteration math for each fractal: `calculate_mandelbrot`, `calculate_julia`, `calculate_nova`.
- Threading
  - `src/thread_pool.c`: Creates/destroys/resizes the worker pool. Workers wait on a condition variable for a new `frame_id`, process their job, and decrement `remaining`.
- Input/Events
  - `src/hook_stuff/*.c`: Keyboard/mouse/resize hooks. They mutate state and trigger a re-render.
- Colors
  - `src/color_stuff/*.c`: Palettes, color selection, inversion, and RGB helpers.

### Render flow

1) An input event changes state in `t_fractol_data`.
2) `render_fractal_with_params` runs:
   - Recompute complex plane bounds (`update_view`).
   - Split image rows into N jobs (N = thread count).
   - Signal workers (condvar) with a new `frame_id`.
   - Wait until `remaining == 0`.
3) Each worker executes `draw_range` on its band, computing per-pixel iterations and drawing with `mlx_put_pixel`.

## Threading model (how it works here)

- Persistent pool
  - A fixed-size pool is created at startup (default 12 threads) and can be resized at runtime with T/Y.
  - Threads live for the entire program; they sleep on a condition variable when idle.

- Jobs and partitioning
  - Each frame, the image is split into horizontal bands: one contiguous [y0, y1) range per worker.
  - Bands cover the full height; the last worker receives the remainder rows when `h % thread_count != 0`.
  - Work is static per frame (no work stealing). This keeps overhead low and avoids locks during drawing.

- Scheduling a frame
  - `render_fractal_with_params` computes the view (`update_view`), fills a `jobs[]` array (one entry per worker with `y0/y1`), sets `remaining = thread_count`, increments `frame_id`, then broadcasts the condition variable.
  - If `thread_count <= 1`, it falls back to a single call to `draw_range` without synchronization overhead.

- Worker loop
  - Each worker keeps a `local_frame` counter. It waits until the shared `frame_id` increases, then reads its `jobs[id]` and calls `draw_range(d, y0, y1)` on the current image.
  - After finishing, it locks the mutex, decrements `remaining`, and if it reaches zero, signals the main thread (which is waiting for the frame to complete). It then updates `local_frame` and waits for the next frame.

- Synchronization
  - Main and workers share `mtx` (mutex) and `cond` (condition variable).
  - The main thread waits in a loop until `remaining == 0` to avoid spurious wake-ups.
  - A `shutdown` boolean is set during destruction to make workers exit their loop cleanly.

- Safety of parallel drawing
  - Each worker writes only to its own set of rows, so pixel writes never overlap; no per-pixel locking is needed.
  - `mlx_image_t`’s buffer is updated directly via `mlx_put_pixel`; band separation makes these writes race-free.

- Resizing and image recreation
  - On window resize, the image is recreated and immediately rendered. Workers always read the current `d->img` when they run, so they target the latest image.
  - Help overlay is rebuilt separately; workers only draw the fractal image, not UI elements.

- Resizing the pool (T/Y)
  - Changing thread count triggers a pool resize: current workers are joined, arrays (`threads`, `jobs`, worker args) are reallocated, and a new pool is spawned.
  - Resizing is done between frames to avoid disrupting in-flight work.

- Performance notes
  - Static row partitioning is simple and cache-friendly. Load is reasonably balanced for these fractals, but dynamic queues could be added if needed for highly heterogeneous workloads.
  - CPU usage scales with thread count up to available cores; beyond that, context switching dominates.

### Band partition (example)

Assume height H=720 and thread_count T=6. Rows are split into 6 bands; the last band absorbs any remainder:

```
 y=0
    ┌───────────────────────────────────────────────┐  band #0 → [   0 .. 119)
    │ rows 0..119                                   │
    ├───────────────────────────────────────────────┤  band #1 → [ 120 .. 239)
    │ rows 120..239                                 │
    ├───────────────────────────────────────────────┤  band #2 → [ 240 .. 359)
    │ rows 240..359                                 │
    ├───────────────────────────────────────────────┤  band #3 → [ 360 .. 479)
    │ rows 360..479                                 │
    ├───────────────────────────────────────────────┤  band #4 → [ 480 .. 599)
    │ rows 480..599                                 │
    ├───────────────────────────────────────────────┤  band #5 → [ 600 .. 720)
    │ rows 600..719 (last band may be larger)       │
 y=H└───────────────────────────────────────────────┘
```

Each worker i renders only its band [y0, y1), so writes never overlap.

### Frame timeline (condvar-based)

```
Main thread                          Worker threads (i = 0..T-1)
-----------                          ---------------------------
update_view()
fill jobs[i] with y0/y1             wait(mtx, cond) until frame_id > local_frame
remaining = T                       └─ wake on broadcast
frame_id++                          read jobs[i]
broadcast(cond) ─────────────────▶   draw_range(d, y0, y1)
wait until remaining == 0           lock(mtx); remaining--; if remaining==0
            └── signal(cond) and/or wake main
            unlock(mtx); local_frame = frame_id
return to event loop                wait for next broadcast
```

Resizes and pool changes happen between frames to keep this sequence simple and safe.

### Main state: `t_fractol_data` (essentials)

- Window/image: `w`, `h`, `img`
- Camera: `zoom`, `offset_x`, `offset_y`, bounds `min_x/max_x/min_y/max_y` and steps `dx/dy`
- Fractal: `fractal_type`, `julia_c`, `nova_p`
- Iterations: `base_iter`, `max_iter`
- Color: `color_scheme`, `invert_mode`
- UI: `show_help`, `help_img`, `help_text_imgs`
- Threads: `thread_count`, `threads`, `jobs[]`, `frame_id`, `remaining`, `mtx`, `cond`, `shutdown`

## Fractal math

### Pixel → complex plane mapping

- Compute a view centered at `(offset_x, offset_y)` with scale `1/zoom` and aspect ratio `w/h`.
- For each pixel `(x, y)`:
  - `cr = min_x + x * dx`
  - `ci = min_y + y * dy`

### Escape criterion and iterations

- Iterate up to `max_iter` or until `|z|^2 > 4` (radius 2)
- The returned iteration governs the color.

### Mandelbrot

- Recurrence: `z_{n+1} = z_n^2 + c`, with `z_0 = 0` and `c = cr + i*ci` (from pixel)
- Optimizations: inside tests (main cardioid and period-2 bulb) mark as interior without iterating.

### Julia

- Recurrence: `z_{n+1} = z_n^2 + c`, with fixed `c = julia_c` and `z_0 = cr + i*ci` (from pixel)
- Changing `c` yields families of Julia sets.

### Nova (Newton variant)

- Iteration: `z_{n+1} = z_n - (z_n^p - 1) / (p * z_n^{p-1}) + c`
  - `p` is `nova_p` (adjustable via S/W)
  - `c = cr + i*ci` depends on pixel
  - Start from `z_0 = 1 + 0i`
- Additional stop: break early when `|z_{n+1} - z_n|^2 < 1e-4` (convergence)
  - What it checks: the step size `Δ = z_{n+1} - z_n`. Using the squared magnitude avoids a costly sqrt and is equivalent to `|Δ| < 1e-2`.
  - Why: Newton-like methods converge rapidly near roots; stopping early saves iterations and reduces noise in basins.
  - Coloring impact: the iteration index where it stops encodes “time to converge” — fewer iterations map to faster-converging colors.
  - Tuning: lower the threshold (e.g. `1e-6`) for stricter convergence (sharper basins, more iterations) or raise it (e.g. `1e-3`) for faster renders with potentially chunkier boundaries.

## Color palettes and coloring

For a pixel with `iter` and `max_iter`:
- If `iter == max_iter`: interior color `0x000000FF` (opaque black)
- Else compute `t = iter / max_iter` and apply `color_scheme`:
  - 0: Simple gradient (increasing RGB)
  - 1: Sinusoidal gradient (phase-shifted RGB)
  - 2: Polynomial “magma” (`poly_color`) with precomputed coefficients
  - 3: “Cheshire cat” (sine wave, purple-ish)
  - 4: “Vintage stripes” (banded)
  - 5: Grayscale (fallback when none matches)
  - 6..9: Discrete palettes (`palette_set_color` index 0..3)
- Press `I` to invert colors; press `C` to cycle (0..9).

## Iterations and performance

- `base_iter` depends on the fractal and acts as a minimum.
- `max_iter` starts at `base_iter` and is adjusted by `+/-` (±10% + 1).
- It does not change automatically with zoom.
- Optimizations:
  - Cardioid/bulb tests for Mandelbrot (early interior return)
  - Multi-threaded bands

## Zoom and navigation

- Wheel zoom: `zoom *= 1.1` (or `/= 1.1`) and recompute `offset` so the cursor’s world point stays fixed.
- Zoom limits: `[1e-12, 1e12]`.
- Arrow pan: `0.1 / zoom` per press.
- Reset with `R`.

## Window resize

- Resize hook updates `w/h`, recreates the MLX image, and re-renders.
- If the help panel was visible, it’s recreated for the new size.

## Julia vs Mandelbrot (quick reference)

- Mandelbrot: `c` varies per pixel, `z_0 = 0`.
- Julia: `c` is fixed (user parameter), `z_0` varies per pixel.
- Both share the escape-time method; the role of `c` yields very different shapes.

## Extending

- New fractal:
  - Implement `int calculate_<name>(...)` in `src/fractals/`.
  - Add a branch in `draw_range` (in `render_stuff.c`).
  - Wire controls in `hook_stuff_keys.c` and update `help_please.c`.

- New palette:
  - Implement in `color_stuff/color_palettes.c` and plug into `select_color_scheme`.
  - Optionally include it in the `C` cycling.

## Repository layout

- `include/` project headers
- `src/` project sources
  - `fractals/` fractal math
  - `hook_stuff/` input & resize hooks
  - `color_stuff/` palettes & color helpers
- `libraries/` vendored deps (MLX42, libft, ft_printf)
- `Makefile` build entry

## Credits

- MLX42 (https://github.com/codam-coding-college/MLX42)
- 42’s `fractol`, `philosophers`, `libft` and `ft_printf`
