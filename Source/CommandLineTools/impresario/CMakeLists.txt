add_executable(
	impresario
	impresario.m
	)

add_sanitizers(
	impresario
	)

target_link_libraries(
	impresario
	${art_generic_link_libraries}
	)

install (
	TARGETS
		impresario
	DESTINATION
		${art_executable_directory}
	)
