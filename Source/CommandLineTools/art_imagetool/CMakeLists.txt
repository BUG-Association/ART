add_executable(
	art_imagetool
	art_imagetool.m
	)

add_sanitizers(
	art_imagetool
	)

target_link_libraries(
	art_imagetool
	${art_generic_link_libraries}
	)

install (
	TARGETS
		art_imagetool
	DESTINATION
		${art_executable_directory}
	)
