add_executable(
	polvis
	polvis.m
	)

target_link_libraries(
	polvis
	${art_generic_link_libraries}
	)

target_compile_options(polvis PRIVATE -Wall -Wextra)

install (
	TARGETS
		polvis
	DESTINATION
		${art_executable_directory}
	)
