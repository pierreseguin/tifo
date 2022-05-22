project('stegano', 'cpp', default_options : ['cpp_std=c++20'])

arguments = meson.get_compiler('cpp').get_supported_arguments('-w')
add_project_arguments(arguments,  language : 'cpp')

includes = [include_directories('includes')]

subdir('src')
stb = static_library('stb', sources: sources_stb, include_directories: includes)

executable('lsbr', sources: sources_stegano + lsbr, link_with: stb, include_directories: includes)
executable('lsbm', sources: sources_stegano + lsbm, link_with: stb, include_directories: includes)
executable('jsteg', sources: sources_stegano + jsteg_main, link_with: stb, include_directories: includes)
executable('reader', dependencies: opencvdep, sources: sources_stegano + reader, link_with: stb, include_directories: includes)
