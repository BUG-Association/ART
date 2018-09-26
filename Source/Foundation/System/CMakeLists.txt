file (
    GLOB
    library_sources
    "*.[h|c|m|mm]"
    )

# get the last two path elements

string(REPLACE "/" ";" p2list "${CMAKE_CURRENT_SOURCE_DIR}")
list(REVERSE p2list)
list(GET p2list 0 first)
list(GET p2list 1 second)

file( APPEND ${source_group_file} "source_group(\n" )
file( APPEND ${source_group_file} "	\"${second}\\\\${first}\"\n")
file( APPEND ${source_group_file} "	FILES\n")

foreach( src_file ${library_sources} )
	file( APPEND ${source_group_file} "	${src_file}\n")
endforeach()

file( APPEND ${source_group_file} "	)\n\n")