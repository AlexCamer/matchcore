project('matching-engine', 'cpp', default_options : ['cpp_std=c++17'])
add_global_arguments('-funroll-loops', language : 'cpp')

cmake = import('cmake')
abseil = subproject('abseil')
doctest = subproject('doctest')
tessil = cmake.subproject('robin-map')

subdir('source')
