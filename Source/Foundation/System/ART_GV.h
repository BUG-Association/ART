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

#ifndef _ART_FOUNDATION_SYSTEM_GLOBAL_VARIABLES_H_
#define _ART_FOUNDATION_SYSTEM_GLOBAL_VARIABLES_H_

/* ---------------------------------------------------------------------------

    'ART_GV' struct
    ----------------------------

    This is the top-level location where any ART module can store variable
    information that has to be globally accessible for a given, running
    instance of ART.

    Apart from this struct, there are no genuine global *variables* in ART,
    only global *constants* (even if some are defined as variables for
    technical reasons, but these should never be modified). Two instances of
    ART can cleanly co-exist in the same memory space if each of them has
    their own copy of this global variable struct, and this also ensures
    that ART can be safely run from/as a shared library (irrespective of the
    memory/linkage model used).

    There is exactly one instance of this type for each running ART instance,
    the components of which are successively initialised during the library
    and module initialisation phase, and de-activated in reverse order during
    module shutdown.

    The only functions that are allowed to create the components of this
    struct are the canonical module startup functions, and the only ones who
    are allowed (and expected) to destroy its contents are the canonical module
    shutdown functions.

    Note that this "create" and "destroy" caveat only applies to the entire
    module-level sub-structures of ART_GV listed below. The individual modules
    can of course access and modfiy the contents of "their" structures at their
    own discretion.

    With the exception of the first entry, the expected type of entry in this
    struct is a forward pointer reference to a module-specific struct type,
    that is only later declared by the module that requires global variables.

    IMPORTANT: what kind of global variables belong here
    ----------------------------------------------------

    As a general rule, global variables that are absolutely guaranteed to be
    read-only (i.e. global constants) should NOT be members of this struct.
    Such variables just have to be protected against multiple initialisations,
    like e.g. the CRC32 tables found in the ART_CRC32 module, but can
    otherwise be implemented as normal global variables.

    This struct is intended for all those global variables that represent some
    element of the global state of a running instance of ART, and which can
    and will be modified by the running system. Classical examples are the
    state of the basic, global random number generator that is used to
    bootstrap all other random generator functionality in a platform-independent
    manner (i.e. independent of the system random generator), the global
    table of ArSymbols, or the state of the ArSpectrum subsystem.

    NOTE #1:
    --------

    All the global variables defined in this struct require protection against
    uncoordinated manipulation by concurrent threads. Most of them will never
    be directly visible to these threads (see note #2), so the mutex logic only
    has to be built into the module that defines the variables, and specifically
    the functions that manipulate the variables. See the ArSymbol module for an
    example of this; the global ArSymbol table is protected against
    inconsistencies by a mutex that only allows one calling thread at a time to
    add an entry to the table.

    NOTE #2:
    --------

    Note that the modules need and should not expose the actual declaration of
    the module-specific "global variable struct"! In most cases, all functions
    that are ever expected and required to access the module-specific part of
    the struct are within the module itself, so no external declaration has to
    be given. Especially since the only place where an instance of this module-
    specific "global variable struct" will be dynamically allocated is within
    the initialisation routine of the module itself.

    NOTE #3:
    --------

    Even though ART provides this clear, strutured and safe mechanism for
    using "global" variables within a running instance of the toolkit,
    chances are that you will *not* need this feature when adding
    functionality to ART.

    Comparatively few modules actually require something like a "system-wide"
    global variable (especially higher-level modules above ART_Foundation),
    but if you think you absolutely have to add one to the list - well, here
    is the place to do it.

    But be warned - from a design viewpoint, you are probably still wrong
    about needing that global variable...

------------------------------------------------------------------------aw- */

//   For reasons of clarity, the individual struct types and entries should
//   have canonical names: the type should be <module name>_GV, and the
//   entry <module name in lowercase>_gv

typedef struct ART_GV
{
    unsigned int                          moduleManagementVerbosity;
    
    //   0..9
    struct ART_EnvironmentVariables_GV  * art_environmentvariables_gv;
    struct ArRandom_GV                  * arrandom_gv;
    struct ArSymbol_GV                  * arsymbol_gv;
    struct ArSpectrum_GV                * arspectrum_gv;
    struct ArSpectralSample_GV          * arspectralsample_gv;
    struct ArCIExy_GV                   * arciexy_gv;
    struct ArCIExyY_GV                  * arciexyy_gv;
    struct ArCIEXYZ_GV                  * arciexyz_gv;
    struct ArCIELab_GV                  * arcielab_gv;
    struct ArCIELuv_GV                  * arcieluv_gv;

    //   10..19
    struct ArCIEColourConversions_GV    * arciecolourconversions_gv;
    struct ArColourSpace_GV             * arcolourspace_gv;
    struct SystemWhitepoint_GV          * systemwhitepoint_gv;
    struct ArCMF_GV                     * arcmf_gv;
    struct ArGrey_GV                    * argrey_gv;
    struct ArRGB_GV                     * arrgb_gv;
    struct ArRGBA_GV                    * arrgba_gv;
    struct ArSpectrum8_GV               * arspectrum8_gv;
    struct ArSpectrum11_GV              * arspectrum11_gv;
    struct ArSpectrum18_GV              * arspectrum18_gv;
    
    //   20..29
    struct ArSpectrum46_GV              * arspectrum46_gv;
    struct ArSpectrum500_GV             * arspectrum500_gv;
    struct ArCrosstalk_GV               * arcrosstalk_gv;
    struct ArLight_GV                   * arlight_gv;
    struct ArLightAlpha_GV              * arlightalpha_gv;
    struct ArSVLight_GV                 * arsvlight_gv;
    struct ArDirectAttenuation_GV       * ardirectattenuation_gv;
    struct ArMMDirectAttenuation_GV     * armmdirectattenuation_gv;
    struct ArAttenuation_GV             * arattenuation_gv;
    struct ArLightSample_GV             * arlightsample_gv;
    
    //   30..39
    struct ArLightAlphaSample_GV          * arlightalphasample_gv;
    struct ArAttenuationSample_GV         * arattenuationsample_gv;
    struct ArSVLightSample_GV             * arsvlightsample_gv;
    struct ArDirectAttenuationSample_GV   * ardirectattenuationsample_gv;
    struct ArMMDirectAttenuationSample_GV * armmdirectattenuationsample_gv;
    struct ArCLS_GV                       * arcls_gv;
    struct UpliftCoefficientCube_GV       * upliftcoefficientcube_gv;
    struct ArSingleton_GV                 * arsingleton_gv;
    struct ArcFileProbe_GV                * arcfileprobe_gv;
    struct ArNodeOperation_GV             * arnodeoperation_gv;

    //   40..49
    struct ExecutionEnvironment_GV      * executionenvironment_gv;
    struct ArcOption_GV                 * arcoption_gv;
    struct ArnColour_GV                 * arncolour_gv;
    struct ArnColourSpace_GV            * arncolourspace_gv;
    struct ArnLightsourceCollection_GV  * arnlightsourcecollection_gv;
    struct ArcComplexSkydomeLightsource_GV  * arccomplexskydomelightsource_gv;
    struct ArnCone_GV                   * arncone_gv;
    struct ArnCube_GV                   * arncube_gv;
    struct ArnCylinder_GV               * arncylinder_gv;
    struct ArnHyperboloid_GV            * arnhyperboloid_gv;

    //   50..59
    struct ArnInfSphere_GV              * arninfsphere_gv;
    struct ArnParaboloid_GV             * arnparaboloid_gv;
    struct ArnSphere_GV                 * arnsphere_gv;
    struct ArnTriangleMesh_GV           * arntrianglemesh_gv;
    struct ArnHTSGSurface_GV            * arnhtsgsurface_gv;
    struct ArnHosekSkyModel_GV          * arnhosekskylight_gv;
    struct ArnPreethamSkyModel_GV       * arnpreethamskymodel_gv;
    struct ArnHNSVolumeMaterial_GV      * arnhnsvolumematerial_gv;
    struct ART_DefaultSurfaceMaterial_GV
           * art_defaultsurfacematerial_gv;
    struct ART_DefaultEmissiveSurfaceMaterial_GV
           * art_defaultemissivesurfacematerial_gv;

    //   60..66
    struct ART_DefaultEnvironmentMaterial_GV
           * art_defaultenvironmentmaterial_gv;
    struct ART_DefaultVolumeMaterial_GV
           * art_defaultvolumematerial_gv;
    struct ARM_Actions_GV               * ar2m_actions_gv;
    struct ARM_ImageSamplers_GV         * ar2m_imagesamplers_gv;
    struct ARM_RayCasting_GV            * ar2m_raycasting_gv;
    struct ARM_ScenegraphActions_GV     * ar2m_scenegraphactions_gv;
    struct ApplicationSupport_GV        * application_support_gv;
}
ART_GV;

void art_gv_initialise(
        ART_GV  * art_gv
        );

#endif /* _ART_FOUNDATION_SYSTEM_GLOBAL_VARIABLES_H_ */
/* ======================================================================== */
