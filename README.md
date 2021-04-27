# Pelota - Tennis Simulation Game

## General Information

Pelota is an open source tennis simulation game, which uses the OpenGL programming interface.

**Pelota currently is in early development and is not in a playable state yet.**

## Dependencies

On Ubuntu/Debian systems:

`sudo apt install libglfw3-dev libglm-dev libassimp-dev libfreetype6-dev libbullet-dev libsfml-dev`

## Build

Simple build procedure:

```
$ meson build                        # Build configuration
$ cd build
$ ninja                              # Build project
```

## Acknowledgments

Thanks to Joey de Vries for his great OpenGl tutorial at [https://learnopengl.com/](https://learnopengl.com/). Pelota is based on the code snippets from his tutorial.
