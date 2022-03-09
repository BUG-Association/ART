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

#ifndef _ARPVALUE_H_
#define _ARPVALUE_H_

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArpValue)

@class ArNode;
@class AraTrafo;
@class AraTrafo2D;
@class ArnExternal;

#import "ArpEvaluationEnvironment.h"
#import "ArcPointContext.h"


#define IS_VALUE(_node)  \
    [_node conformsToArProtocol :ARPROTOCOL(ArpValues)]

#define IS_DOUBLE_VALUE(_node)  \
    [_node conformsToArProtocol :ARPROTOCOL(ArpDoubleValues)]

#define IS_LONG_VALUE(_node)  \
    [_node conformsToArProtocol :ARPROTOCOL(ArpIntValues)]

#define IS_COLOUR_VALUE(_node)  \
    [_node conformsToArProtocol :ARPROTOCOL(ArpSpectrumValues)]

#define IS_PNT2D_VALUE(_node)  \
    [_node conformsToArProtocol :ARPROTOCOL(ArpPnt2DValues)]

#define IS_VEC2D_VALUE(_node)  \
    [_node conformsToArProtocol :ARPROTOCOL(ArpVec2DValues)]

#define IS_PNT3D_VALUE(_node)  \
    [_node conformsToArProtocol :ARPROTOCOL(ArpPnt3DValues)]

#define IS_VEC3D_VALUE(_node)  \
    [_node conformsToArProtocol :ARPROTOCOL(ArpVec3DValues)]

typedef enum ArValueType
{
    arvalue_none                = 0x00000000,
    arvalue_untyped             = 0x00000000,

    arvalue_int                = 0x00000001,
    arvalue_double              = 0x00000002,

    arvalue_pnt2d               = 0x00000004,
    arvalue_vec2d               = 0x00000008,
    arvalue_pnt3d               = 0x00000010,
    arvalue_vec3d               = 0x00000020,

    arvalue_spectrum              = 0x00000040,
    arvalue_attenuation         = 0x00000080,

    arvalue_type                = 0x00000fff,
    arvalue_hit                 = 0x00001000,
    arvalue_ray                 = 0x00002000,
    arvalue_trafo               = 0x00004000,
    arvalue_map                 = 0x00008000,  // The value takes its
                                               // arguments through the
                                               // supplied array of
                                               // the get... method.

    /* -----------------------------------------------------------------------
        The following convenience definitions help to make the macros
        for operator definition shorter.
    ----------------------------------------------------------------------- */
    arvalue_Int                = arvalue_int,
    arvalue_Double              = arvalue_double,
    arvalue_Pnt2D               = arvalue_pnt2d,
    arvalue_Vec2D               = arvalue_vec2d,
    arvalue_Pnt3D               = arvalue_pnt3d,
    arvalue_Vec3D               = arvalue_vec3d,
}
ArValueType;

/* ===========================================================================
    'ArpValues'
=========================================================================== */
@protocol ArpValues

/* ---------------------------------------------------------------------------
    'valueType'
        This function returns an enumeration indicating what types of value
        an expression can return.  This could also be checked using the
        protocol conformance interface, but since this function is needed
        during evaluation of expression trees, this interface was introduced
        as an optimization. The supplied mask specifies, which bits the
        caller is interested in. This is used to avoid unnecessary recursive
        calls.
--------------------------------------------------------------------------- */
- (ArValueType) valueType
        : (ArValueType) valueTypeMask
        ;

/* ---------------------------------------------------------------------------
    Default 'valueType' implementations
 --------------------------------------------------------------------------- */

#define ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(_type) \
        - (ArValueType) valueType \
                : (ArValueType) valueTypeMask \
        { \
            return (_type) & valueTypeMask; \
        }

#define ARPVALUES_UNARY_VALUETYPE_IMPLEMENTATION(_type) \
        - (ArValueType) valueType \
                : (ArValueType) valueTypeMask \
        { \
            ArValueType subValueType = valueTypeMask; \
            if (subValueType) \
                subValueType = \
                    [(id <ArpValues>)ARNUNARY_SUBNODE valueType : subValueType]; \
            return (subValueType | (_type)) & valueTypeMask; \
        }

#define ARPVALUES_BINARY_VALUETYPE_IMPLEMENTATION(_type) \
        - (ArValueType) valueType \
                : (ArValueType) valueTypeMask \
        { \
            ArValueType subValueType = valueTypeMask; \
            if (subValueType) \
                subValueType = \
                  [(id <ArpValues>)ARNBINARY_SUBNODE_0 valueType : subValueType] | \
                  [(id <ArpValues>)ARNBINARY_SUBNODE_1 valueType : subValueType]; \
            return (subValueType | (_type)) & valueTypeMask; \
        }

#define ARPVALUES_TERNARY_VALUETYPE_IMPLEMENTATION(_type) \
        - (ArValueType) valueType \
                : (ArValueType) valueTypeMask \
        { \
            ArValueType subValueType = valueTypeMask; \
            if (subValueType) \
                subValueType = \
                  [(id <ArpValues>)ARNTERNARY_SUBNODE_0 valueType : subValueType] | \
                  [(id <ArpValues>)ARNTERNARY_SUBNODE_1 valueType : subValueType] | \
                  [(id <ArpValues>)ARNTERNARY_SUBNODE_2 valueType : subValueType]; \
            return (subValueType | (_type)) & valueTypeMask; \
        }

/* ---------------------------------------------------------------------------
    'evalEnvType'
        Returns a bit mask representing an evaluation environment capabilities
        needed for evaluating context-dependent constants.
--------------------------------------------------------------------------- */
- (ArEvaluationEnvironmentType) evalEnvType
        ;

/* ---------------------------------------------------------------------------
    Default 'evalEnvType' implementations
 --------------------------------------------------------------------------- */

#define ARPVALUES_NULLARY_EVALENVTYPE_IMPLEMENTATION(_type) \
        - (ArEvaluationEnvironmentType) evalEnvType \
        { \
            return (_type); \
        }

#define ARPVALUES_UNARY_EVALENVTYPE_IMPLEMENTATION(_type) \
        - (ArEvaluationEnvironmentType) evalEnvType \
        { \
            ArEvaluationEnvironmentType subValueType = \
                [ (id <ArpValues>)ARNUNARY_SUBNODE evalEnvType ]; \
            return subValueType | (_type); \
        }

#define ARPVALUES_BINARY_EVALENVTYPE_IMPLEMENTATION(_type) \
        - (ArEvaluationEnvironmentType) evalEnvType \
        { \
            ArEvaluationEnvironmentType subValueType = \
                [ (id <ArpValues>)ARNBINARY_SUBNODE_0 evalEnvType ] | \
                [ (id <ArpValues>)ARNBINARY_SUBNODE_1 evalEnvType ]; \
            return subValueType | (_type); \
        }

#define ARPVALUES_TERNARY_EVALENVTYPE_IMPLEMENTATION(_type) \
        - (ArEvaluationEnvironmentType) evalEnvType \
        { \
            ArEvaluationEnvironmentType subValueType = \
                [ (id <ArpValues>)ARNTERNARY_SUBNODE_0 evalEnvType ] | \
                [ (id <ArpValues>)ARNTERNARY_SUBNODE_1 evalEnvType ] | \
                [ (id <ArpValues>)ARNTERNARY_SUBNODE_2 evalEnvType ]; \
            return subValueType | (_type); \
        }

#define ARPVALUES_QUATERNARY_EVALENVTYPE_IMPLEMENTATION(_type) \
        - (ArEvaluationEnvironmentType) evalEnvType \
        { \
            ArEvaluationEnvironmentType subValueType = \
                [ (id <ArpValues>)ARNQUATERNARY_SUBNODE_0 evalEnvType ] | \
                [ (id <ArpValues>)ARNQUATERNARY_SUBNODE_1 evalEnvType ] | \
                [ (id <ArpValues>)ARNQUATERNARY_SUBNODE_2 evalEnvType ] | \
                [ (id <ArpValues>)ARNQUATERNARY_SUBNODE_3 evalEnvType ]; \
            return subValueType | (_type); \
        }

@end

/* ===========================================================================
    'ArpVariable'
=========================================================================== */
@protocol ArpVariable

- (void) addValueToEvalEnv
        : (ArNode <ArpValues> *) value
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        ;

- (void) delValueFromEvalEnv
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        ;

@end

#define ARPVALUES_MAX_VALUES        8

/* ===========================================================================
    'ArpIntValues'
        ...

        'getIntValue(s)' methods are used for accessing the node value(s)
        based on the provided evaluation environment. The returned value is
        the number of returned values (0 means error).

        'getConstIntValue(s)' methods are used for both checking whether
        the node does not change it's value(s) and accessing that value(s)
        at the same time. This can be useful for example when a surface wants
        to precompute some data based on non-changing input parameters.
        The returned value is the number of returned values, 0 means
        an error or that the node does not act as a constant.
=========================================================================== */
@protocol ArpIntValues <ArpValues>

- (unsigned int) getIntValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const unsigned int) numberOfValues
        : (      Int *) outValues
        ;

- (unsigned int) getIntValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (      Int *) outValue
        ;

- (unsigned int) getConstIntValues
        : (const unsigned int) numberOfValues
        : (      Int *) outValues
        ;

- (unsigned int) getConstIntValue
        : (Int *) outValue
        ;

@end

/* ===========================================================================
    'ArpDoubleValues'
        ...

        'ArpDoubleValue(s)' methods are used for accessing the node value(s)
        based on the provided evaluation environment. The returned value is
        the number of returned values (0 means error).

        'getConstDoubleValue(s)' methods are used for both checking whether
        the node does not change it's value(s) and accessing that value(s)
        at the same time. This can be useful for example when a surface wants
        to precompute some data based on non-changing input parameters.
        The returned value is the number of returned values, 0 means
        an error or that the node does not act as a constant.
=========================================================================== */
@protocol ArpDoubleValues <ArpValues>

- (unsigned int) getDoubleValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const unsigned int) numberOfValues
        : (      Double *) outValues
        ;

- (unsigned int) getDoubleValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (      Double *) outValue
        ;

- (unsigned int) getConstDoubleValues
        : (const unsigned int) numberOfValues
        : (      Double *) outValues
        ;

- (unsigned int) getConstDoubleValue
        : (Double *) outValue
        ;

@end

/* ===========================================================================
    'ArpSpectrumValues'

        This protocol is based on the ArpSpectrum protocol.
        TODO: Move the proper comments from it here.
 =========================================================================== */
@protocol ArpSpectrumValues <ArpValues>

- (unsigned int) getSpectrumValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const unsigned int) numberOfValues
        : (      ArSpectrum *) outSpectrum
        ;

- (unsigned int) getSpectrumValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (      ArSpectrum *) outSpectrum
        ;

- (unsigned int) getHiresSpectrumValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (      ArSpectrum500 *) outSpectrum
        ;

- (unsigned int) getSpectralSample
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const ArWavelength *) wavelength
        : (      ArSpectralSample *) outSpectralSample
        ;

- (unsigned int) getAttenuationValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const unsigned int) numberOfValues
        : (      ArAttenuation *) outAttenuations
        ;

- (unsigned int) getAttenuationValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (      ArAttenuation *) outAttenuation
        ;

- (unsigned int) getAttenuationSample
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const ArWavelength *) wavelength
        : (      ArAttenuationSample *) outAttenuationSample
        ;

- (unsigned int) getDepolarisingAttenuationValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (const unsigned int) numberOfValues
        : (      ArAttenuation *) outAttenuations
        ;

- (unsigned int) getDepolarisingAttenuationValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (      ArAttenuation *) outAttenuation
        ;

- (unsigned int) getDepolarisingAttenuationSample
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (const ArWavelength *) wavelength
        : (      ArAttenuationSample *) outAttenuationSample
        ;

- (unsigned int) getNonpolarisingAttenuationValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (const unsigned int) numberOfValues
        : (      ArAttenuation *) outAttenuations
        ;

- (unsigned int) getNonpolarisingAttenuationValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (      ArAttenuation *) outAttenuation
        ;

// TODO: get constant values?

@end

//- (unsigned int) getNewPSSpectrumValues
//    : (ArcPointContext *) locationInfo
//    : (const unsigned int) numberOfValues
//    : (ArPSSpectrum *) outPSSpectrums
//    ;
//
//- (unsigned int) getNewPSSpectrumValue
//    : (ArcPointContext *) locationInfo
//    : (ArPSSpectrum *) outPSSpectrum
//    ;
//
//- (unsigned int) valueAtWavelengthValues
//    : (ArcPointContext *) locationInfo
//    : (const unsigned int) numberOfValues
//    : (const double) wavelength
//    : (double) outValues
//    ;
//
//- (unsigned int) valueAtWavelengthValue
//    : (ArcPointContext *) locationInfo
//    : (const double) wavelength
//    : (double) outValue
//    ;

/* ===========================================================================
    'ArpPnt2DValues'
        ...

        'getPnt2DValue(s)' methods are used for accessing the node value(s)
        based on the provided evaluation environment. The returned value is
        the number of returned values (0 means error).

        'getConstPnt2DValue(s)' methods are used for both checking whether
        the node does not change it's value(s) and accessing that value(s)
        at the same time. This can be useful for example when a surface wants
        to precompute some data based on non-changing input parameters.
        The returned value is the number of returned values, 0 means
        an error or that the node does not act as a constant.
=========================================================================== */
@protocol ArpPnt2DValues <ArpValues>

- (unsigned int) getPnt2DValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const unsigned int) numberOfValues
        : (      Pnt2D *) outValues
        ;

- (unsigned int) getPnt2DValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (      Pnt2D *) outValue
        ;

- (unsigned int) getConstPnt2DValues
        : (const unsigned int) numberOfValues
        : (      Pnt2D *) outValues
        ;

- (unsigned int) getConstPnt2DValue
        : (Pnt2D *) outValue
        ;

@end

/* ===========================================================================
    'ArpVec2DValues'
        ...

        'getPnt2DValue(s)' methods are used for accessing the node value(s)
        based on the provided evaluation environment. The returned value is
        the number of returned values (0 means error).

        'getConstPnt2DValue(s)' methods are used for both checking whether
        the node does not change it's value(s) and accessing that value(s)
        at the same time. This can be useful for example when a surface wants
        to precompute some data based on non-changing input parameters.
        The returned value is the number of returned values, 0 means
        an error or that the node does not act as a constant.
=========================================================================== */
@protocol ArpVec2DValues <ArpValues>

- (unsigned int) getVec2DValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const unsigned int) numberOfValues
        : (      Vec2D *) outValues
        ;

- (unsigned int) getVec2DValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (      Vec2D *) outValue
        ;

- (unsigned int) getConstVec2DValues
        : (const unsigned int) numberOfValues
        : (      Vec2D *) outValues
        ;

- (unsigned int) getConstVec2DValue
        : (Vec2D *) outValue
        ;

@end

/* ===========================================================================
    'ArpPnt3DValues'
        ...

        'getPnt3DValue(s)' methods are used for accessing the node value(s)
        based on the provided evaluation environment. The returned value is
        the number of returned values (0 means error).

        'getConstPnt3DValue(s)' methods are used for both checking whether
        the node does not change it's value(s) and accessing that value(s)
        at the same time. This can be useful for example when a surface wants
        to precompute some data based on non-changing input parameters.
        The returned value is the number of returned values, 0 means
        an error or that the node does not act as a constant.
=========================================================================== */
@protocol ArpPnt3DValues <ArpValues>

- (unsigned int) getPnt3DValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const unsigned int) numberOfValues
        : (      Pnt3D *) outValues
        ;

- (unsigned int) getPnt3DValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (      Pnt3D *) outValue
        ;

- (unsigned int) getConstPnt3DValues
        : (const unsigned int) numberOfValues
        : (      Pnt3D *) outValues
        ;

- (unsigned int) getConstPnt3DValue
        : (Pnt3D *) outValue
        ;

@end

/* ===========================================================================
    'ArpVec3DValues'
        ...

        'getVec3DValue(s)' methods are used for accessing the node value(s)
        based on the provided evaluation environment. The returned value is
        the number of returned values (0 means error).

        'getConstVec3DValue(s)' methods are used for both checking whether
        the node does not change it's value(s) and accessing that value(s)
        at the same time. This can be useful for example when a surface wants
        to precompute some data based on non-changing input parameters.
        The returned value is the number of returned values, 0 means
        an error or that the node does not act as a constant.
=========================================================================== */
@protocol ArpVec3DValues <ArpValues>

- (unsigned int) getVec3DValues
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (const unsigned int) numberOfValues
        : (      Vec3D *) outValues
        ;

- (unsigned int) getVec3DValue
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv
        : (      Vec3D *) outValue
        ;

- (unsigned int) getConstVec3DValues
        : (const unsigned int) numberOfValues
        : (      Vec3D *) outValues
        ;

- (unsigned int) getConstVec3DValue
        : (Vec3D *) outValue
        ;

@end

/* ---------------------------------------------------------------------------
    'ARPVALUES_...'
--------------------------------------------------------------------------- */

#define ARPVALUES_SOLID_TEXTURE_IMPLEMENTATION(_pnt,_texf) \
        ARPVALUES_UNARY_VALUETYPE_IMPLEMENTATION(arvalue_int|arvalue_double) \
        - (unsigned int) getIntValues \
                : (const ArcObject <ArpEvaluationEnvironment> *) env \
                : (const unsigned int) numVals \
                : (int *) outValues \
        { \
            double doubleVals[ARPVALUES_MAX_VALUES]; \
            _pnt pnt[ARPVALUES_MAX_VALUES]; \
            unsigned int i; \
            unsigned int storedVals = [env getInts: (const unsigned int)self \
                                   : numVals : outValues]; \
            if (storedVals) return storedVals; \
            numVals=[(ArNode <Arp##_pnt##Values> *)subnode get##_pnt##Values \
                            :env :numVals :pnt]; \
            for (i = 0; i < numVals; i++) \
                doubleVals[i] = _texf(self,&pnt[i],&outValues[i]); \
            [env addInts :(unsigned int)self :numVals :outValues]; \
            [env addDoubles :1+(unsigned int)self :numVals :doubleVals]; \
            return numVals; \
        } \
        - (unsigned int) getDoubleValues \
                : (const ArcObject <ArpEvaluationEnvironment> *) env \
                : (const unsigned int) numVals \
                : (double *) outValues \
        { \
            long longVals[ARPVALUES_MAX_VALUES]; \
            _pnt pnt[ARPVALUES_MAX_VALUES]; \
            unsigned int i; \
            unsigned int storedVals = \
                [env getDoubles :1+(unsigned int)self :numVals :outValues]; \
            if (storedVals) return storedVals; \
            numVals = [(ArNode <Arp##_pnt##Values> *)subnode \
                            get##_pnt##Values :env :numVals :pnt]; \
            for (i = 0; i < numVals; i++) \
                outValues[i] = _texf(self,&pnt[i],&longVals[i]); \
            [env addInts :(unsigned int)self :numVals :longVals]; \
            [env addDoubles :1+(unsigned int)self :numVals :outValues]; \
            return numVals; \
        } \


#define ARPVALUES_SOLID_XX_TEX_IMPLEMENTATION(_p0,_p1,_texf) \
        ARPVALUES_BINARY_VALUETYPE_IMPLEMENTATION(arvalue_int|arvalue_double) \
        - (unsigned int) getIntValues \
                : (const ArcObject <ArpEvaluationEnvironment> *) env \
                : (const unsigned int) numVals \
                : (long *) outValues \
        { \
            double doubleVals[ARPVALUES_MAX_VALUES]; \
            _p0 pnt0[ARPVALUES_MAX_VALUES]; \
            _p1 pnt1[ARPVALUES_MAX_VALUES]; \
            unsigned int i; \
            unsigned int storedVals = [env getInts: (const unsigned int)self \
                                   : numVals : outValues]; \
            if (storedVals) return storedVals; \
            numVals = M_MIN( \
                [(ArNode <Arp##_p0##Values> *)subnodeArray[0] \
                    get##_p0##Values \
                        :   env \
                        :   numVals \
                        :   pnt0 \
                        ], \
                [(ArNode <Arp##_p1##Values> *)subnodeArray[1] \
                    get##_p1##Values \
                        :   env \
                        :   numVals \
                        :   pnt1 \
                        ]); \
            for (i = 0; i < numVals; i++) \
                doubleVals[i] = _texf(self,&pnt0[i],&pnt1[i],&outValues[i]); \
            [env addInts :(unsigned int)self :numVals :outValues]; \
            [env addDoubles :1+(unsigned int)self :numVals :doubleVals]; \
            return numVals; \
        } \
        - (unsigned int) getDoubleValues \
                : (const ArcObject <ArpEvaluationEnvironment> *) env \
                : (const unsigned int) numVals \
                : (double *) outValues \
        { \
            long longVals[ARPVALUES_MAX_VALUES]; \
            _p0 pnt0[ARPVALUES_MAX_VALUES]; \
            _p1 pnt1[ARPVALUES_MAX_VALUES]; \
            unsigned int i; \
            unsigned int storedVals = \
                [env getDoubles :1+(unsigned int)self :numVals :outValues]; \
            if (storedVals) \
                return storedVals; \
            numVals = M_MIN( \
                [(ArNode <Arp##_p0##Values>*)subnodeArray[0] \
                    get##_p0##Values \
                            :env :numVals :pnt0], \
                [(ArNode <Arp##_p1##Values>*)subnodeArray[1] \
                    get##_p1##Values \
                            :env :numVals :pnt1]); \
            for (i = 0; i < numVals; i++) \
                outValues[i] = _texf(self,&pnt0[i],&pnt1[i], &longVals[i]); \
            [env addInts :(unsigned int)self :numVals :longVals]; \
            [env addDoubles :1+(unsigned int)self :numVals :outValues]; \
            return numVals; \
        } \

#endif // _ARPVALUE_H_

// ===========================================================================
