add_executable(
	art_imagediff
	art_imagediff.m
	)

add_sanitizers(
	art_imagediff
	)

target_link_libraries(
	art_imagediff
	${art_generic_link_libraries}
	)

install (
	TARGETS
		art_imagediff
	DESTINATION
		${art_executable_directory}
	)
