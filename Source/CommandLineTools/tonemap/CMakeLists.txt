add_executable(
	tonemap
	tonemap.m
	)

add_sanitizers(
	tonemap
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
