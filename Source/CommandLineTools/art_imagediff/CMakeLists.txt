add_executable(
  art_imagediff
  art_imagediff.m
  )

target_link_libraries(
  art_imagediff
  ${art_generic_link_libraries}
  )

target_compile_options(art_imagediff PRIVATE -Wall -Wextra)

install (
  TARGETS
    art_imagediff
  DESTINATION
    ${art_executable_directory}
  )
