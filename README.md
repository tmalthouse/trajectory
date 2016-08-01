# Trajectory

A 3d solar system simulator written in C, using SDL for rendering.

### About:

Trajectory is a solar system simulator, written in C. The physics is fully simulated in 3D, although the renderer is (currently) a top-down 2d view. Updates are calculated every thousand simulated seconds (every 17 minutes), which offers high precision while still remaining easy to calculate. The passage of time can be sped up and slowed down, making it just as easy to watch a close encounter between bodies as it happen as watching a thousand years pass by a second.

### Building:

To build, simply run `make`. Run the resulting `trajectory` executable.

### Dependencies:
 Trajectory needs the `SDL2`, `SDL2_gfx`, and `libconfig` libraries installed. If you don't have them, you can download them with your package manager:


 `brew install SDL2 SDL2_gfx libconfig` (on OSX)


 `sudo apt install SDL2 SDL2_gfx libconfig` (on Debian Linux)

### Controls:
