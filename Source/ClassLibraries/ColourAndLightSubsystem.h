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

#include "ART_Foundation_ColourAndSpectra.h"

ART_MODULE_INTERFACE(ColourAndLightSubsystem)

void art_set_default_rgbspace_ref(
        ART_GV               * art_gv,
        ArColourSpace const  * newRef
        );

void art_set_isr(
        ART_GV          * art_gv,
        ArDataType    isr
        );

ArDataType  art_isr(
        ART_GV  * art_gv
        );

const ArWavelengthSamplingData  * art_isr_sampling_data(
        const ART_GV  * art_gv
        );

const ArSpectralSampleSplattingData  * art_isr_splatting_data(
        const ART_GV  * art_gv
        );

#define ART_CURRENT_ISR         art_isr(art_gv)
#define ART_WAVELENGTH_SAMPLING_DATA \
    art_isr_sampling_data(art_gv)
#define ART_SAMPLE_SPLATTING_DATA \
    art_isr_splatting_data(art_gv)
#define ART_SET_ISR(__c)        art_set_isr(art_gv,(__c))

// ===========================================================================
