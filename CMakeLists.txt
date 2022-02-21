cmake_minimum_required (VERSION 3.0)

project (ART)

set ( ART_VERSION_MAJOR 2 )
set ( ART_VERSION_MINOR 1 )
set ( ART_VERSION_PATCH 0 )
set ( ART_VERSION_DEVTAG -gamutmapping )

if ( APPLE )
set(CMAKE_MACOSX_RPATH 1)
endif ( APPLE )

set(CMAKE_CXX_STANDARD 11)

# configure a header file to pass some of the CMake settings
# to the source code

configure_file (
  "${PROJECT_SOURCE_DIR}/Source/Foundation/System/ART_Version_Numbers.h.in"
  "${PROJECT_BINARY_DIR}/ART_Version_Numbers.h"
  )

# enable deprecated actions: those actions are still there for backward
# compatibility. At the time of writing this comment, they handle 
# ARTRAWs and Spectral OpenEXRs but were named *ARTRAW* and changed
# to *RAW* after adding the support for Spectral OpenEXRs.
add_definitions(-DENABLE_DEPRECATED_ACTIONS)

include_directories("${PROJECT_BINARY_DIR}")

message("")
message("Configuring the Advanced Rendering Toolkit - ART")
message("")
message("Version is " ${ART_VERSION_MAJOR} "." ${ART_VERSION_MINOR} "." ${ART_VERSION_PATCH} ${ART_VERSION_DEVTAG} )
message("")

#debug
#if ( APPLE )
#  message("Platform: APPLE")
#elseif ( UNIX )
#  message("Platform: UNIX")
#else ( APPLE )
#  message("Unknown platform...")
#endif ( APPLE )

find_package( TIFF REQUIRED )

# Try to find the OpenEXR native cmake package config
find_package(Imath CONFIG)
find_package(OpenEXR CONFIG)

if (OpenEXR_FOUND)
	message("Native OpenEXR 3.0 configure")

	set(OPENEXR_LIBRARIES 
		OpenEXR::OpenEXR
		OpenEXR::IlmThread
		OpenEXR::Iex
		Imath::Imath
		)

	SET(OPENEXR_FOUND 1)
endif()

# Now, activate our custom modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake_modules/")

# For older version of OpenEXR, we need to use our CMake module
if (NOT OpenEXR_FOUND)
	message("OpenEXR 3.0 not found, trying older releases")

	find_package( OPENEXR )
endif()

find_package( LCMS2 REQUIRED )
#find_package( JPEG REQUIRED )
find_package( GSL )

if ( NOT APPLE )
    find_package( GNUSTEP-BASE REQUIRED )
endif()

## debug - DELETE THIS eventually!
message("")
message("Directory information, useful in case anything goes wrong:")
message("")
message("ART source dir:        ${PROJECT_SOURCE_DIR}")
message("ART binary dir:        ${PROJECT_BINARY_DIR}")
message("")
message("TIFF include dir:      ${TIFF_INCLUDE_DIR}")
message("TIFF library:          ${TIFF_LIBRARY}")
message("JPEG include dir:      ${JPEG_INCLUDE_DIR}")
message("JPEG library:          ${JPEG_LIBRARY}")
message("Lcms2 include dir:     ${LCMS2_INCLUDE_DIR}")
message("Lcms2 library:         ${LCMS2_LIBRARY}")
if ( NOT APPLE )
    message("GNUstep include dir:   ${GNUSTEP-BASE_INCLUDE_DIR}")
    message("GNUstep library:       ${GNUSTEP-BASE_LIBRARY}")
endif ( NOT APPLE )
message("GSL_FOUND:             ${GSL_FOUND}")
message("GSL include dir:       ${GSL_INCLUDE_DIR}")
message("GSL library:           ${GSL_LIBRARY}")
message("OPENEXR_FOUND:         ${OPENEXR_FOUND}")
message("OPENEXR include dir:   ${OPENEXR_INCLUDE_DIR}")
message("OPENEXR library:       ${OPENEXR_LIBRARIES}")
message("")


# configure one more header file to pass some of the CMake settings
# to the source code

if ( GSL_FOUND )
    set ( GSL_FOUND_INT 1 )
else ( GSL_FOUND )
    set ( GSL_FOUND_INT 0 )
endif ( GSL_FOUND )

configure_file (
  "${PROJECT_SOURCE_DIR}/Source/Foundation/LightAndAttenuation/ArMuellerMatrix_GSLSettings.h.in"
  "${PROJECT_BINARY_DIR}/Source/Foundation/LightAndAttenuation/ArMuellerMatrix_GSLSettings.h"
  )

include_directories (
  "${PROJECT_BINARY_DIR}/Source/Foundation/LightAndAttenuation/"
)

if ( OPENEXR_FOUND )
    set ( OPENEXR_FOUND_INT 1 )
else ( OPENEXR_FOUND )
    set ( OPENEXR_FOUND_INT 0 )
endif ( OPENEXR_FOUND )

configure_file (
  "${PROJECT_SOURCE_DIR}/Source/ClassLibraries/ImageFileFormat/OpenEXRSettings.h.in"
  "${PROJECT_BINARY_DIR}/Source/ClassLibraries/ImageFileFormat/OpenEXRSettings.h"
  )

include_directories (
  "${PROJECT_BINARY_DIR}/Source/ClassLibraries/ImageFileFormat/"
)


# We specify where stuff will end up being copied to

# Executables go to bin

set( art_executable_directory "bin")

message("")
message("Executables will be installed in '${art_executable_directory}'")

# Miscellaneous resources go to ~/ART_Resources, or ~/Library/ART_Resources
# (depending on platform)

if ( APPLE )
	if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin" 
    		AND CMAKE_HOST_SYSTEM_VERSION VERSION_GREATER_EQUAL 20
# leaving this here in case we ever need to be specific for MacOS 11, saves me having to google this again
#    		AND CMAKE_HOST_SYSTEM_VERSION VERSION_LESS 21
		)
    		message("Building for MacOS 11 or later, activating code signing")
		set(CMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "-o linker-signed")
	endif ()

	set( art_resource_directory_location "Library" )
	set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE -DART_4_OPENSTEP -D_ART_WITHOUT_JPEGLIB" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GNU_SOURCE -DART_4_OPENSTEP -D_ART_WITHOUT_JPEGLIB" )
#	set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE -DART_4_OPENSTEP -D_ART_WITHOUT_JPEGLIB_ -O3" )
#	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GNU_SOURCE -DART_4_OPENSTEP -D_ART_WITHOUT_JPEGLIB_ -O3" )
else ( APPLE )
	set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE -DNDEBUG -DART_4_GNUSTEP -O3" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GNU_SOURCE -DNDEBUG -DART_4_GNUSTEP -D_NATIVE_OBJC_EXCEPTIONS=1 -O3" )
	set( art_resource_directory_location "lib" )

if ( CYGWIN )
	set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCYGWIN" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCYGWIN" )
endif ( CYGWIN )

endif ( APPLE )

message("ART_Resources will be installed in '${art_resource_directory_location}'")

set( art_resource_directory_name "ART_Resources" )

set(
	art_header_directory_location
	"${art_resource_directory_location}/${art_resource_directory_name}/arm2art"
	)

set(
	art_library_directory
	"${art_resource_directory_location}/${art_resource_directory_name}/arm2art/lib"
	)

message("ART headers will be installed in '${art_header_directory_location}/include'")

# Global flags for the make process

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wno-unused -D_ART_WITHOUT_JPEGLIB_" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused -D_ART_WITHOUT_JPEGLIB_" )
set( CMAKE_CXX_COMPILER gcc )

# GNUstep include directories.

#include_directories (/usr/GNUstep/Local/Library/Headers)
#include_directories (/usr/GNUstep/System/Library/Headers)
#include_directories (/usr/include/GNUstep)
#include_directories (/usr/lib64/GNUstep/Headers)
#include_directories (/usr/local/include)
include_directories (${TIFF_INCLUDE_DIR})
#include_directories (${JPEG_INCLUDE_DIR})
include_directories (${LCMS2_INCLUDE_DIR})
if ( GSL_FOUND )
    include_directories (${GSL_INCLUDE_DIR})
endif ( GSL_FOUND )
if ( OPENEXR_FOUND )
    message("EXR include dir is '${OPENEXR_INCLUDE_DIR}'")
    include_directories (${OPENEXR_INCLUDE_DIR})    
    include_directories (${OPENEXR_INCLUDE_DIR}/OpenEXR)
endif ( OPENEXR_FOUND )
include_directories (${GNUSTEP-BASE_INCLUDE_DIR})

# All .h files from the entire toolkit are gathered in one place. arm2art translation
# needs this anyway, and it makes the actual ART build possible without a major circus
# w/r to the include paths.

set( art_internal_include_dir ${CMAKE_CURRENT_BINARY_DIR}/include )

file( MAKE_DIRECTORY ${art_internal_include_dir} )

message("Internal header directory is '${art_internal_include_dir}'")

#include_directories ( ${art_internal_include_dir} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/Source
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Foundation
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Foundation/ColourAndSpectra
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Foundation/Datastructures
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Foundation/Geometry
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Foundation/LightAndAttenuation
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Foundation/Math
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Foundation/ObjectiveC
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Foundation/Physics
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Foundation/System
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/ARM_Interface
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/ActionSequence
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/Camera
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/ColourAndSpectra
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/EmissiveSurfaceMaterial
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/EnvironmentMaterial
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/Expression
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/ImageAction
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/ImageData
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/ImageFileFormat
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/ImageSampler
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/Lightsource
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/MiscellaneousStructs
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/MiscellaneousClasses
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/Parser
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/PathspaceIntegrator
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/PhaseFunction
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/Physics
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/Protocols
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/RayCasting
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/RayCastingAcceleration
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/Scenegraph
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/Shape
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/SkyModel
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/SurfaceMaterial
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/Trafo
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/VolumeData
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/VolumeIntegrator
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/VolumeMaterial
  )

# We place the library we build in a subdirectory of the source tree that we create for 
# the occasion. It will get installed to a different place, but for building purposes, it
# goes there first.

set( art_internal_library_dir ${CMAKE_CURRENT_BINARY_DIR}/lib )

file( MAKE_DIRECTORY ${art_internal_library_dir} )

message("Internal library build directory is '${art_internal_library_dir}'")


# Next, we gather all .h, .c and .m files from the ART_Foundation and ART_Library source
# sub-trees. We combine them into top-level variables - one for the headers, one for the
# sources, and copy them all to the header and source directories created in the previous
# step.

# First, the headers:

# find them...

file ( GLOB_RECURSE art_foundation_headers "${CMAKE_CURRENT_SOURCE_DIR}/Source/Foundation/*.h" )
file ( GLOB_RECURSE art_library_headers "${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/*.h")

# ...combine them into one variable...

set (
	art_header_files 
	${art_foundation_headers} 
	${art_library_headers}
	${CMAKE_CURRENT_SOURCE_DIR}/Source/AdvancedRenderingToolkit.h
	"${PROJECT_BINARY_DIR}/Source/Foundation/LightAndAttenuation/ArMuellerMatrix_GSLSettings.h"
	"${PROJECT_BINARY_DIR}/Source/ClassLibraries/ImageFileFormat/OpenEXRSettings.h"
	)

# ...and copy the whole lot.

foreach( header ${art_header_files} )
	configure_file( 
		${header} 
		${art_internal_include_dir} 
		COPYONLY
		)
endforeach( header ${art_header_files} )

# Now we also do this for the sources - both .c and .m:

# Again, find...

file ( GLOB_RECURSE art_foundation_c_sources "${CMAKE_CURRENT_SOURCE_DIR}/Source/Foundation/*.c" )
file ( GLOB_RECURSE art_foundation_m_sources "${CMAKE_CURRENT_SOURCE_DIR}/Source/Foundation/*.m" )
file ( GLOB_RECURSE art_library_c_sources "${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/*.c" )
file ( GLOB_RECURSE art_library_cpp_sources "${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/*.cpp" )
file ( GLOB_RECURSE art_library_m_sources "${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/*.m" )
file ( GLOB_RECURSE art_library_mm_sources "${CMAKE_CURRENT_SOURCE_DIR}/Source/ClassLibraries/*.mm" )

# ...combine...

set (
	art_sources 
	${art_foundation_c_sources} 
	${art_foundation_m_sources} 
	${art_library_c_sources}
	${art_library_cpp_sources}
	${art_library_m_sources}
	${art_library_mm_sources}
	${CMAKE_CURRENT_SOURCE_DIR}/Source/AdvancedRenderingToolkit.m
	)

# Next major thing: tell CMake what external libraries we will be linking against:

if ( APPLE )
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
		art_other_link_libraries 
		${cocoa_framework}
		${TIFF_LIBRARY}
#                ${JPEG_LIBRARY}
		${LCMS2_LIBRARY}
		m
		)

else ( APPLE )

	set (
		art_other_link_libraries 
		${GNUSTEP-BASE_LIBRARY}
		${TIFF_LIBRARY}
        ${JPEG_LIBRARY}
		${LCMS2_LIBRARY}
		objc
		m
		)

endif ( APPLE )

if ( GSL_FOUND )

    set (
        art_other_link_libraries
        ${art_other_link_libraries}
        ${GSL_LIBRARY}
        )

endif ( GSL_FOUND )

if ( CYGWIN )

    set (
        art_other_link_libraries
        ${art_other_link_libraries}
        stdc++
        )

endif ( CYGWIN )

if ( OPENEXR_FOUND )

    set (
        art_other_link_libraries
        ${art_other_link_libraries}
        ${OPENEXR_LIBRARIES}
        )

endif ( OPENEXR_FOUND )


set (
	art_generic_link_libraries 
	AdvancedRenderingToolkit
	${art_other_link_libraries}
	)

# Copy the 'CMakeList' we provide for the 'include' and 'src' directories:

configure_file( 
	Source/CMakeLists_ART_include.txt 
	${art_internal_include_dir}/CMakeLists.txt
	COPYONLY 
	)

set( source_group_file "${art_internal_library_dir}/sgf.txt" )

file( WRITE ${source_group_file} "\n" )

add_subdirectory (Source/Foundation) 
add_subdirectory (Source/ClassLibraries)

file (
    GLOB
    library_sources
		"*.[h|c|m|mm]"
		"*.cpp"
    )

file( APPEND ${source_group_file} "source_group(\n" )
file( APPEND ${source_group_file} "	\"\"\n")
file( APPEND ${source_group_file} "	FILES\n")

foreach( src_file ${library_sources} )
	file( APPEND ${source_group_file} "	${src_file}\n")
endforeach()

file( APPEND ${source_group_file} "	)\n\n")

file( READ ${source_group_file} source_group_commands )

configure_file( 
	Source/CMakeLists_ART_library.txt 
	${art_internal_library_dir}/CMakeLists.txt
	@ONLY 
	)

# On installation, we create/modify the ART_Resource directory, wherever it ends up being
# This has to be done first, because the library and the headers get placed there later.

install (
	DIRECTORY
		"ART_Resources"
	DESTINATION
		${art_resource_directory_location}
	PATTERN ".svn" EXCLUDE
	PATTERN ".DS_Store" EXCLUDE
	)

# Copy the headers to the resource dir

add_subdirectory (${art_internal_include_dir} ${art_internal_include_dir})


# Build the library

add_subdirectory (${art_internal_library_dir} ${art_internal_library_dir})


message("")

# Build the executables

add_subdirectory ("Source/CommandLineTools")
