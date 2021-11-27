# PE-Game-Engine

## General Information

- PE-Game-Engine is an open source game engine, which uses the [OpenGL](https://www.opengl.org/)  programming interface and the [Bullet](https://pybullet.org/wordpress/) physics engine.
- PE-Game-Engine was originally developed for the [Pelota](https://github.com/mbkma/pelota-godot) tennis simulation game which was ported to the [Godot](https://godotengine.org/) game engine.

*What is currently working:*

  - [x] Creating and displaying models through a convenient GameObject Interface. Models can be of any standard format (e.g. glb, obj, dae).
  - [x] Creating and simulating physic objects with an optional model through a convenient PhysicObject Interface.
  - [x] Background sound.
  - [x] Basic Input functionality.
  - [x] Shadow (at the moment this is considered experimental)

*What is being worked on:*
  - [ ] Displaying animations.


**PE-Game-Engine currently is in early development!**

## Dependencies

On Ubuntu/Debian systems:

`sudo apt install libglfw3-dev libglm-dev libassimp-dev libfreetype6-dev libbullet-dev libsfml-dev`

On Arch systems:

`sudo pacman -S glfw-x11 glm assimp freetype2 bullet sfml`

## Build

Simple build procedure:

```
$ meson build                        # Build configuration
$ cd build
$ ninja                              # Build project
```

## Acknowledgments

Thanks to Joey de Vries for his great OpenGl tutorial at [https://learnopengl.com/](https://learnopengl.com/). PE-Game-Engine is based on the code snippets from his tutorial.

## Picture

![im1](https://user-images.githubusercontent.com/39454100/143680941-7ba62ee5-35c8-434b-be09-b68238326874.png)
