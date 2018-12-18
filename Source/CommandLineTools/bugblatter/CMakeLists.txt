add_executable(
	bugblatter
	bugblatter.m
	)

add_sanitizers(
	bugblatter
	)

target_link_libraries(
	bugblatter
	${art_generic_link_libraries}
	)

install (
	TARGETS
		bugblatter
	DESTINATION
		${art_executable_directory}
	)
