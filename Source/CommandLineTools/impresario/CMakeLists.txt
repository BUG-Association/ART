add_executable(
	impresario
	impresario.m
	)

target_link_libraries(
	impresario
	${art_generic_link_libraries}
	)

target_compile_options(impresario PRIVATE -Wall -Wextra)

install (
	TARGETS
		impresario
	DESTINATION
		${art_executable_directory}
	)
