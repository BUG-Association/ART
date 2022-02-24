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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArnConstSpectrum)

ART_SPECTRUM_MODULE_INTERFACE(ArnConstSpectrum)

#import "ART_Scenegraph.h"
#import "ArpValue.h"

@interface ArnValConstSpectrum
        : ArNode
{
    ArSpectrum     * spectrum;
    ArSpectrum500  * internalHiresSpectrum;
}

@end

#define ARNVAL_CONST_COLOUR_INTERFACE(_Type, _type ) \
@interface ArnVal_const_##_type \
        : ArnValConstSpectrum < \
                ArpConcreteClass, \
                ArpSpectrum, \
                ArpSpectrumValues \
                > \
{ \
    Ar##_Type           nativeValue; \
} \
 \
- (id) init \
        : (Ar##_Type) newNativeValue \
        ; \
 \
@end

ARNVAL_CONST_COLOUR_INTERFACE( Grey,        g   );
ARNVAL_CONST_COLOUR_INTERFACE( RGB,         rgb );
ARNVAL_CONST_COLOUR_INTERFACE( CIEXYZ,      xyz );
ARNVAL_CONST_COLOUR_INTERFACE( Spectrum8,   s8  );
ARNVAL_CONST_COLOUR_INTERFACE( Spectrum11,  s11 );
ARNVAL_CONST_COLOUR_INTERFACE( Spectrum18,  s18 );
ARNVAL_CONST_COLOUR_INTERFACE( Spectrum46,  s46 );
ARNVAL_CONST_COLOUR_INTERFACE( PSSpectrum,  pss );
ARNVAL_CONST_COLOUR_INTERFACE( RSSpectrum,  rss );


@interface ArnVal_const_xyz (initFrom_xyY)

- (id) initFrom_CIExyY
        : (ArCIExyY) newCIExyY
        ;

@end

//   The following class is an almost exact clone of ArnVal_const_rss
//   It additionally contains a description string: ART measurement
//   archives can contain such extra information. During rendering, this
//   data is almost never needed, but can be important for some colour
//   science tasks. So if you read something from an ARK file, this sort
//   of node is created, and not an ArnVal_const_rss.

@interface ArnVal_const_archive_rss
        : ArnValConstSpectrum <
                ArpConcreteClass,
                ArpSpectrum,
                ArpSpectrumValues
                >
{
    ArRSSpectrum  nativeValue;
    ArSymbol      description;
    ArSymbol      additionalDescription;
}

@end

@interface ArnVal_const_archive_rss (archiveData)

- (id) init
        : (ArRSSpectrum) newNativeValue
        : (ArSymbol) newDescription
        : (ArSymbol) newAdditionalDescription
        ;

- (ArSymbol) description
        ;

- (ArSymbol) additionalDescription
        ;

@end


ArnVal_const_pss * arnconstpsspectrumvalue(
              ART_GV  * art_gv,
        const double    unit,
        ...
        );

ArnVal_const_rss * arnconstrsspectrumvalue(
              ART_GV  * art_gv,
        const double    start,
        const double    step,
        const double    unit,
        ...
        );


ArnVal_const_g  * arnconstgrey_100(
        ART_GV  * art_gv
        );

ArnVal_const_g  * arnconstgrey_050(
        ART_GV  * art_gv
        );

ArnVal_const_g  * arnconstgrey_000(
        ART_GV  * art_gv
        );

#define ARNGREY_100 \
        arnconstgrey_100(art_gv)
#define ARNGREY_050 \
        arnconstgrey_050(art_gv)
#define ARNGREY_000 \
        arnconstgrey_000(art_gv)

// ===========================================================================
