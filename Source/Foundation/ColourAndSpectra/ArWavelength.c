/* ===========================================================================

    Copyright (c) 1996-2021 The ART Development Team
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

#define ART_MODULE_NAME     ArWavelength

#include "ArWavelength.h"
#include "ART_Foundation_Math.h"
#include "ART_Foundation_ColourAndSpectra.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

void arwavelength_sampling_data_from_current_ISR_s(
        const ART_GV                    * art_gv,
              ArWavelengthSamplingData  * sd
        )
{
    sd->spectralRangeStart = spc_channel_lower_bound( art_gv, 0 );
    sd->spectralRangeEnd =
        spc_channel_lower_bound(
            art_gv,
            spc_channels(art_gv) - 1
            )
        +
        spc_channel_width(
            art_gv,
            spc_channels(art_gv) - 1
            );
    
    sd->spectralRange = sd->spectralRangeEnd - sd->spectralRangeStart;
    sd->spectralRangeDiv4 = sd->spectralRange / 4.0;
}

void arwavelength_d_init_w(
        const ART_GV                    * art_gv,
        const double                      wavelength_0,
              ArWavelength              * wavelength_r
        )
{
    ARWL_WI( *wavelength_r, 0 ) = wavelength_0;
    ARWL_WI( *wavelength_r, 1 ) = wavelength_0;
    ARWL_WI( *wavelength_r, 2 ) = wavelength_0;
    ARWL_WI( *wavelength_r, 3 ) = wavelength_0;
}

void arwavelength_c_init_w(
        const ART_GV                    * art_gv,
        const Crd4                      * wavelengths,
              ArWavelength              * wavelength_r
        )
{
    ARWL_C(*wavelength_r) = *wavelengths;
}

void arwavelength_sd_init_w(
        const ART_GV                    * art_gv,
        const ArWavelengthSamplingData  * sd,
        const double                      unitRandomValue,
              ArWavelength              * wavelength_r
        )
{
    ASSERT_ALLOCATED_WAVELENGTH( wavelength_r )

    ARWL_WI( *wavelength_r, 0 ) =
        sd->spectralRangeStart + unitRandomValue * sd->spectralRange;
    
    for ( int i = 1; i < 4; i++ )
    {
        double  otherWL =
            ARWL_WI( *wavelength_r, 0 ) + i * sd->spectralRangeDiv4;
        
        if ( otherWL > sd->spectralRangeEnd )
        {
            otherWL -= sd->spectralRange;
        }
        
        ARWL_WI( *wavelength_r, i ) = otherWL;
    }

    ASSERT_VALID_WAVELENGTH( wavelength_r )
}

void arwavelength_i_deterministic_init_w(
        const ART_GV        * art_gv,
        const int             wavelengthStartIndex,
              ArWavelength  * wavelength_r
        )
{
    ASSERT_ALLOCATED_WAVELENGTH( wavelength_r )
    
    for ( int i = 0; i < 4; i++ )
    {
        ARWL_WI( *wavelength_r, i ) = spc_channel_center(art_gv, wavelengthStartIndex );
    }

    ASSERT_VALID_WAVELENGTH( wavelength_r )
}

unsigned int arwavelength_w_valid(
        const ART_GV        * art_gv,
        const ArWavelength  * w0
        )
{
    double
        spectralRangeStart = s500_channel_lower_bound( art_gv, 0 );

    double
        spectralRangeEnd =
            s500_channel_lower_bound(
                art_gv,
                s500_channels(art_gv) - 1
                )
            +
            s500_channel_width(
                art_gv,
                s500_channels(art_gv) - 1
                );
    
    int  valid = 1;
    
    if ( w0 )
    {
        for ( int i = 0; i < 4; i++ )
        {
            if (    m_d_isInf( ARWL_WI( *w0, i ) )
                 || m_d_isNaN( ARWL_WI( *w0, i ) )
                 || ARWL_WI( *w0, i ) < spectralRangeStart
                 || ARWL_WI( *w0, i ) > spectralRangeEnd )
            {
                valid = 0;
            }
        }
    }
    else
    {
        valid = 0;
    }
    
    return valid;
}

int arwavelength_ww_equal(
        const ART_GV        * art_gv,
        const ArWavelength  * w0,
        const ArWavelength  * w1
        )
{
    //  Potential problem: we only check the hero WL
    //  OTOH, given how HWSS is intended to work like, this should be ok
    
    return ARWL_WI( *w0, 0 ) == ARWL_WI( *w1, 0 );
}

void arwavelength_w_debugprintf(
        const ART_GV             * art_gv,
        const ArWavelength  * w0
        )
{
    debugprintf(
        "\nArWavelength( %3.0f,  %3.0f,  %3.0f,  %3.0f) [nm]\n",
        NANO_FROM_UNIT( ARWL_WI( *w0, 0 ) ),
        NANO_FROM_UNIT( ARWL_WI( *w0, 1 ) ),
        NANO_FROM_UNIT( ARWL_WI( *w0, 2 ) ),
        NANO_FROM_UNIT( ARWL_WI( *w0, 3 ) )
        );
}

/* ======================================================================== */
