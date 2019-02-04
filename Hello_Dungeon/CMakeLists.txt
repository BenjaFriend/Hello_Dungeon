
cmake_minimum_required (VERSION 3.8)
############### Project Settings ###############

project( "Hello_Dungeon" )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin" )

# Grab the source files
file( GLOB_RECURSE SRC_FILES "src/*.c*" "src/*.h*" )

include_directories(
    include
    src
    )

################# Complier Options ############
if( MSVC )
    set ( MY_COMPILER_FLAGS "/W3" )
else()
    set ( MY_COMPILER_FLAGS "-Wall -Wextra -pedantic" )
	message ( WARNING "\tThis project uses the winsoc2 library! Needs windows..." )
endif()

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_COMPILER_FLAGS}" )


################# Add Exe ######################
message ( STATUS "\tMY  Flags:" ${MY_COMPILER_FLAGS} )
message ( STATUS "\tcxx Flags:" ${CMAKE_CXX_FLAGS} )

# Add source to this project's executable.
add_executable( ${PROJECT_NAME} ${SRC_FILES} )