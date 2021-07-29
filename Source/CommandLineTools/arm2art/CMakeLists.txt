add_executable(
	arm2art
	arm2art.m
	)

target_link_libraries(
	arm2art
	${art_generic_link_libraries}
	)

target_compile_options(arm2art PRIVATE -Wall -Wextra)

install (
	TARGETS
		arm2art
	DESTINATION
		${art_executable_directory}
	)
