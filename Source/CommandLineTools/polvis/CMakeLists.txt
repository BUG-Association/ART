add_executable(
	polvis
	polvis.m
	)

add_sanitizers(
	polvis
	)

target_link_libraries(
	polvis
	${art_generic_link_libraries}
	)

install (
	TARGETS
		polvis
	DESTINATION
		${art_executable_directory}
	)
