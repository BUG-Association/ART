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

#define ART_MODULE_NAME     ArnConstSpectrum

#import "ArnConstSpectrum.h"
#import "ArnColourStandardImplementation.h"

typedef struct ArnColour_GV
{
    ArnVal_const_g  * grey100;
    ArnVal_const_g  * grey050;
    ArnVal_const_g  * grey000;
}
ArnColour_GV;

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnValConstSpectrum       registerWithRuntime ];

    [ ArnVal_const_g            registerWithRuntime ];
    [ ArnVal_const_rgb          registerWithRuntime ];
    [ ArnVal_const_xyz          registerWithRuntime ];
    [ ArnVal_const_s8           registerWithRuntime ];
    [ ArnVal_const_s18          registerWithRuntime ];
    [ ArnVal_const_s46          registerWithRuntime ];
    [ ArnVal_const_pss          registerWithRuntime ];
    [ ArnVal_const_rss          registerWithRuntime ];

    ArnColour_GV  * arncolour_gv;

    arncolour_gv = ALLOC(ArnColour_GV);

    arncolour_gv->grey100 = 0;
    arncolour_gv->grey050 = 0;
    arncolour_gv->grey000 = 0;

    art_gv->arncolour_gv = arncolour_gv;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    RELEASE_OBJECT( art_gv->arncolour_gv->grey100 );
    RELEASE_OBJECT( art_gv->arncolour_gv->grey050 );
    RELEASE_OBJECT( art_gv->arncolour_gv->grey000 );

    FREE( art_gv->arncolour_gv );
)

ART_SPECTRUM_MODULE_INITIALISATION_FUNCTION
(
    if ( ! art_gv->arncolour_gv->grey100 )
        ARNODE_SINGLETON(
            art_gv->arncolour_gv->grey100,
            ARNGREY_100,
            [ ALLOC_INIT_OBJECT(ArnVal_const_g)
                :   ARGREY(1.0)
                ]
            );
    else
        [ art_gv->arncolour_gv->grey100 reinitialiseAfterISRChange
            ];

    if ( ! art_gv->arncolour_gv->grey050 )
        ARNODE_SINGLETON(
            art_gv->arncolour_gv->grey050,
            ARNGREY_050,
            [ ALLOC_INIT_OBJECT(ArnVal_const_g)
                :   ARGREY(0.5)
                ]
            );
    else
        [ art_gv->arncolour_gv->grey050 reinitialiseAfterISRChange
            ];

    if ( ! art_gv->arncolour_gv->grey000 )
        ARNODE_SINGLETON(
            art_gv->arncolour_gv->grey000,
            ARNGREY_000,
            [ ALLOC_INIT_OBJECT(ArnVal_const_g)
                :   ARGREY(0.0)
                ]
            );
    else
        [ art_gv->arncolour_gv->grey000 reinitialiseAfterISRChange
            ];
)

ART_SPECTRUM_MODULE_SHUTDOWN_FUNCTION
(
    if ( art_gv->arncolour_gv->grey100 )
        [ art_gv->arncolour_gv->grey100 prepareForISRChange ];
    if ( art_gv->arncolour_gv->grey050 )
        [ art_gv->arncolour_gv->grey050 prepareForISRChange ];
    if ( art_gv->arncolour_gv->grey000 )
        [ art_gv->arncolour_gv->grey000 prepareForISRChange ];
)



ArnVal_const_g  * arnconstgrey_100(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arncolour_gv->grey100;
}

ArnVal_const_g  * arnconstgrey_050(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arncolour_gv->grey050;
}

ArnVal_const_g  * arnconstgrey_000(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arncolour_gv->grey000;
}


@implementation ArnValConstSpectrum

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnValConstSpectrum)

- (void) _setup
{
}

- (void) dealloc
{
    if ( spectrum )
        spc_free(
            art_gv,
            spectrum
            );

    if ( internalHiresSpectrum )
        s500_free(
            art_gv,
            internalHiresSpectrum
            );

    [ super dealloc ];
}

@end

ARNVAL_CONST_COLOUR_IMPL(Grey,       g,    g);
ARNVAL_CONST_COLOUR_IMPL(RGB,        rgb,  rgb);
ARNVAL_CONST_COLOUR_IMPL(CIEXYZ,     xyz,  xyz);
ARNVAL_CONST_COLOUR_IMPL(Spectrum8,  s8,   s8);
ARNVAL_CONST_COLOUR_IMPL(Spectrum18, s18,  s18);
ARNVAL_CONST_COLOUR_IMPL(Spectrum46, s46,  s46);
ARNVAL_CONST_COLOUR_IMPL(PSSpectrum, pss,  pss);
ARNVAL_CONST_COLOUR_IMPL(RSSpectrum, rss,  rss);
ARNVAL_CONST_COLOUR_IMPL(RSSpectrum, rss,  archive_rss);

@implementation ArnVal_const_archive_rss (archiveData)

- (id) init
        : (ArRSSpectrum) newNativeValue
        : (ArSymbol) newDescription
        : (ArSymbol) newAdditionalDescription
{
    self =
        [ self init
            :   newNativeValue
            ];
    
    if ( self )
    {
        description = newDescription;
        additionalDescription = newAdditionalDescription;
    }
    
    return self;
}

- (ArSymbol) description
{
    return  description;
}

- (ArSymbol) additionalDescription
{
    return  additionalDescription;
}

@end

ArnVal_const_rss * arnconstrsspectrumvalue(
              ART_GV  * art_gv,
        const double    start,
        const double    step,
        const double    unit,
        ...
        )
{
    ArnVal_const_rss          * s;
    va_list                     argPtr;

    va_start( argPtr, unit );

    ArRSSpectrum rssSpectrum =
        rsspectrum_v( start, step, unit, argPtr );

    s =
        [ ALLOC_INIT_OBJECT(ArnVal_const_rss)
            :   rssSpectrum
            ];

    //   In this case, we do not free the created rss spectrum contents,
    //   as the allocated space is taken over (and later freed) by the
    //   ArnVal_const_rss node.

    va_end( argPtr );

    return s;
}

ArnVal_const_pss * arnconstpsspectrumvalue(
              ART_GV  * art_gv,
        const double    maximum,
        ...
        )
{
    ArnVal_const_pss          * s;
    va_list                     argPtr;

    va_start( argPtr, maximum );

    ArPSSpectrum psSpectrum =
        psspectrum_v( maximum, argPtr );

    s =
        [ ALLOC_INIT_OBJECT(ArnVal_const_pss)
            :   psSpectrum
            ];

    //   In this case, we do not free the created pss spectrum contents,
    //   as the allocated space is taken over (and later freed) by the
    //   ArnVal_const_pss node.

    va_end( argPtr );

    return s;
}


// ===========================================================================
