add_executable(
	greymap
	greymap.m
	)

target_link_libraries(
	greymap
	${art_generic_link_libraries}
	)

install(
	TARGETS
		greymap
	DESTINATION
		${art_executable_directory}
	)
