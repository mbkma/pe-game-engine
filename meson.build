project('pelota', 'cpp',
  version: '0.0.1',
  meson_version: '>= 0.40.0',
)

i18n = import('i18n')
cc = meson.get_compiler('cpp')
dl_dep = cc.find_library('dl', required : false)
fs = cc.find_library('stdc++fs', required : false)

add_languages('c', required: true)

subdir('src')

meson.add_install_script('meson-post-install.py')


