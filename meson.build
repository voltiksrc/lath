project(
   'lath',
   'cpp',
   default_options: ['cpp_std=c++23']
)
sources = files(
   'src/main.cpp',
   'src/recipe.cpp',
   'src/download.cpp',
   'src/archive.cpp',
   'src/package.cpp',
   'src/runner.cpp',
)
lua_dep = dependency('lua5.3')
curl_dep = dependency('libcurl')
archive_dep = dependency('libarchive')
executable(
   'lath',
   sources,
   dependencies: [
     lua_dep,
     curl_dep,
     archive_dep,
   ],
)
