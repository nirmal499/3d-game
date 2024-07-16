# OpenGL Based 3D Renderer With MMO feature

![3D Renderer](img/pic1.png)

A 3D renderer using OpenGL that supports massively multiplayer online (MMO) network system. This project includes an OpenGL-based graphics engine, object loading with Assimp, lighting and shading with shaders, and a server-client architecture for MMO functionality.

### Prerequisites
Ensure you have the following installed:

- Assimp
- Boost 1.84

## Installation

Step-by-step instructions on how to run the project.

```bash
git clone --recursive https://github.com/nirmal499/3d-renderer.git
cd 3d-renderer

chmod u+x run.sh

# Configure the project
./run.sh project-configure-fresh

# Build the project
./run.sh project-build

# To run the client(renderer)
./run.sh project-run-client

# To run the MMO server
./run.sh project-run-server
```
**A lot more features will be added in this project {WORK IN PROGRESS}**