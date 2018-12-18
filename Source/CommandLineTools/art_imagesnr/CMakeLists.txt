add_executable(
	art_imagesnr
	art_imagesnr.m
	)

add_sanitizers(
	art_imagesnr
	)

target_link_libraries(
	art_imagesnr
	${art_generic_link_libraries}
	)

install (
	TARGETS
		art_imagesnr
	DESTINATION
		${art_executable_directory}
	)
