add_executable(
	artist
	artist.m
	)

add_sanitizers(
	artist
	)

target_link_libraries(
	artist
	${art_generic_link_libraries}
	)

install (
	TARGETS
		artist
	DESTINATION
		${art_executable_directory}
	)
