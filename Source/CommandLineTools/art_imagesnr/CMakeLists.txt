add_executable(
	art_imagesnr
	art_imagesnr.m
	)

target_link_libraries(
	art_imagesnr
	${art_generic_link_libraries}
	)

target_compile_options(art_imagesnr PRIVATE -Wall -Wextra)

install (
	TARGETS
		art_imagesnr
	DESTINATION
		${art_executable_directory}
	)
