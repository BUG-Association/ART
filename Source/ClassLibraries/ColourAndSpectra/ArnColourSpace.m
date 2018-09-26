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

#define ART_MODULE_NAME     ArnColourSpace

#import "ArnColourSpace.h"

#import "ART_Parser.h"

typedef struct ArnColourSpace_GV
{
    ArnDefaultRGBColourSpace  * dcsp;
}
ArnColourSpace_GV;


ArColourSpace const  * ARCOLOURSPACEREF_sRGB;
ArColourSpace const  * ARCOLOURSPACEREF_AdobeRGB;
ArColourSpace const  * ARCOLOURSPACEREF_WideGamutRGB;

ART_MODULE_INITIALISATION_FUNCTION
(
    [ ArnColourSpace registerWithRuntime ];
    [ ArnDefaultRGBColourSpace registerWithRuntime ];

    ArnColourSpace  * ics0 =
        ART_PARSE_EXISTING_FILE_SEARCH_LIB_PATH(
            "sRGB.icm",
            ArnColourSpace
            );

    ArnColourSpace  * ics1 =
        ART_PARSE_EXISTING_FILE_SEARCH_LIB_PATH(
            "AdobeRGB1998.icc",
            ArnColourSpace
            );

    ArnColourSpace  * ics2 =
        ART_PARSE_EXISTING_FILE_SEARCH_LIB_PATH(
            "WideGamutRGB.icc",
            ArnColourSpace
            );

    ARCOLOURSPACEREF_sRGB         = [ ics0 colourSpaceRef ];
    ARCOLOURSPACEREF_AdobeRGB     = [ ics1 colourSpaceRef ];
    ARCOLOURSPACEREF_WideGamutRGB = [ ics2 colourSpaceRef ];

    RELEASE_OBJECT( ics0 );
    RELEASE_OBJECT( ics1 );
    RELEASE_OBJECT( ics2 );

    set_default_rgbspace_ref( art_gv, ARCOLOURSPACEREF_sRGB );
    set_rgb_computationspace_ref( art_gv, ARCOLOURSPACEREF_sRGB );

    ArnColourSpace_GV  * arncolourspace_gv;

    arncolourspace_gv = ALLOC(ArnColourSpace_GV);

    ARNODE_SINGLETON(
        arncolourspace_gv->dcsp,
        ARNDEFAULT_RGB_COLOURSPACE_SINGLETON,
        [ ALLOC_INIT_OBJECT(ArnDefaultRGBColourSpace)
            ]
        );

    art_gv->arncolourspace_gv = arncolourspace_gv;
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    RELEASE_OBJECT( art_gv->arncolourspace_gv->dcsp );

    FREE( art_gv->arncolourspace_gv );
)


ArnDefaultRGBColourSpace  * arndefault_rgb_colourspace_singleton(
        ART_GV  * art_gv
        )
{
    return
        art_gv->arncolourspace_gv->dcsp;
}


@implementation ArnColourSpace

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnColourSpace)

- init
        : (ArColourSpaceRef) newColourSpaceRef
{
    self = [ super init ];
//printf(">>>> %s <<<<",ARCSR_NAME(newColourSpaceRef));fflush(stdout);

    if ( self )
    {
        colourSpaceRef = newColourSpaceRef;
    }
    
    return self;
}

- (ArColourSpace const *) colourSpaceRef
{
    return colourSpaceRef;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];

    if ( [ coder isReading ] )
    {
        ArSymbol  symbol;

        [ coder codeSymbol: & symbol ];

        if ( ! ( colourSpaceRef = arcolourspaceref_for_csname( art_gv, symbol ) ) )
            ART_ERRORHANDLING_FATAL_ERROR(
                "no colour space with name '%s' registered in global table"
                ,   symbol
                );
    }
    else
        [ coder codeSymbol: & ARCSR_NAME(colourSpaceRef) ];
}

@end


@implementation ArnDefaultRGBColourSpace

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnDefaultRGBColourSpace)

- (ArColourSpace const *) colourSpaceRef
{
    return default_rgbspace_ref(art_gv);
}

@end

// ===========================================================================
