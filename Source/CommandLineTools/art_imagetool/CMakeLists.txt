add_executable(
	art_imagetool
	art_imagetool.m
	)

target_link_libraries(
	art_imagetool
	${art_generic_link_libraries}
	)

target_compile_options(art_imagetool PRIVATE -Wall -Wextra)

install (
	TARGETS
		art_imagetool
	DESTINATION
		${art_executable_directory}
	)
