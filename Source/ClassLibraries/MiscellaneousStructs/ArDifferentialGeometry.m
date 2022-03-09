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

#define ART_MODULE_NAME     ArDifferentialGeometry

#import "ArDifferentialGeometry.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

BOOL arface_on_shape_same_compound_face(
        const ArFaceOnShape * sf0, const ArFaceOnShape * sf1
        )
{
    return (sf0->compound_face
            && (sf0->compound_face == sf1->compound_face));
}

BOOL arface_on_shape_same_shape_and_face(
        const ArFaceOnShape * sf0, const ArFaceOnShape * sf1
        )
    { return ( (sf0->shape == sf1->shape) && (sf0->sheet == sf1->sheet) ); }

BOOL arface_on_shape_same(
        const ArFaceOnShape * sf0, const ArFaceOnShape * sf1
        )
{
    return ( arface_on_shape_same_compound_face(sf0, sf1)
            || arface_on_shape_same_shape_and_face(sf0, sf1) );
}

void arface_on_shape_update_crc32(
        const ArFaceOnShape * sf,
        int interpolated,
        UInt32 * crc32
        )
{
    if (sf->compound_face)
        crc32_update_with_pointer(crc32, sf->compound_face);
    else
    {
        crc32_update_with_pointer(crc32, sf->shape);
        crc32_update_with_long(crc32, sf->sheet);
        if (! interpolated)
            crc32_update_with_long(crc32, sf->face);
    }
}

BOOL armaterialpair_same(
        const ArMaterialPair * mp0, const ArMaterialPair * mp1
        )
    { return ( ( mp0->from == mp1->from )  && ( mp0->into == mp1->into ) ); }

BOOL armaterialpair_flipped(
        const ArMaterialPair * mp0, const ArMaterialPair * mp1
        )
    { return ( ( mp0->from == mp1->into )  && ( mp0->into == mp1->from ) ); }

BOOL armaterialpair_flipped_same(
        int flip, const ArMaterialPair * mp0, const ArMaterialPair * mp1
        )
{
    return ( flip ? armaterialpair_flipped(mp0, mp1)
                    : armaterialpair_same(mp0, mp1) );
}

void armaterialpair_invert(
        ArMaterialPair * mp
        )
{
    ArNode <ArpVolumeMaterial>  * help = mp->from;
    mp->from = mp->into; mp->into = help;
}

void armaterialpair_update_crc32(
        const ArMaterialPair * mp, UInt32 * crc32
        )
{
    crc32_update_with_pointer(crc32, mp->from);
    crc32_update_with_pointer(crc32, mp->into);
}

ArDifferentialGeometry ardifferentialgeometry_reflect(
        ArDifferentialGeometry  mf_0
        )
{
    ArDifferentialGeometry  mf_r;

    mf_r.materialPair.from = mf_0.materialPair.into;
    mf_r.materialPair.into = mf_0.materialPair.from;
    mf_r.faceOnShape.shape   = mf_0.faceOnShape.shape;
    mf_r.faceOnShape.sheet   = mf_0.faceOnShape.sheet;
    mf_r.faceOnShape.face    = mf_0.faceOnShape.face;
    mf_r.faceOnShape.type    = mf_0.faceOnShape.type ^ arface_on_shape_reverse;
    mf_r.faceOnShape.compound_face = mf_0.faceOnShape.compound_face;
    mf_r.localNormal            = mf_0.localNormal;

    return mf_r;
}

void ardifferentialgeometry_update_crc32(
        ArDifferentialGeometry  * mf_0,
        int             interpolated,
        UInt32        * crc32
        )
{
    arface_on_shape_update_crc32( & ARDIFFERENTIALGEOMETRY_FACE_ON_SHAPE(*mf_0),
                                interpolated,
                                crc32 );

    armaterialpair_update_crc32( & ARDIFFERENTIALGEOMETRY_MATERIALPAIR(*mf_0),
                                   crc32);
}

BOOL ardifferentialgeometry_same(
        const ArDifferentialGeometry * mf_0,
        const ArDifferentialGeometry * mf_1
        )
{
    return (armaterialpair_same( & ARDIFFERENTIALGEOMETRY_MATERIALPAIR(*mf_0),
                                 & ARDIFFERENTIALGEOMETRY_MATERIALPAIR(*mf_1) )
            && arface_on_shape_same( & ARDIFFERENTIALGEOMETRY_FACE_ON_SHAPE(*mf_0),
                                 & ARDIFFERENTIALGEOMETRY_FACE_ON_SHAPE(*mf_1) ) );
}

// ===========================================================================
