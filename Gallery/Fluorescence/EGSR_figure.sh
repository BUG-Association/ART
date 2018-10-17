#!/bin/sh

common_properties="
	-res=256x256
	-DSAMPLES=256
	-DALG_MIS
	"

# Medium material selection
fluo_properties="
	-DFLUO_ABSORB=420
	-DFLUO_ABSORB_SPREAD=40
	-DFLUO_EMIT=550
	-DFLUO_EMIT_SPREAD=40
	-DFLUO_INTENSITY=0.025
	"

fluo_uv_absorb_properties="
	-DFLUO_ABSORB=360
	-DFLUO_ABSORB_SPREAD=40
	-DFLUO_EMIT=560
	-DFLUO_EMIT_SPREAD=40
	-DFLUO_INTENSITY=0.025
	"

fluo_teaser_properties="
	-DFLUO_ABSORB=360
	-DFLUO_ABSORB_SPREAD=40
	-DFLUO_EMIT=620
	-DFLUO_EMIT_SPREAD=80
	-DFLUO_INTENSITY=0.025
	"

# scene geometry properties
scene_geom_teaser_properties="
	-DPLAIN_BOX
	-DSMALL_LIGHT
	-DSPHERES
	"

scene_geom_two_lights_properties="
	-DUSE_SHADE
	"

# Medium selection
no_volume_properties="
	-DEMPTY
	"

volume_homo_properties="
	-DHOMOGENEOUS
	"

volume_smoke_properties="
	-DHETERO_SCALE=10
	"

volume_cube_properties="
	-DFLOATING_CUBE 
	"

# Teaser
artist Volume.arm \
	   ${common_properties} \
	   ${volume_homo_properties} \
	   ${fluo_teaser_properties} \
	   ${scene_geom_teaser_properties} \
	   -tt teaser

# Fluo volume & homogeneous
artist Volume.arm \
	   ${common_properties} \
	   ${volume_homo_properties} \
	   ${fluo_properties} \
	   -tt fluo_homo

artist Volume.arm \
	   ${common_properties} \
	   ${volume_smoke_properties} \
	   ${fluo_properties} \
	   -tt fluo_smoke

artist Volume.arm \
	   ${common_properties} \
	   ${volume_cube_properties} \
	   ${fluo_properties} \
	   -tt fluo_cube

# Scenes with two lights
artist Volume.arm \
	   ${common_properties} \
	   ${volume_homo_properties} \
	   ${fluo_uv_absorb_properties} \
	   ${scene_geom_two_lights_properties} \
	   -tt fluo_homo_two_lights
