# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required ( VERSION 3.8 )
set( CMAKE_SUPPRESS_REGENERATION true )

project ( "Hello_Dungeon" )

# Include sub-projects.
add_subdirectory ( "Hello_Dungeon" )
