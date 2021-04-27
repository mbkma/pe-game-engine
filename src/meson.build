sources = [
    'main.cpp',
    'game.cpp',
    'tournament.cpp',
    'player.cpp',
    'match.cpp',
]

subdir('engine')

add_project_arguments('-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp', language : 'cpp')
add_project_arguments('-std=c++17', language : 'cpp')
add_project_arguments('-lstdc++fs', language : 'cpp')

glfw = dependency('glfw3')
glm = dependency('glm')
assimp = dependency('assimp')
freetype = dependency('freetype2')
bullet = dependency('bullet')
sfmla = dependency('sfml-audio')

executable('pelota', sources, dependencies : [glfw, glm, assimp, freetype, bullet, sfmla, dl_dep, fs], cpp_pch : 'stb_image/stb_image.h', install: true)

