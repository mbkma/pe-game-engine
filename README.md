# PE-Game-Engine

## General Information

PE-Game-Engine is an open source game engine, which uses the [OpenGL programming interface](https://www.opengl.org/) und the [Bullet physics engine](https://pybullet.org/wordpress/). PE-Game-Engine was originally developed for the [Pelota tennis simulation game](https://github.com/mbkma/pelota-godot). Pelota was ported to the [Godot game engine](https://godotengine.org/).

**PE-Game-Engine currently is in early development**

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
