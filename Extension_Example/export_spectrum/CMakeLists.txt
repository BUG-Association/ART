#################################################
# ART - Advanced Rendering Toolkit - search paths
#################################################

# The XCode generator only works properly on cmake > 2.8.7
# On other platforms, earlier 2.8 versions are fine

if ( APPLE )
	cmake_minimum_required (VERSION 2.8.11)
else ( APPLE )
	cmake_minimum_required (VERSION 2.8)
endif ( APPLE )

project (export_spectrum)

if ( APPLE )
  if ( NOT $ENV{ART_RESOURCE_PATH} STREQUAL "" )
	include_directories ($ENV{ART_RESOURCE_PATH}/arm2art/include)
  else ()
	include_directories (/Library/ART_Resources/arm2art/include)	
	include_directories (~/Library/ART_Resources/arm2art/include)
  endif ()
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE -DART_4_OPENSTEP" )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GNU_SOURCE -DART_4_OPENSTEP" )
else ( APPLE )
  if ( NOT $ENV{ART_RESOURCE_PATH} STREQUAL "" )
	include_directories ($ENV{ART_RESOURCE_PATH}/arm2art/include)
  else ()
	include_directories (lib/ART_Resources/arm2art/include)
	include_directories (~/lib/ART_Resources/arm2art/include)
  endif ()
  
  if ( GNUSTEP_CONFIG )
	exec_program ( gnustep-config
	  ARGS "--objc-flags"
	  OUTPUT_VARIABLE COMPILE_FLAGS
	  )
	exec_program ( gnustep-config
	  ARGS "--objc-libs"
	  OUTPUT_VARIABLE LINKER_FLAGS
	  )
	
	set( CMAKE_C_FLAGS    "${CMAKE_C_FLAGS}    -DART_4_GNUSTEP ${COMPILE_FLAGS}" )
	set( CMAKE_OBJC_FLAGS "${CMAKE_OBJC_FLAGS} -DART_4_GNUSTEP ${COMPILE_FLAGS}" )
	set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}  -DART_4_GNUSTEP" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKER_FLAGS}" )
  else ( GNUSTEP_CONFIG )
	set( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -D_GNU_SOURCE -DART_4_GNUSTEP -O3" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GNU_SOURCE -DART_4_GNUSTEP -D_NATIVE_OBJC_EXCEPTIONS=1 -O3" )
	
	# GNUstep include directories. There is a number of places where these can be,
	# currently we just add them all.
	
	include_directories (/usr/GNUstep/Local/Library/Headers)
	include_directories (/usr/include/GNUstep)
	include_directories (/usr/lib64/GNUstep/Headers)
  endif( GNUSTEP_CONFIG)
endif ( APPLE )

include_directories (/usr/local/include)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wno-unused -D_ART_WITHOUT_JPEGLIB_" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused -D_ART_WITHOUT_JPEGLIB_" )
set( CMAKE_CXX_COMPILER gcc )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake_modules/")

find_package( TIFF REQUIRED )
find_package( LCMS2 REQUIRED )

if ( APPLE )
  if ( NOT $ENV{ART_LIBRARY_PATH} STREQUAL "" )
	find_library(
	  art_framework
	  NAME
	    AdvancedRenderingToolkit
	  PATHS
	    $ENV{ART_LIBRARY_PATH}
	  CMAKE_FIND_FRAMEWORK
	  ONLY
	  )
  else ()
	
	find_library(
	  art_framework
	  NAME
	    AdvancedRenderingToolkit
	  PATHS
	    /Library/Frameworks/
   	    ~/Library/Frameworks/
	  CMAKE_FIND_FRAMEWORK
	  ONLY
	  )
  endif ()
  
	MARK_AS_ADVANCED(
		art_framework
		)

	find_library(
		cocoa_framework
		NAME
			Cocoa
		PATHS
			/System/Library/Frameworks/
		CMAKE_FIND_FRAMEWORK
			ONLY
		)

	MARK_AS_ADVANCED(
		cocoa_framework
		)

	set (
		art_link_libraries 
		${art_framework}
		${cocoa_framework}
		${TIFF_LIBRARY}
		${LCMS2_LIBRARY}
		m
		)

else ( APPLE )
  if ( NOT $ENV{ART_LIBRARY_PATH} STREQUAL "" )
	link_directories ($ENV{ART_LIBRARY_PATH})
  else ()
	link_directories (/usr/local/lib)
	link_directories (/usr/lib)
	link_directories (~/lib)
  endif ()


	set (
		art_link_libraries
		AdvancedRenderingToolkit
		gnustep-base
		${TIFF_LIBRARY}
		${LCMS2_LIBRARY}
		objc
		m
		)

endif ( APPLE )

###############################
# Project sources
###############################


add_executable(
  export_spectrum
  src/export_spectrum.m
  )

target_link_libraries(
  export_spectrum
  ${art_link_libraries}
  )

install (
  TARGETS
    export_spectrum
  DESTINATION
    bin
  )
