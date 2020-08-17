project('opengl-starter', 'cpp',
  default_options: [
    'prefix=' + meson.source_root() / 'packaged',
    'buildtype=debugoptimized',
    'warning_level=3',
    'werror=true',
    'cpp_std=c++17',
    'b_sanitize=address,undefined',
    'b_lundef=false',
  ]
)

add_project_arguments(
  '-Wno-deprecated-copy',
  '-Wno-unused-parameter',
  language: 'cpp',
)

eigen = dependency('eigen3')
glfw = dependency('glfw3')
glew = dependency('glew')

executable('main', 'src/main.cc', 'src/controller.cc', 'src/renderer.cc', dependencies: [eigen, glfw, glew], install: true)
