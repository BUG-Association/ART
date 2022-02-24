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

//#define ARSPECTRUM_CONVERSION_TRACE_DEBUGPRINTF
//#define ARSPECTRUM_CONVERSION_RESULTS_DEBUGPRINTF

#ifdef ARSPECTRUM_CONVERSION_TRACE_DEBUGPRINTF
#define CC_START_DEBUGPRINTF(__fn) \
printf( "\n" #__fn "( \n");fflush(stdout);

#define CC_END_DEBUGPRINTF(__fn) \
printf(") \n");fflush(stdout);

#else

#define CC_START_DEBUGPRINTF(__fn)
#define CC_END_DEBUGPRINTF(__fn)

#endif

#ifdef ARSPECTRUM_CONVERSION_RESULTS_DEBUGPRINTF
#define CC_OPERAND_DEBUGPRINTF( __type, __ptr ) \
__type##_s_debugprintf( art_gv, __ptr );

#else

#define CC_OPERAND_DEBUGPRINTF( __type, __ptr )

#endif

#define pss_new_c_debugprintf   pss_c_debugprintf
#define rss_new_c_debugprintf   rss_c_debugprintf

#define DUMMY_COLOUR_CONVERSION_BY_COPYING(_Type,_type) \
void _type##_to_##_type( \
        const ART_GV     * art_gv, \
        const Ar##_Type  * c0, \
              Ar##_Type  * cr  \
        ) \
{ \
    (void) art_gv; \
\
    CC_START_DEBUGPRINTF( _type##_to_##_type ) \
    CC_OPERAND_DEBUGPRINTF( _type, c0 ) \
    CC_OPERAND_DEBUGPRINTF( _type, cr ) \
    (*cr) = (*c0); \
    CC_OPERAND_DEBUGPRINTF( _type, cr ) \
    CC_END_DEBUGPRINTF( _type##_to_##_type ) \
}

#define COLOUR_CONVERSION_BY_MACRO(_TYPE0,_Type0,_type0,_TYPE1,_Type1,_type1) \
void _type0##_to_##_type1( \
        const ART_GV      * art_gv, \
        const Ar##_Type0  * c0, \
              Ar##_Type1  * cr  \
        ) \
{ \
   (void) art_gv; \
\
    CC_START_DEBUGPRINTF( _type0##_to_##_type1 ) \
    CC_OPERAND_DEBUGPRINTF( _type0, c0 ) \
    (*cr) = AR##_TYPE1##_OF_AR##_TYPE0(*c0); \
    CC_OPERAND_DEBUGPRINTF( _type1, cr ) \
    CC_END_DEBUGPRINTF( _type0##_to_##_type1 ) \
}

#define COLOUR_CONVERSION_VIA_XYZ(_TypeA,_typeA,_TypeB,_typeB) \
    _COLOUR_CONVERSION_VIA_XYZ(_TypeA,_typeA,_TypeB,_typeB,)

#define COLOUR_CONVERSION_VIA_XYZ_NEW(_TypeA,_typeA,_TypeB,_typeB) \
    _COLOUR_CONVERSION_VIA_XYZ(_TypeA,_typeA,_TypeB,_typeB,_new)

#define _COLOUR_CONVERSION_VIA_XYZ(_TypeA,_typeA,_TypeB,_typeB,_new) \
void _typeA##_to_##_typeB##_new( \
        const ART_GV      * art_gv, \
        const Ar##_TypeA  * source, \
              Ar##_TypeB  * target \
        ) \
{ \
    CC_START_DEBUGPRINTF( _typeA##_to_##_typeB ) \
    CC_OPERAND_DEBUGPRINTF( _typeA, source ) \
    ArCIEXYZ  xyz = ARCIEXYZ( 0.0, 0.0, 0.0 ); \
    _typeA##_to_xyz(art_gv,source, &xyz); \
    xyz_to_##_typeB##_new(art_gv,&xyz, target); \
    CC_OPERAND_DEBUGPRINTF( _typeB, target ) \
    CC_END_DEBUGPRINTF( _typeA##_to_##_typeB ) \
}


#define COLOUR_CONVERSION_VIA_GREY(_TypeA,_typeA,_TypeB,_typeB) \
    _COLOUR_CONVERSION_VIA_GREY(_TypeA,_typeA,_TypeB,_typeB,)

#define COLOUR_CONVERSION_VIA_GREY_NEW(_TypeA,_typeA,_TypeB,_typeB) \
    _COLOUR_CONVERSION_VIA_GREY(_TypeA,_typeA,_TypeB,_typeB,_new)

#define _COLOUR_CONVERSION_VIA_GREY(_TypeA,_typeA,_TypeB,_typeB,_new) \
void _typeA##_to_##_typeB##_new( \
        const ART_GV      * art_gv, \
        const Ar##_TypeA  * source, \
              Ar##_TypeB  * target \
        ) \
{ \
    CC_START_DEBUGPRINTF( _typeA##_to_##_typeB ) \
    CC_OPERAND_DEBUGPRINTF( _typeA, source ) \
    ArGrey  gry = ARGREY( 0.0 ); \
    _typeA##_to_g(art_gv,source, &gry); \
    g_to_##_typeB##_new(art_gv,&gry, target); \
    CC_OPERAND_DEBUGPRINTF( _typeB, target ) \
    CC_END_DEBUGPRINTF( _typeA##_to_##_typeB ) \
}

#define COLOUR_CONVERSION_VIA_RGB(_TypeA,_typeA,_TypeB,_typeB) \
    _COLOUR_CONVERSION_VIA_RGB(_TypeA,_typeA,_TypeB,_typeB,)

#define COLOUR_CONVERSION_VIA_RGB_NEW(_TypeA,_typeA,_TypeB,_typeB) \
    _COLOUR_CONVERSION_VIA_RGB(_TypeA,_typeA,_TypeB,_typeB,_new)

#define _COLOUR_CONVERSION_VIA_RGB(_TypeA,_typeA,_TypeB,_typeB,_new) \
void _typeA##_to_##_typeB##_new( \
        const ART_GV      * art_gv, \
        const Ar##_TypeA  * source, \
              Ar##_TypeB  * target \
        ) \
{ \
    CC_START_DEBUGPRINTF( _typeA##_to_##_typeB ) \
    CC_OPERAND_DEBUGPRINTF( _typeA, source ) \
    ArRGB  rgb = ARRGB( 0.0, 0.0, 0.0 ); \
    _typeA##_to_rgb(art_gv,source, &rgb); \
    rgb_to_##_typeB##_new(art_gv,&rgb, target); \
    CC_OPERAND_DEBUGPRINTF( _typeB, target ) \
    CC_END_DEBUGPRINTF( _typeA##_to_##_typeB ) \
}


#define COLOUR_CONVERSION_VIA_RGBA(_TypeA,_typeA,_TypeB,_typeB) \
    _COLOUR_CONVERSION_VIA_RGBA(_TypeA,_typeA,_TypeB,_typeB,)

#define COLOUR_CONVERSION_VIA_RGBA_NEW(_TypeA,_typeA,_TypeB,_typeB) \
    _COLOUR_CONVERSION_VIA_RGBA(_TypeA,_typeA,_TypeB,_typeB,_new)

#define _COLOUR_CONVERSION_VIA_RGBA(_TypeA,_typeA,_TypeB,_typeB,_new) \
void _typeA##_to_##_typeB( \
        const ART_GV      * art_gv, \
        const Ar##_TypeA  * source, \
              Ar##_TypeB  * target \
        ) \
{ \
    CC_START_DEBUGPRINTF( _typeA##_to_##_typeB ) \
    CC_OPERAND_DEBUGPRINTF( _typeA, source ) \
    ArRGBA  rgba = ARRGBA( 0.0, 0.0, 0.0, 0.0 ); \
    _typeA##_to_rgba(art_gv,source, &rgba); \
    rgba_to_##_typeB##_new(art_gv,&rgba, target); \
    CC_OPERAND_DEBUGPRINTF( _typeB, target ) \
    CC_END_DEBUGPRINTF( _typeA##_to_##_typeB ) \
}

#define ARGREY_TO_SPECTRUM(_n) \
void g_to_s##_n ( \
        const ART_GV          * art_gv, \
        const ArGrey          * source, \
              ArSpectrum##_n  * target  \
        ) \
{ \
    s##_n##_d_init_s( art_gv,ARGREY_G(*source), target ); \
}

#define ARGREYALPHA_TO_SPECTRUM(_n) \
void ga_to_s##_n ( \
        const ART_GV          * art_gv, \
        const ArGreyAlpha     * source, \
              ArSpectrum##_n  * target  \
        ) \
{ \
    s##_n##_d_init_s( art_gv,ARGREYALPHA_G(*source), target ); \
}

#define ARGREY8_TO_SPECTRUM(_n) \
void g8_to_s##_n ( \
        const ART_GV          * art_gv, \
        const ArGrey8         * source, \
              ArSpectrum##_n  * target  \
        ) \
{ \
    s##_n##_d_init_s( art_gv,INTCOL_DBL_OF_I8 * (*source), target ); \
}

#define ARGREY16_TO_SPECTRUM(_n) \
void g16_to_s##_n ( \
        const ART_GV          * art_gv, \
        const ArGrey16        * source, \
              ArSpectrum##_n  * target  \
        ) \
{ \
    s##_n##_d_init_s( art_gv,INTCOL_DBL_OF_I16 * (*source), target ); \
}

#define ARGREYALPHA16_TO_SPECTRUM(_n) \
void ga16_to_s##_n ( \
        const ART_GV          * art_gv, \
        const ArGreyAlpha16   * source, \
              ArSpectrum##_n  * target  \
        ) \
{ \
    s##_n##_d_init_s( art_gv,INTCOL_DBL_OF_I8 * ARGREYALPHA16_G(*source), target ); \
}

#define ARGREYALPHA32_TO_SPECTRUM(_n) \
void ga32_to_s##_n ( \
        const ART_GV          * art_gv, \
        const ArGreyAlpha32   * source, \
              ArSpectrum##_n  * target  \
        ) \
{ \
    s##_n##_d_init_s( art_gv,INTCOL_DBL_OF_I16 * ARGREYALPHA32_G(*source), target ); \
}

#define CCV_CIEXYZ_PRIMARY(__i) \
    (&ARCMF_CURVE(*DEFAULT_CMF,(__i)))

#define COLOUR_CONVERSION_S_T_PRIMARY(_stype,_st,_sc,_ptype,_nc,_primfunc,_factor)\
const _stype  ** _st ## _ ## _ptype ## _primary( \
        const ART_GV  * art_gv \
        ) \
{ \
    static _stype  ** spectrum = 0; \
    if ( !spectrum ) \
    { \
        spectrum = ALLOC_ARRAY( _stype *, _nc); \
        for ( int i = 0; i < _nc; i++ ) \
        { \
            spectrum[i] = ALLOC( _stype ); \
            pss_to_ ## _st ( art_gv, _primfunc(i), spectrum[i]); \
            for ( unsigned int j = 0; j < _st ## _channels(art_gv); j++ ) \
                _st ## _set_sid( \
                    art_gv, \
                    spectrum[i], \
                    j, \
                    _factor (art_gv,j) * _st ## _si(art_gv,spectrum[i],j) ); \
        } \
    } \
    return ((const _stype  **) spectrum); \
}

#define COLOUR_CONVERSION_S_TO_XYZ_IMPLEMENTATION(_n) \
COLOUR_CONVERSION_S_T_PRIMARY(ArSpectrum##_n,s##_n,S##_n,ciexyz,3,CCV_CIEXYZ_PRIMARY, \
                             s##_n##_channel_width) \
void s##_n##_to_xyz( \
        const ART_GV          * art_gv, \
        const ArSpectrum##_n  * s_0,  \
              ArCIEXYZ        * xyz_r \
        ) \
{ \
    CC_START_DEBUGPRINTF( s##_n##_to_xyz ) \
    CC_OPERAND_DEBUGPRINTF( s##_n, s_0 ) \
    const ArSpectrum##_n ** primary = s##_n##_ciexyz_primary(art_gv); \
    ArSpectrum##_n  temp; \
    s##_n##_ss_mul_s( art_gv,s_0, primary[0], & temp ); \
    ARCIEXYZ_X(*xyz_r) = s##_n##_s_sum( art_gv, & temp ); \
    s##_n##_ss_mul_s( art_gv,s_0, primary[1], & temp ); \
    ARCIEXYZ_Y(*xyz_r) = s##_n##_s_sum( art_gv, & temp ); \
    s##_n##_ss_mul_s( art_gv,s_0, primary[2], & temp ); \
    ARCIEXYZ_Z(*xyz_r) = s##_n##_s_sum( art_gv, & temp ); \
    CC_OPERAND_DEBUGPRINTF( xyz, xyz_r ) \
    CC_END_DEBUGPRINTF( s##_n##_to_xyz ) \
}


#define SPECTRAL_TO_COLOURTYPE_CONVERSIONS(_n) \
COLOUR_CONVERSION_VIA_XYZ( Spectrum##_n ,s##_n,Grey,g) \
COLOUR_CONVERSION_VIA_XYZ( Spectrum##_n ,s##_n,GreyAlpha,ga) \
COLOUR_CONVERSION_VIA_XYZ( Spectrum##_n ,s##_n,Grey8,g8) \
COLOUR_CONVERSION_VIA_XYZ( Spectrum##_n ,s##_n,Grey16,g16) \
COLOUR_CONVERSION_VIA_XYZ( Spectrum##_n ,s##_n,GreyAlpha16,ga16) \
COLOUR_CONVERSION_VIA_XYZ( Spectrum##_n ,s##_n,GreyAlpha32,ga32) \
COLOUR_CONVERSION_S_TO_XYZ_IMPLEMENTATION(_n) \
COLOUR_CONVERSION_VIA_XYZ( Spectrum##_n ,s##_n,CIEXYZA,xyza) \
COLOUR_CONVERSION_VIA_XYZ( Spectrum##_n ,s##_n,RGB,rgb) \
COLOUR_CONVERSION_VIA_XYZ( Spectrum##_n ,s##_n,RGBA,rgba) \
COLOUR_CONVERSION_VIA_XYZ( Spectrum##_n ,s##_n,RGB24,rgb24) \
COLOUR_CONVERSION_VIA_XYZ( Spectrum##_n ,s##_n,RGBA32,rgba32) \
COLOUR_CONVERSION_VIA_XYZ( Spectrum##_n ,s##_n,RGB48,rgb48) \
COLOUR_CONVERSION_VIA_XYZ( Spectrum##_n ,s##_n,RGBA64,rgba64) \
COLOUR_CONVERSION_VIA_XYZ( Spectrum##_n ,s##_n,RGB96,rgb96) \
COLOUR_CONVERSION_VIA_XYZ( Spectrum##_n ,s##_n,RGBA128,rgba128)

#define COLOUR_CONVERSION_S_TO_S_IMPLEMENTATION(_n0,_n1)\
void s##_n0##_to_s##_n1 ( \
        const ART_GV           * art_gv, \
        const ArSpectrum##_n0  * s0, \
              ArSpectrum##_n1  * sr \
        ) \
{ \
    CC_START_DEBUGPRINTF( s##_n0##_to_s##_n1 ) \
    ArPSSpectrum  pss; \
    s##_n0##_to_pss_new(art_gv, s0, & pss ); \
    pss_to_s##_n1 (art_gv, & pss, sr ); \
    FREE_ARRAY( pss.array ); \
    CC_END_DEBUGPRINTF( s##_n0##_to_s##_n1 ) \
}

#define COLOUR_CONVERSION_S_TO_RSS_IMPLEMENTATION(_n) \
void s##_n##_to_rss_new( \
        const ART_GV          * art_gv, \
        const ArSpectrum##_n  * s0, \
              ArRSSpectrum    * sr \
        ) \
{ \
    CC_START_DEBUGPRINTF( s##_n##_to_rss_new ) \
    sr->size = 2 * s##_n##_channels(art_gv); \
    sr->scale = 1.0; \
    sr->start = s##_n##_channel_lower_bound(art_gv,0); \
    sr->step  = s##_n##_channel_lower_bound(art_gv,1) - s##_n##_channel_lower_bound(art_gv,0); \
    sr->array = ALLOC_ARRAY(double, sr->size); \
    for (unsigned int i = 0; i < s##_n##_channels(art_gv); i++) \
    { \
        sr->array[i] = s##_n##_si(art_gv,s0,i); \
    } \
    CC_END_DEBUGPRINTF( s##_n##_to_rss_new ) \
}

#define COLOUR_CONVERSION_S_TO_PSS_IMPLEMENTATION(_n) \
void s##_n##_to_pss_new( \
        const ART_GV          * art_gv, \
        const ArSpectrum##_n  * s0, \
              ArPSSpectrum    * sr \
        ) \
{ \
    CC_START_DEBUGPRINTF( s##_n##_to_pss_new ) \
    sr->size = 2 * s##_n##_channels(art_gv); \
    sr->scale = 1.0; \
    sr->array = ALLOC_ARRAY(Pnt2D, sr->size); \
    for (unsigned int i = 0; i < s##_n##_channels(art_gv); i++) \
    { \
        sr->array[2 * i] = \
            PNT2D( \
                s##_n##_channel_lower_bound(art_gv,i), \
                s##_n##_si(art_gv,s0,i) ); \
        sr->array[2 * i + 1] = \
            PNT2D( \
                s##_n##_channel_lower_bound(art_gv,i+1), \
                s##_n##_si(art_gv,s0,i)); \
    } \
    CC_END_DEBUGPRINTF( s##_n##_to_pss_new ) \
}

#define COLOUR_CONVERSION_RGBXX_TO_RGB(_n,_a,_A) \
void rgb##_a##_n##_to_rgb( \
        const ART_GV         * art_gv, \
        const ArRGB##_A##_n  * source, \
              ArRGB          * target \
        ) \
{ \
    CC_START_DEBUGPRINTF( rgb##_a##_n##_to_rgb##_a ) \
    ArRGB  rgb; \
\
    rgb##_a##_n##_to_rgb(art_gv,source,&rgb); \
    rgb_to_rgb(art_gv,&rgb,target); \
    CC_END_DEBUGPRINTF( rgb##_a##_n##_to_rgb ) \
}

#define COLOUR_CONVERSION_RGBXX_TO_RGBA(_n,_a,_A) \
void rgb##_a##_n##_to_rgba( \
        const ART_GV         * art_gv, \
        const ArRGB##_A##_n  * source, \
              ArRGBA         * target \
        ) \
{ \
    CC_START_DEBUGPRINTF( rgb##_n##_to_rgba ) \
    ArRGBA  rgba; \
\
    rgb##_a##_n##_to_rgba(art_gv,source,&rgba); \
    rgba_to_rgba(art_gv,&rgba,target); \
    CC_END_DEBUGPRINTF( rgb##_n##_to_rgba ) \
}

#define COLOUR_CONVERSION_COL_TO_S_IMPLEMENTATION(_Type,_type,_n) \
void _type##_to_s##_n( \
        const ART_GV          * art_gv, \
        const Ar##_Type       * val, \
              ArSpectrum##_n  * target \
        ) \
{ \
    CC_START_DEBUGPRINTF( _type##_to_s##_n ) \
    CC_OPERAND_DEBUGPRINTF( _type, val ) \
\
    ArRSSpectrum  temp; \
\
    _type##_to_rss_new( art_gv, val, & temp ); \
\
    rss_to_s##_n( art_gv, & temp, target ); \
\
    rss_free_contents( art_gv, & temp ); \
\
    CC_OPERAND_DEBUGPRINTF( s##_n, target ) \
    CC_END_DEBUGPRINTF( _type##_to_s##_n ) \
}

#define COLOUR_CONVERSION_RSS_TO_S_IMPLEMENTATION(_n) \
void rss_to_s##_n ( \
        const ART_GV          * art_gv, \
        const ArRSSpectrum    * spectrum, \
              ArSpectrum##_n  * target \
        ) \
{ \
    CC_START_DEBUGPRINTF( rss_to_s##_n ) \
    CC_OPERAND_DEBUGPRINTF( rss, spectrum ) \
    ArPSSpectrum  pss; \
    rss_to_pss_new(art_gv,spectrum, &pss); \
    pss_to_s##_n (art_gv, & pss, target ); \
    FREE_ARRAY(pss.array); \
    CC_OPERAND_DEBUGPRINTF( s##_n, target ) \
    CC_END_DEBUGPRINTF( rss_to_s##_n ) \
}

#define COLOUR_CONVERSION_PSS_TO_S_IMPLEMENTATION(_n) \
void pss_to_s##_n ( \
        const ART_GV          * art_gv, \
        const ArPSSpectrum    * spectrum, \
              ArSpectrum##_n  * target \
        ) \
{ \
    CC_START_DEBUGPRINTF( pss_to_s##_n ) \
    CC_OPERAND_DEBUGPRINTF( pss, spectrum ) \
    for ( unsigned int i = 0; i < s##_n##_channels(art_gv); i++ ) \
    { \
        double  value = \
            pss_inner_product( \
                  art_gv, \
                  spectrum, \
                  s##_n##_sample_basis(art_gv,i) \
                ); \
        s##_n##_set_sid( \
            art_gv, \
            target, \
            i, \
            value \
            ); \
    } \
    CC_OPERAND_DEBUGPRINTF( s##_n, target ) \
    CC_END_DEBUGPRINTF( pss_to_s##_n ) \
}


/* ======================================================================== */
