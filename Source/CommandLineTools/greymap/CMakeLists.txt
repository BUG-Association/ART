add_executable(
	greymap
	greymap.m
	)

target_link_libraries(
	greymap
	${art_generic_link_libraries}
	)

target_compile_options(greymap PRIVATE -Wall -Wextra)

install(
	TARGETS
		greymap
	DESTINATION
		${art_executable_directory}
	)
