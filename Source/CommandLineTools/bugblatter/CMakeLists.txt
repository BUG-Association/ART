add_executable(
	bugblatter
	bugblatter.m
	)

target_link_libraries(
	bugblatter
	${art_generic_link_libraries}
	)

target_compile_options(bugblatter PRIVATE -Wall -Wextra)

install (
	TARGETS
		bugblatter
	DESTINATION
		${art_executable_directory}
	)
