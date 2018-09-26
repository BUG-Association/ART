add_executable(
arm2art
arm2art.m
)

target_link_libraries(
arm2art
	${art_generic_link_libraries}
)

install (
	TARGETS
		arm2art
	DESTINATION
		${art_executable_directory}
	)
