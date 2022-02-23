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
 * @file ARM_Trafo.h
 * @brief Transformations
 * @type Trafo
 */

#import "ART_Foundation.h"

ART_MODULE_INTERFACE(ARM_Trafo)

#import "ART_Trafo.h"

#import "ARM_Foundation.h"

ArnScale2D * arnscale2d(
        ART_GV   * art_gv,
        Scale2D    newScale
        );

ArnScale3D * arnscale3d(
        ART_GV   * art_gv,
        Scale3D    newScale
        );

ArnTranslation2D * arntranslation2d(
        ART_GV         * art_gv,
        Translation2D    newTranslation
        );

ArnTranslation3D * arntranslation3d(
        ART_GV         * art_gv,
        Translation3D    newTranslation
        );

ArnVarScale3D * arnvarscale3d(
        ART_GV                    * art_gv,
        ArNode <ArpDoubleValues>  * x_scale,
        ArNode <ArpDoubleValues>  * y_scale,
        ArNode <ArpDoubleValues>  * z_scale
        );

ArnVarTranslation3D * arnvartranslation3d(
        ART_GV                    * art_gv,
        ArNode <ArpDoubleValues>  * x_translation,
        ArNode <ArpDoubleValues>  * y_translation,
        ArNode <ArpDoubleValues>  * z_translation
        );

ArnRotX3D * arnrotx3d(
        ART_GV  * art_gv,
        double    alpha
        );

ArnRotY3D * arnroty3d(
        ART_GV  * art_gv,
        double    alpha
        );

ArnRotZ3D * arnrotz3d(
        ART_GV  * art_gv,
        double    alpha
        );

ArnRot3D * arnrot3d(
        ART_GV  * art_gv,
        Rot3D     newRot
        );

ArnVarRotX3D * arnvarrotx3d(
        ART_GV  * art_gv,
        ArNode  * alpha
        );

ArnVarRotY3D * arnvarroty3d(
        ART_GV  * art_gv,
        ArNode  * alpha
        );

ArnVarRotZ3D * arnvarrotz3d(
        ART_GV  * art_gv,
        ArNode  * alpha
        );

ArnShearXY3D * arnshearxy3d(
        ART_GV     * art_gv,
        ShearXY3D    newShear
        );

ArnShearYZ3D * arnshearyz3d(
        ART_GV     * art_gv,
        ShearYZ3D    newShear
        );

ArnShearZX3D * arnshearzx3d(
        ART_GV     * art_gv,
        ShearZX3D    newShear
        );

ArNode * arntrafopair(
        ART_GV  * art_gv,
        ArNode  * node0,
        ArNode  * node1
        );

ArnTrafo2DPair * arntrafo2dpair(
        ART_GV               * art_gv,
        ArNode <ArpTrafo2D>  * node0,
        ArNode <ArpTrafo2D>  * node1
        );

ArnTrafo3DPair * arntrafo3dpair(
        ART_GV               * art_gv,
        ArNode <ArpTrafo3D>  * node0,
        ArNode <ArpTrafo3D>  * node1
        );

/**
 * @section Scaling
 */
    /**
     * @brief Scale transform on x, y, z
     *
     *
     * Scale operator.
     * @def SCALE(x, y, z)
     * @param x     double  The scale factor on x axis.
     * @param y     double  The scale factor on y axis.
     * @param z     double  The scale factor on z axis.
     */
    #define SCALE(_x,_y,_z)         arnscale3d( art_gv, SCALE3D((_x),(_y),(_z)) )

    /**
     * @brief Uniform Scale transform
     *
     * Scale operator operating uniformly on x, y and z.
     *
     * @def USCALE(s)
     *
     * @param s     double  The scale factor on x, y z, axis.
     */
    #define USCALE(_s)              arnscale3d( art_gv, SCALE3D((_s),(_s),(_s)) )

    /**
     * @brief Scale transform on x
     *
     * Scale operator acting on x axis only
     *
     * @def SCALE_X(s)
     *
     * @param s     double  The scale factor on x axis.
     */
    #define SCALE_X(_s)             arnscale3d( art_gv, SCALE3D((_s),1,1) )

    /**
     * @brief Scale transform on y
     *
     * Scale operator acting on y axis only
     *
     * @def SCALE_Y(s)
     *
     * @param s     double  The scale factor on y axis.
     */
    #define SCALE_Y(_s)             arnscale3d( art_gv, SCALE3D(1,(_s),1) )

    /**
     * @brief Scale transform on z
     *
     * Scale operator acting on z axis only
     *
     * @def SCALE_Z(s)
     *
     * @param s     double  The scale factor on z axis.
     */
    #define SCALE_Z(_s)             arnscale3d( art_gv, SCALE3D(1,1,(_s)) )

    /**
     * @brief Scale transform
     *
     * Scale operator.
     *
     * @def SCALE_BT(vec)
     *
     * @param vec     Scale3D  The scale factor.
     */
    #define SCALE_BY(_vec)          arnscale3d( art_gv, SCALE3D_OF_VEC(_vec) )

/**
 * @section Translate
 */
    /**
     * @brief Translation on x, y, z
     *
     * Translation operator.
     *
     * @def TRANSLATION(x, y, z)
     *
     * @param x     double  The translation on x axis.
     * @param y     double  The translation on y axis.
     * @param z     double  The translation on z axis.
     */
    #define TRANSLATION(_x,_y,_z)   arntranslation3d( art_gv, TRANSLATION3D((_x),(_y),(_z)) )

    /**
     * @brief Translation on x, y, z from vector
     *
     * @def TRANSLATION_BY(vector)
     *
     * @param vector Vec3D  The x, y, z coordinates of the vector are going to be used for translation.
     */
    #define TRANSLATION_BY(_vec)    arntranslation3d( art_gv, TRANSLATION3D_V(_vec) )

    /**
     * @brief Translation on x, y, z from point
     *
     * @def TRANSLATION_TO(point)
     *
     * @param point Pnt3D  The x, y, z coordinates of the point are going to be used for translation.
     */
    #define TRANSLATION_TO(_pnt)    arntranslation3d( art_gv, TRANSLATION3D_P(_pnt) )

    /**
     * @brief Translation on x
     *
     * Translation on x axis.
     *
     * @def TRANSLATION_X(x)
     *
     * @param x     double  The translation on x axis.
     */
    #define TRANSLATION_X(_s)       arntranslation3d( art_gv, TRANSLATION3D((_s),0,0) )

    /**
     * @brief Translation on y
     *
     * Translation on y axis.
     *
     * @def TRANSLATION_Y(y)
     *
     * @param y     double  The translation on y axis.
     */
    #define TRANSLATION_Y(_s)       arntranslation3d( art_gv, TRANSLATION3D(0,(_s),0) )

    /**
     * @brief Translation on z
     *
     * Translation on z axis.
     *
     * @def TRANSLATION_Z(z)
     *
     * @param z     double  The translation on z axis.
     */
    #define TRANSLATION_Z(_s)       arntranslation3d( art_gv, TRANSLATION3D(0,0,(_s)) )

/**
 * @section Shear
 */
    /**
     * @brief Shear on xy
     *
     * @def SHEAR_XY(x, y)
     *
     * @param x     double  The shear parameter for x axis.
     * @param y     double  The shear parameter for y axis.
     */
    #define SHEAR_XY(_x,_y)         arnshearxy3d( art_gv, SHEARXY3D((_x),(_y)) )

    /**
     * @brief Shear on yz
     *
     * @def SHEAR_YZ(y, z)
     *
     * @param y     double  The shear parameter for y axis.
     * @param z     double  The shear parameter for z axis.
     */
    #define SHEAR_YZ(_y,_z)         arnshearyz3d( art_gv, SHEARYZ3D((_y),(_z)) )

    /**
     * @brief Shear on zx
     *
     * @def SHEAR_YZ(z, x)
     *
     * @param z     double  The shear parameter for z axis.
     * @param x     double  The shear parameter for x axis.
     */
    #define SHEAR_ZX(_z,_x)         arnshearzx3d( art_gv, SHEARZX3D((_z),(_x)) )

/**
 * @section Rotations
 */
    /**
     * @brief Rotation around x axis
     *
     * Rotation operator around x axis given an angle. The unit of the angle must be
     * specified (\verb?DEGREES? or \verb?RADIANS?), e.g. \verb?ROT_X(15 DEGREES)?
     *
     * @def ROT_X(alpha)
     *
     * @param alpha     double  The angle of rotation (in \verb?DEGREES? or \verb?RADIANS?).
     */
    #define ROT_X(_alpha)           arnrotx3d( art_gv,_alpha)

    /**
     * @brief Rotation around y axis
     *
     * Rotation operator around y axis given an angle. The unit of the angle must be
     * specified (\verb?DEGREES? or \verb?RADIANS?), e.g. \verb?ROT_Y(15 DEGREES)?
     *
     * @def ROT_Y(alpha)
     *
     * @param alpha     double  The angle of rotation (in \verb?DEGREES? or \verb?RADIANS?).
     */
    #define ROT_Y(_alpha)           arnroty3d( art_gv,_alpha)

    /**
     * @brief Rotation around z axis
     *
     * Rotation operator around z axis given an angle. The unit of the angle must be
     * specified (\verb?DEGREES? or \verb?RADIANS?), e.g. \verb?ROT_Z(15 DEGREES)?
     *
     * @def ROT_Z(alpha)
     *
     * @param alpha     double  The angle of rotation (in \verb?DEGREES? or \verb?RADIANS?).
     */
    #define ROT_Z(_alpha)           arnrotz3d( art_gv,_alpha)
/**
 * @popsection
 */
// ===========================================================================
