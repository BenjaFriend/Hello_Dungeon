cmake_minimum_required ( VERSION 3.8 )

############### Project Settings ###############
set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin" )

# Get rid of zero_check project gen
set( CMAKE_SUPPRESS_REGENERATION true )

############### Include Files ##################
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


### Setup visual studio source groups / filters ###
file( GLOB_RECURSE _source_list *.cpp* *.h* *.hpp* )

foreach( _source IN ITEMS ${_source_list} )

	message( "Source file: " ${_source} )
    get_filename_component( _source_path "${_source}" PATH )
    string( REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}" )
    string( REPLACE "/" "\\" _group_path "${_group_path}" )
    source_group( "${_group_path}" FILES "${_source}" )

endforeach()


################# Add Exe ######################
message ( STATUS "\tMY  Flags:" ${MY_COMPILER_FLAGS} )
message ( STATUS "\tcxx Flags:" ${CMAKE_CXX_FLAGS} )

# Add source to this project's executable.
add_executable( ${PROJECT_NAME} ${_source_list} )
