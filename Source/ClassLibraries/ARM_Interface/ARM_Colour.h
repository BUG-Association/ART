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
/**
 * @file ARM_Colour.h
 * @brief Colours and Spectra
 * @type Spectrum
 */

#import "ART_Foundation.h"

ART_MODULE_INTERFACE(ARM_Colour)

#import "ART_ColourAndSpectra.h"

#import "ARM_Foundation.h"

/**
 * @brief Regularly spaced spectral values
 *
 * This type of data is what measurement devices usually produce. Wavelengths
 * have to be tagged with the appropriate physical units, e.g.
 * 380 \verb?NANOMETERS?, or 380.0 \verb?NM? for short.
 *
 * Note that the maximum value argument is not the maximum value of the given
 * samples (which could be eas- ily computed), but the theoretical maximum
 * value of the whole spectrum. This information is needed to normalise the
 * spectrum. After the sample values (which are of type \verb?double?), there
 * MUST follow the end tag \verb?RSS_END? as last argument of the macro,
 * otherwise the \verb?vararg? routines will be unable to process the sample
 * list. A symptom of such an omission can be a make: \verb?error 139? (i.e.
 * runaway vararg) message during an \verb?arm->art? translation.
 *
 * @def CONST_COLOUR_RSSPECTRUM(start, sample_dist, max, sample0, sample1, ..., RSS_END)
 *
 * \paragraph{Example:}
 * \begin{verbatim}
 * CONST_COLOUR_RSSPECTRUM(400 NANOMETER, 100 NANOMETER, 1.0,
 *     0.0, 8.0, 15.6, 18.4,
 *     RSS_END
 *     );
 * \end{verbatim}
 *
 * @param start         double  Starting wavelenght (in nanometers).
 * @param sample_dist   double  Sampling distance (in nanometers).
 * @param max           double  Maximum value of the spectrum, used for normalisation.
 * @param sample0       double  Sample.
 * @param ...           double  Sample(s)...
 * @param PSS_END       -       Mandatory ending tag to let ART detect the size of the sample set.
 */
#define CONST_COLOUR_RSSPECTRUM( start...) \
    arnconstrsspectrumvalue( art_gv, ## start )

/**
 * @brief Point sampled spectrum
 *
 * These can have arbitrarily spaced samples and are basically a polyline,
 * which is more con- venient for manually constructed spectra. The samples are
 * of type \verb?Pnt2D?, and the maximum value argument has the same
 * significance as for regularly spaced spectra. As in the example, specifying
 * the end tag \verb?PSS_END? is crucial.
 *
 * @def CONST_COLOUR_PSSPECTRUM(max, sample1, sample2, ..., PSS_END)
 *
 * \paragraph{Example:}
 * \begin{verbatim}
 * CONST_COLOUR_PSSPECTRUM(1.0,
 *     PNT2D( 300.0 NANOMETER, 0.0 ),
 *     PNT2D( 440.0 NANOMETER, 0.0 ),
 *     PNT2D( 450.0 NANOMETER, 1.0 ),
 *     PNT2D( 460.0 NANOMETER, 0.0 ),
 *     PNT2D( 800.0 NANOMETER, 0.0 ),
 *     PSS_END
 *     );
 * \end{verbatim}
 *
 * @param max       double  Maximum value of the spectrum, used for normalisation.
 * @param sample0   Pnt2D   Sample with the form \verb?PNT2D(wavelength NM, value)?.
 * @param ...       Pnt2D   Sample(s)...
 * @param PSS_END   -       Mandatory ending tag to let ART detect the size of the sample set.
 */
#define CONST_COLOUR_PSSPECTRUM( start...) \
    arnconstpsspectrumvalue( art_gv, ## start )

/**
 * @brief RGB tristimulus spectrum without colour space
 *
 * The colour is assumed to be in the default colour space used during
 * rendering (i.e. a floating reference).
 *
 * @def CONST_COLOUR_RGB(r, g, b)
 *
 * @param r     double      Red channel value.
 * @param g     double      Green channel value.
 * @param b     double      Blue channel value.
 */
#define CONST_COLOUR_RGB( _r, _g, _b ) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnVal_const_utf_rgb) \
        : ARUTF_RGB( _r, _g, _b ) \
    ]

/**
 * @brief Grey spectrum
 *
 * This creates a flat spectrum with the same value for every wavelength
 *
 * @def CONST_COLOUR_GREY(value)
 * @def CONST_COLOUR_GRAY(value)
 *
 * @param value double  Value of the spectral at all wavelengths.
 */
#define CONST_COLOUR_GREY( _value ) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnVal_const_g) \
        : ARGREY(_value) \
    ]

#define CONST_COLOUR_GRAY CONST_COLOUR_GREY

/**
 * @brief Blackbody radiator spectrum
 *
 * This creates a spectrum based on the colour temperature provided.
 *
 * @def CONST_BLACKBODY_RADIATOR(temperature)
 *
 * \paragraph{Example:}
 *
 * \begin{verbatim}
 * CONST_BLACKBODY_RADIATOR(6500 KELVIN)
 * \end{verbatim}
 *
 * @param temperator    double  Colour temperature to use for generating the blackbody spectrum (in Kelvin).
 */
#define CONST_BLACKBODY_RADIATOR( _temperature ) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnBlackbodyRadiation) \
        : (_temperature) \
    ]

/**
 * @brief Gaussian spetrum
 *
 * Creates a Gaussian spectrum.
 *
 * @def CONST_GAUSSIAN_COLOUR(center, sigma, max)
 *
 * @param center    double  Main wavelenth, expected value of the Gaussian (in nanometers).
 * @param sigma     double  Root squared variance.
 * @param max       double  Maximum value of the Gaussian.
 */
#define CONST_GAUSSIAN_COLOUR( _center, _sigma, _max ) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnGaussianSpectrum) \
        : (_center) \
        : (_sigma) \
        : (_max) \
        ]

/**
 * @section Fluorescent and 2D spectra
 */

    /**
     * @brief Spectrum from reradiation matrix
     *
     * This takes a reradiation matrix to define a fluorescent spectra. Each line
     * of the matrix must be terminated with \verb?RSS2D_LINE_END?. The matrix has to be ended with \verb?RSS2D_END?.
     *
     * @def CONST_COLOUR_RSSPECTRUM2D(a_start, a_step, e_start, a_end, max, samples, ..., RSS2D_END)
     *
     * \paragraph{Example:}
     * \begin{verbatim}
     * CONST_COLOUR_RSSPECTRUM2D(
     *     300 NM, 40 NM,
     *     300 NM, 40 NM, 1.0,
     *
     *     0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, RSS2D_LINE_END,
     *     0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, RSS2D_LINE_END,
     *     0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, RSS2D_LINE_END,
     *     0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, RSS2D_LINE_END,
     *     0.000, 0.600, 0.000, 0.000, 0.000, 0.000, 0.000, RSS2D_LINE_END,
     *     0.000, 0.000, 1.000, 0.000, 0.000, 0.000, 0.000, RSS2D_LINE_END,
     *     0.015, 0.012, 0.000, 1.000, 0.000, 0.000, 0.000, RSS2D_LINE_END,
     *     0.025, 0.015, 0.000, 0.000, 1.000, 0.000, 0.000, RSS2D_LINE_END,
     *     0.015, 0.010, 0.000, 0.000, 0.000, 1.000, 0.000, RSS2D_LINE_END,
     *     0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 1.000, RSS2D_LINE_END,
     *     RSS2D_END
     *     );
     * \end{verbatim}
     *
     * @param a_start           double  Exitation sampling start (in nanometers).
     * @param a_step            double  Exitation sampling distance (in nanometers).
     * @param e_start           double  Re-emission sampling start (in nanometers).
     * @param e_step            double  Re-emission sampling distance (in nanometers).
     * @param max               double  Maximum value of the spectrum, used for normalisation.
     * @param sample            double  Sample.
     * @param ...               double  Sample(s).
     * @param RSS2D_LINE_END    -       Mandatory ending tag for a line.
     * @param ...               double  Sample(s).
     * @param RSS2D_END         -       Mandatory ending tag for the matrix.
     */
    #define CONST_COLOUR_RSSPECTRUM2D( start...) \
        arnconstrsspectrum2Dvalue( art_gv, ## start )

    #define CONST_COLOUR_RSS2D \
        CONST_COLOUR_RSSPECTRUM2D

    /**
     * @brief Synthetic fluorescent spectrum
     *
     * Defines a synthetic fluorescent spectrum. Build from non fluorescent part and
     * characterised by the absorption and reemission spectrum to build a
     * reradiation matrix.
     *
     * @def CONST_FLUORESCENT_COLOUR(diagonal, fluo_power, stoke, expand)
     *
     * \paragraph{Example:} Builds a fluorescent spectrum with a main diagonal build
     * from an RGB colour, having an absorption peak at 360nm and reemission peak
     * at 560nm with an aborption bandwidth of 40nm and reemission bandwidth of
     * 50nm.
     *
     * \begin{verbatim}
     * CONST_FLUORESCENT_COLOUR(
     *     CONST_COLOUR_RGB(0.2, 0.6, 0.04),
     *     0.0025,
     *     PNT2D( 360 NM, 560 NM ), //340 NM
     *     VEC2D(  40 NM,  50 NM )
     *     );
     * \end{verbatim}
     *
     * @param diagonal      Spectrum    The non fluorescent part of the spectrum.
     * @param fluo_power    double      The maximum of the crosstalk.
     * @param stoke         Pnt2D       The absorption and reemission peaks (in nanometers).
     * @param expand        Vec2D       The bandwidth of the absorption and reemission spectrum (in nanometers).
     */
    #define CONST_FLUORESCENT_COLOUR( _mainDiagonal, _cD, _cC, _cE ) \
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnSyntheticFluorescentSpectrum) \
            : HARD_NODE_REFERENCE(_mainDiagonal) \
            : (_cD) \
            : (_cC) \
            : (_cE) \
            ]
/**
 * @popsection
 */

#define CONST_REFLECTIVITY_SPECTRUM( _spectrum, _length ) \
    CONST_ABSORBANCE_SPECTRUM( \
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnAbsorbanceFromReflectivity) \
            :   (_spectrum) \
            ], \
        (_length) \
        )

#define CONST_ABSORBANCE_SPECTRUM( _spectrum, _length ) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnExtinctionFromAbsorbance) \
        :   (_spectrum) \
        :   (_length) \
        ]

/**
 * @brief Exponential operation on spetrum
 *
 * This returns an exponentiated version of a given spectrum.
 *
 * @def CONST_EXPONENTIATED_COLOUR(spectrum, exponent)
 *
 * @param spectrum    Spectrum    Given spectrum to exponentiate.
 * @param exponent    double      Power to apply for exponentiation.
 */
#define CONST_EXPONENTIATED_COLOUR(_c, _e) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnExponentiatedColour) \
        :   (_c) \
        :   (_e) \
        ]

#define CONST_ABSORPTION_COLOUR(_c, _e) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnExponentiatedColour) \
        :   (_c) \
        :   (1.0/(_e)) \
        ]

#define CONST_SELLMEIER_DISPERSION_4(_a,_b,_c,_d) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnSellmeierDispersion) \
        :(_a) \
        :(_b) \
        :(_c) \
        :(_d) \
        ]

#define CONST_SELLMEIER_DISPERSION_5(_a,_b,_c,_d,_e) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnSellmeierDispersion) \
        :(_a) \
        :(_b) \
        :(_c) \
        :(_d) \
        :(_e) \
        ]

#define CONST_SELLMEIER_DISPERSION_6(_a,_b,_c,_d,_e,_f) \
    [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnSellmeierDispersion) \
        :(_a) \
        :(_b) \
        :(_c) \
        :(_d) \
        :(_e) \
        :(_f) \
        ]

/**
 * @section Spectrum loading from files
 */
    /**
     * @brief Image Map
     *
     * Spectrum from image. The spectrum is then spatially varying according to the
     * image loaded.
     *
     * @def IMAGE_MAP(filename, scaleFactor)
     *
     * @param filename      char*           Path to the file to load.
     * @param scaleFactor   double          Scaling of the image.
     */
    #define IMAGE_MAP( _fn, _sf ) \
        [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnImageMap) \
            : [ ALLOC_INIT_OBJECT_AUTORELEASE(ArnExternal) \
                : arsymbol(art_gv,_fn) \
                : ARPROTOCOL(ArpImageFile) \
                ] \
            : _sf \
        ]


    #define ARP_COLOUR       ARPROTOCOL(ArpSpectrum)
    #define ARP_COLOUR_VAL   ARPROTOCOL(ArpSpectrumValues)

    /**
     * @brief Load spectrum from file
     *
     * Loads a spectrum from a provided file.
     *
     * @def EXTERNAL_NAMED_COLOUR(path, name)
     *
     * @param path  char*   Path of the file to load.
     * @param name  char*   Name of the spectrum to load from the file.
     */
    #define EXTERNAL_NAMED_COLOUR(__path,__name) \
        ((ArNode <ArpSpectrum> *)EXTERNAL((__path),(__name), \
        ARP_COLOUR,ARP_COLOUR_VAL))

    /**
     * @brief Load spectrum from archive
     *
     * Loads a spectrum from a provided archive.
     *
     * @def EXTERNAL_COLOUR_ARCHIVE(path)
     *
     * @param path  char*   Path of the archive to load.
     */
    #define EXTERNAL_COLOUR_ARCHIVE(__path) \
        ((ArnNamedNodeSet *)EXTERNAL((__path),0, \
        ARPROTOCOL(ArpNamedNodeSet),0))

    #define EXTERNAL_COLOUR(__path) \
        EXTERNAL((__path),0,ARP_COLOUR,ARP_COLOUR_VAL)
/**
 * @popsection
 */


// ===========================================================================
