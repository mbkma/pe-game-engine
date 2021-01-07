sources = [
    'main.cpp',
    'ball-object.cpp',
    'camera.cpp',
    'game.cpp',
    'game-object.cpp',
    'mesh.cpp',
    'model.cpp',
    'renderer.cpp',
    'resource-manager.cpp',
    'shader.cpp',
    'text-renderer.cpp',
    'tournament.cpp',
    'player.cpp',
    'match.cpp',
    'glad.c',
]

add_project_arguments('-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp', language : 'cpp')
add_project_arguments('-std=c++17', language : 'cpp')
add_project_arguments('-lstdc++fs', language : 'cpp')

glfw = dependency('glfw3')
glm = dependency('glm')
assimp = dependency('assimp')
freetype = dependency('freetype2')

executable('pelota', sources, dependencies : [glfw, glm, assimp, freetype, dl_dep, fs], cpp_pch : 'stb_image/stb_image.h', install: true)

