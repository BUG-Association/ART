file (
    GLOB
    library_sources
    "*.[h|c|m|mm]"
    "*.cpp"
    )

string(REPLACE "/" ";" p2list "${CMAKE_CURRENT_SOURCE_DIR}")
list(REVERSE p2list)
list(GET p2list 0 first)
list(GET p2list 1 second)

file( APPEND ${source_group_file} "source_group(\n" )
file( APPEND ${source_group_file} "	\"${first}\"\n")
file( APPEND ${source_group_file} "	FILES\n")

foreach( src_file ${library_sources} )
	file( APPEND ${source_group_file} "	${src_file}\n")
endforeach()

file( APPEND ${source_group_file} "	)\n\n")

add_subdirectory (ARM_Interface) 
add_subdirectory (ActionSequence) 
add_subdirectory (Camera) 
add_subdirectory (ColourAndSpectra)
add_subdirectory (EmissiveSurfaceMaterial) 
add_subdirectory (EnvironmentMaterial) 
add_subdirectory (Expression) 
add_subdirectory (ImageAction) 
add_subdirectory (ImageData) 
add_subdirectory (ImageFileFormat) 
add_subdirectory (ImageSampler) 
add_subdirectory (Lightsource)
add_subdirectory (MiscellaneousStructs) 
add_subdirectory (MiscellaneousClasses) 
add_subdirectory (Parser) 
add_subdirectory (PathspaceIntegrator)
add_subdirectory (PhaseFunction)
add_subdirectory (Physics) 
add_subdirectory (Protocols) 
add_subdirectory (RayCasting) 
add_subdirectory (RayCastingAcceleration) 
add_subdirectory (Scenegraph) 
add_subdirectory (Shape) 
add_subdirectory (SkyModel) 
add_subdirectory (SurfaceMaterial) 
add_subdirectory (Trafo) 
add_subdirectory (VolumeData)
add_subdirectory (VolumeIntegrator)
add_subdirectory (VolumeMaterial) 
