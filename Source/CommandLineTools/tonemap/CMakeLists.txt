add_executable(
	tonemap
	tonemap.m
	)

target_link_libraries(
	tonemap
	${art_generic_link_libraries}
	)

install (
	TARGETS
		tonemap
	DESTINATION
		${art_executable_directory}
	)
