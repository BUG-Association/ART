file (
    GLOB
    library_sources
    "*.[h|c|m|mm]"
    )

string(REPLACE "/" ";" p2list "${CMAKE_CURRENT_SOURCE_DIR}")
list(REVERSE p2list)
list(GET p2list 0 first)
list(GET p2list 1 second)

file( APPEND ${source_group_file} "source_group(\n" )
file( APPEND ${source_group_file} "	\"${first}\"\n")
file( APPEND ${source_group_file} "	FILES\n")

foreach( src_file ${library_sources} )
	file( APPEND ${source_group_file} "	${src_file}\n")
endforeach()

file( APPEND ${source_group_file} "	)\n\n")

add_subdirectory (System) 
add_subdirectory (Datastructures) 
add_subdirectory (Math) 
add_subdirectory (Geometry) 
add_subdirectory (ColourAndSpectra) 
add_subdirectory (LightAndAttenuation) 
add_subdirectory (Physics)
add_subdirectory (ObjectiveC)
