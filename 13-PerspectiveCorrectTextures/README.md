# CPU Based 3D Rendering
Mapping texture to 3D objects using interpolation and  barycentric weights.

# How To Use
Press 1, 2, 3, 4, 5, 6, 7, 9 to change render mode (shading mode), and press wasd to move the model and x, z  to toggle backface culling.

## Demo
<img src="./demo.gif" />

## Setup
Install [MinGW](https://www.mingw-w64.org/downloads/).
Run this followed commands:

``` bash
# Install SDL2 library
sudo apt install build-essential
sudo apt install libsdl2-dev

# Compiling the project on your computer
make

# Running the application
make run
```

# Soloution for Possible Errors
1. Windows users can use WSL.