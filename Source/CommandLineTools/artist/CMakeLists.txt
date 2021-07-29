add_executable(
	artist
	artist.m
	)

target_link_libraries(
	artist
	${art_generic_link_libraries}
	)

target_compile_options(artist PRIVATE -Wall -Wextra)

install (
	TARGETS
		artist
	DESTINATION
		${art_executable_directory}
	)
