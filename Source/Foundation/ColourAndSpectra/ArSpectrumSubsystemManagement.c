/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
    ------------------------------------------------

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

#include "ArSpectrumSubsystemManagement.h"

#include "_ArSpectrum_GV.h"

#include "ArRGB.h"
#include "ArRGBA.h"
#include "ArSpectrum.h"

#include "SpectralDatatype_InterfaceMacros.h"

SWITCHING_INTERFACE_FOR_ISR( ArUT_RGB, ut_rgb );
SWITCHING_INTERFACE_FOR_ISR( ArSpectrum8, s8 );
SWITCHING_INTERFACE_FOR_ISR( ArSpectrum11, s11 );
SWITCHING_INTERFACE_FOR_ISR( ArSpectrum18, s18 );
SWITCHING_INTERFACE_FOR_ISR( ArSpectrum46, s46 );

void art_foundation_set_isr(
        ART_GV          * art_gv,
        ArSpectrumType    isr
        )
{
    switch (isr)
    {
        case arspectrum_ut_rgb:
        case arspectrum_ut_rgb_polarisable:
            switch_isr_to_ut_rgb( art_gv );
            art_gv->arspectrum_gv->current_isr = isr;
            break;

        case arspectrum_spectrum8:
        case arspectrum_spectrum8_polarisable:
            switch_isr_to_s8( art_gv );
            art_gv->arspectrum_gv->current_isr = isr;
            break;

        case arspectrum_spectrum11:
        case arspectrum_spectrum11_polarisable:
            switch_isr_to_s11( art_gv );
            art_gv->arspectrum_gv->current_isr = isr;
            break;

        case arspectrum_spectrum18:
        case arspectrum_spectrum18_polarisable:
            switch_isr_to_s18( art_gv );
            art_gv->arspectrum_gv->current_isr = isr;
            break;

        case arspectrum_spectrum46:
        case arspectrum_spectrum46_polarisable:
            switch_isr_to_s46( art_gv );
            art_gv->arspectrum_gv->current_isr = isr;
            break;

        default:
            ART_ERRORHANDLING_FATAL_ERROR(
                "invalid ISR"
                );
            break;
    }
}

ArSpectrumType  art_foundation_isr(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arspectrum_gv->current_isr & ~arspectrum_polarisable;
}

void art_foundation_initialise_spectral_subsystem(
        ART_GV  * art_gv
        )
{
    INITIALISE_COLOUR_MODULE( ArRGB );
    INITIALISE_COLOUR_MODULE( ArRGBA );
    INITIALISE_COLOUR_MODULE( ArSpectrum );
}

/* ======================================================================== */
