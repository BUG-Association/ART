/* ===========================================================================

    Copyright (c) The ART Development Team
    --------------------------------------

    For a comprehensive list of the members of the development team, and a
    description of their respective contributions, see the file
    "ART_DeveloperList.txt" that is distributed with the libraries.

    This file is part of the Advanced Rendering Toolkit (ART) libraries.

    ART is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, either version 3 of the License, or (at your option) any
    later version.

    ART is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with ART.  If not, see <http://www.gnu.org/licenses/>.

=========================================================================== */
/**
 * @file ARM_SkyModel.h
 * @brief Sky models
 * @type SkyModel
 */

#import "ARM_Foundation.h"

ART_MODULE_INTERFACE(ARM_SkyModel)

#import "ArnPreethamSkyModel.h"
#import "ArnHosekSkyModel.h"
#import "ArnPragueSkyModel.h"

@interface ArnPreethamSkyModel ( ARM_Interface )

- (id) elevation
                 : (double) newElevation
        azimuth  : (double) newAzimuth
        turbidity: (double) newTurbidity
        ;

- (id) elevation
                 : (double) newElevation
        azimuth  : (double) newAzimuth
        turbidity: (double) newTurbidity
        polarised: (BOOL)   newPolarised
        ;

@end

@interface ArnHosekSkyModel ( ARM_Interface )

- (id) elevation
                    : (double) newElevation
        azimuth     : (double) newAzimuth
        turbidity   : (double) newTurbidity
        groundAlbedo: (ArNode <ArpSpectrum> *) newGroundAlbedo
        ;

- (id) elevation
                    : (double) newElevation
        azimuth     : (double) newAzimuth
        solColour   : (double) newKelvin
        solIntensity: (double) newSolIntensity
        turbidity   : (double) newTurbidity
        groundAlbedo: (ArNode <ArpSpectrum> *) newGroundAlbedo
        ;

//   The trafo parameter in the second version can be used to rotate the entire
//   skylight around an axis. This can come in handy if an entire scene is
//   already specified in a coordinate system where positive Z is not "up".

- (id) elevation
                    : (double) newElevation
        azimuth     : (double) newAzimuth
        turbidity   : (double) newTurbidity
        groundAlbedo: (ArNode <ArpSpectrum> *) newGroundAlbedo
        trafo       : (ArNode <ArpTrafo3D> *) newTrafo
        ;

@end

@interface ArnPragueSkyModel ( ARM_Interface )

- (id) elevation
                    : (double) newElevation
        azimuth     : (double) newAzimuth
        turbidity   : (double) newTurbidity
        groundAlbedo: (ArNode <ArpSpectrum> *) newGroundAlbedo
        ;

@end

#define DIFFUSE_INFINITESPHERE \
    ALLOC_INIT_OBJECT_AUTORELEASE(ArnSkyModel)

#define PREETHAM_SKYLIGHT \
    ALLOC_OBJECT_AUTORELEASE(ArnPreethamSkyModel)

#define PRAGUE_SKYLIGHT \
    ALLOC_OBJECT_AUTORELEASE(ArnPragueSkyModel)

/**
 * @brief Hosek sky model
 *
 * This is a physically-based analytical model of the daytime sky presented
 * by Hosek et Wilkie \ref{HW12AAMFFSSDR}. More information is available on
 * \url{http://cgg.mff.cuni.cz/projects/SkylightModelling/}.
 *
 * @artist SkyModel.arm -DHOSEK_SKYLIGHT_ -DTURB=2.0 -DAZIM=0 -DELEV=2
 * @artist SkyModel.arm -DHOSEK_SKYLIGHT_ -DTURB=2.0 -DAZIM=0 -DELEV=20
 * @artist SkyModel.arm -DHOSEK_SKYLIGHT_ -DTURB=2.0 -DAZIM=0 -DELEV=90
 *
 * @def [ HOSEK_SKYLIGHT elevation: azimuth: turbidity: groundAlbedo: ]
 *
 * @param elevation     double      Star's elevation (specify the unit with \verb?RADIANS? or \verb?DEGREES?).
 * @param azimuth       double      Star's azimuth (specify the unit with \verb?RADIANS? or \verb?DEGREES?).
 * @param turbidity     double      Atmosphere turbidity.
 * @param groundAlbedo  Spectrum    Specify the albedo for the planet's soil.
 *
 * @def [ HOSEK_SKYLIGHT
 * @fdef    elevation: azimuth: solColour: solIntensity: turbidity: groundAlbedo: ]
 *
 * @param elevation     double      Star's elevation (specify the unit with \verb?RADIANS? or \verb?DEGREES?).
 * @param azimuth       double      Star's azimuth (specify the unit with \verb?RADIANS? or \verb?DEGREES?).
 * @param solColour     Spectrum    Star's emission spectrum.
 * @param solIntensity  double      Intensity of the sun emission.
 * @param turbidity     double      Atmosphere turbidity.
 * @param groundAlbedo  Spectrum    Specify the albedo for the planet's soil.
 *
 * @def [ HOSEK_SKYLIGHT
 * @fdef    elevation: azimuth: turbidity: groundAlbedo: trafo: ]
 *
 * The trafo parameter in the second version can be used to rotate the entire
 * skylight around an axis. This can come in handy if an entire scene is
 * already specified in a coordinate system where positive Z is not "up".
 *
 * @param elevation     double      Star's elevation (specify the unit with \verb?RADIANS? or \verb?DEGREES?).
 * @param azimuth       double      Star's azimuth (specify the unit with \verb?RADIANS? or \verb?DEGREES?).
 * @param turbidity     double      Atmosphere turbidity.
 * @param groundAlbedo  Spectrum    Specify the albedo for the planet's soil.
 * @param trafo         Trafo       Rotation to apply to the sky.
 */
#define HOSEK_SKYLIGHT \
    ALLOC_OBJECT_AUTORELEASE(ArnHosekSkyModel)

/**
 * @brief Hosek sky model with two stars
 *
 * This is similar to the standard Hosek model, except that two solar discs
 * are generated, and that the combined skydome due to both of them is
 * computed and used. When they overlap, sun A is assumed to occlude sun B.
 *
 * Note that no radii/diameters can be directly specified for the two suns.
 * Instead, these follow directly from the overall irradiance, the emission
 * spectra of the suns, and the intensity ratio between the two that was
 * requested.
 *
 * @def [ HOSEK_BINARY_SKYLIGHT
 * @fdef    elevationA: azimuthA: solColourA:
 * @fdef    elevationB: azimuthB: solColourB:
 * @fdef    solIntensity: intensityOfA:
 * @fdef    turbidity: groundAlbedo: ]
 *
 * @param elevationA    double    Star A elevation (specify the unit with \verb?RADIANS? or \verb?DEGREES?).
 * @param azimuthA      double    Star A azimuth (specify the unit with \verb?RADIANS? or \verb?DEGREES?).
 * @param solColourA    double    Star A colour temperature (use \verb?KELVIN?).
 * @param elevationB    double    Star B elevation (specify the unit with \verb?RADIANS? or \verb?DEGREES?).
 * @param azimuthB      double    Star B azimuth (specify the unit with \verb?RADIANS? or \verb?DEGREES?).
 * @param solColourB    double    Star B colour temperature (use \verb?KELVIN?).
 * @param solIntensity  double    Overall intensity of both stars combined, compared to the solar irradiance on Earth. A value of 1.0 means that both stars together will be as bright as the sun on Earth, 1.2 would yield an irradiance of 120%, compared to Earth.
 * @param intensityOfA  double    Percent of the total intensity that will come from star A. The relative brightness of B will of course be '1 - intensityOfA'. So a value of 0.5 yields two equally bright suns (in terms of radiance on the ground), 0.33 yields an A that is half as bright as B etc. Since the differential radiance for a point on a solar disc is fixed for a given solar surface temperarture in Kelvin, the brightness has to be altered via changes to the size of the solar discs. So two equally intense suns of different temperature will be of different size!
 * @param turbidity     double    Atmosphere turbidity.
 * @param groundAlbedo  Spectrum  Specify the albedo for the planet's soil.
 */
#define HOSEK_BINARY_SKYLIGHT \
    ALLOC_OBJECT_AUTORELEASE(ArnHosekBinarySkyModel)

#define SIMPLE_SKYLIGHT \
    ALLOC_INIT_OBJECT_AUTORELEASE(ArnSimpleSkyModel)

#define CIE_SKYLIGHT \
    ALLOC_INIT_OBJECT_AUTORELEASE(ArnCIESkyModel)



// ===========================================================================
