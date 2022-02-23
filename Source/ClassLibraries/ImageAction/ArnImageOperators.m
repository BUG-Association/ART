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

#define ART_MODULE_NAME     ArnImageOperators

#import "ArnImageOperators.h"
#import "ART_ImageData.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArnPlainImage (Dithering)

- (ArnRGBA32Image *) fractionDitheredRGBA32Image
        : (ArnRGBA32Image *) ditherImage
        : (ArcObject <ArpReporter> *) reporter
{
    long  dIX = XC([ditherImage size]);
    long  dIY = YC([ditherImage size]);

    [ reporter beginAction: "ordered dithering" ];

    ArnRGBImage  * rgbImage =
        [ ALLOC_OBJECT(ArnRGBImage) initAsPartOf: self ];

    IVec2D imageSize  = [ rgbImage size ];

    ArnRGBA32Image  * rgba32Image;

    rgba32Image = [ ALLOC_OBJECT(ArnRGBA32Image) initWithSize: imageSize ];

    for ( long y = 0; y < YC(imageSize); y++ )
    {
        long     dy = y % dIY;
        ArRGB  * rgbLine = [ rgbImage line: y ];

        ArRGBA32  * ditherLine = [ ditherImage line: dy ];
        ArRGBA32  * rgba32Line  = [ rgba32Image  line: y  ];

        for ( long x = 0; x < XC(imageSize); x++ )
        {
            long  dx = x % dIX;

            for ( int k = 0; k < 3; k++ )
            {
                double  byteval =
                        M_CLAMP(ARRGB_CI(rgbLine[x],k), 0.0, 1.0) * 255.0;
                UInt8  bytefrac = (UInt8)(255.0 * (byteval - floor(byteval)));

                if ( bytefrac <= IC_CI(ditherLine[dx],k))
                    byteval = floor(byteval);
                else
                    byteval = floor(byteval) + 1;

                IC_CI(rgba32Line[x],k) = (UInt8)byteval;
            }
            IAC(rgba32Line[x]) = 255;
        }
    }
    [ reporter endAction ];
    RELEASE_OBJECT( rgbImage );
    return rgba32Image;
}

@end


@implementation ArnCIEXYZAImage (Tonemapping)

- (void) gammaTransform
        : (double) gamma
        : (ArcObject <ArpReporter> *) reporter
{
    double  exponent = 1.0 / gamma;

    if ( gamma == 0.0 || gamma == 1.0 )
        return;

    [ reporter beginAction
        :   "gamma correction [g=%g]"
        ,   gamma
        ];

    for ( long y = 0; y < YC( size ); y++ )
    {
        ArCIEXYZA * line = [ self line: y ];

        for ( long x = 0; x < XC( size ); x++ )
        {
            ArCIExyY  temp_xyY;

            xyz_to_xyy( art_gv, & ARCIEXYZA_C(line[x]),  & temp_xyY );

            ARCIExyY_Y( temp_xyY ) = m_dd_pow( ARCIExyY_Y( temp_xyY ), exponent);

            xyy_to_xyz( art_gv, & temp_xyY, & ARCIEXYZA_C(line[x]) );
        }
    }

    [ reporter endAction ];
}

- (void) exponentialMapping
        : (double) mappingValue
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
{
    double averageLuminance;

    averageLuminance = (mappingValue != 0.0) ? mappingValue :
                                               [metrics averageLuminance];

    [ reporter beginAction
        :   "exponential mapping"
        ];

    for ( long y = 0; y < YC( size ); y++ )
    {
        ArCIEXYZA * line = [ self line: y ];

        for ( long x = 0; x < XC( size ); x++ )
        {
            ArCIExyY  temp_xyY;

            xyz_to_xyy( art_gv, & ARCIEXYZA_C(line[x]),  & temp_xyY );

            ARCIExyY_Y( temp_xyY ) = 1.0 - exp(  -ARCIExyY_Y( temp_xyY )
                                               / averageLuminance );

            xyy_to_xyz( art_gv, & temp_xyY, & ARCIEXYZA_C(line[x]) );
        }
    }

    [ reporter endAction ];
}

- (void) meanValueMapping
        : (double) mappingValue
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
{
    double  averageLuminance = [ metrics averageLuminance ];

    [ reporter beginAction
        :   "mean value mapping [p=%g]"
        ,   mappingValue
        ];

    for ( long y = 0; y < YC( size ); y++ )
    {
        ArCIEXYZA * line = [ self line: y ];

        for ( long x = 0; x < XC( size ); x++ )
        {
            ArCIExyY  temp_xyY;

            xyz_to_xyy( art_gv, & ARCIEXYZA_C(line[x]),  & temp_xyY );

            ARCIExyY_Y( temp_xyY ) = (   ARCIExyY_Y( temp_xyY )
                                       / averageLuminance ) * mappingValue;

            xyy_to_xyz( art_gv, & temp_xyY, & ARCIEXYZA_C(line[x]) );
        }
    }

    [ reporter endAction ];
}

- (void) interactiveCalibration
        : (double) aperture
        : (double) contrast
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
{
    double  start = (    m_dd_pow( 2.0, aperture + 1.0 )
                       * [ metrics averageLuminance ] )
                    / ( 1.0 + contrast );

    double  end   = start * contrast;

    double  a     = 1.0 / ( end - start );

    double  b     = start / ( start - end );

    [ reporter beginAction
        :   "interactive calibration"
        ];

    [ reporter printf
        :   "aperture/contrast = %g / %g\n"
        ,   aperture
        ,   contrast ];

    [ reporter beginAction
        :   "performing..."
        ];

    for ( long y = 0; y < YC( size ); y++ )
    {
        ArCIEXYZA * line = [ self line: y ];

        for ( long x = 0; x < XC( size ); x++ )
        {
            ArCIExyY  temp_xyY;

            xyz_to_xyy( art_gv, & ARCIEXYZA_C(line[x]),  & temp_xyY );

            ARCIExyY_Y( temp_xyY ) = ARCIExyY_Y( temp_xyY ) * a + b;

            xyy_to_xyz( art_gv, & temp_xyY, & ARCIEXYZA_C(line[x]) );
        }
    }

    [ reporter endAction : "done." ];
    [ reporter endAction ];
}

#define PAR_N 256
#define PAR_M 10

- (void) schlicksMapping
        : (double) p_in
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
{
    double  maximumLuminance = [ metrics maximumLuminance ];
    double  p = ( p_in != 0.0 ? p_in : ( PAR_M / PAR_N ) * maximumLuminance) ;

    [ reporter beginAction
        :   "Schlick's mapping [p=%g]"
        ,   p
        ];

    for ( long y = 0; y < YC( size ); y++ )
    {
        ArCIEXYZA * line = [ self line: y ];

        for ( long x = 0; x < XC( size ); x++ )
        {
            ArCIExyY  temp_xyY;

            xyz_to_xyy( art_gv, & ARCIEXYZA_C(line[x]),  & temp_xyY );

            ARCIExyY_Y( temp_xyY ) =
                  ( p * ARCIExyY_Y( temp_xyY ) )
                / (   p * ARCIExyY_Y( temp_xyY )
                    - ARCIExyY_Y( temp_xyY )
                    + metrics->maximumLuminance );

            xyy_to_xyz( art_gv, & temp_xyY, & ARCIEXYZA_C(line[x]) );
        }
    }

    [ reporter endAction ];
}

- (void) wardsMapping
        : (double) lwa_in
        : (double) ldmax
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
{
    double  lwa = ( lwa_in != 0.0 ? lwa_in : [ metrics adaptationLuminance ] );

    double  m   =   m_dd_pow(   (   1.219
                                  + m_dd_pow( ldmax / 2.0, 0.4 ) )
                              / (   1.219
                                  + m_dd_pow( lwa, 0.4 ) )
                              , 2.5 )
                  / ldmax;

    if ( lwa_in != 0.0 )
        [ reporter printf
            :   "user specified adaption luminance = %g\n"
            ,   lwa_in ];

    [ reporter beginAction
        :   "Ward's mapping [Ldmax=%g]"
        ,   ldmax
        ];

    for ( long y = 0; y < YC( size ); y++ )
    {
        ArCIEXYZA * line = [ self line: y ];

        for ( long x = 0; x < XC( size ); x++ )
        {
            ArCIExyY  temp_xyY;

            xyz_to_xyy( art_gv, & ARCIEXYZA_C(line[x]),  & temp_xyY );

            ARCIExyY_Y( temp_xyY ) *= m;

            xyy_to_xyz( art_gv, & temp_xyY, & ARCIEXYZA_C(line[x]) );
        }
    }

    [ reporter endAction ];
}

static double alpha(double logLa)
    { return 0.4   * logLa + 2.92; }

static double beta(double logLa)
    { return -0.4 * logLa * logLa - 2.584 * logLa + 2.0208; }

- (void) tumblinRushmeierMapping
        : (double) lwa_in
        : (double) ldmax
        : (double) cmax
        : (ArcImageMetrics *) metrics
        : (ArcObject <ArpReporter> *) reporter
{
    double lwa = (lwa_in != 0.0 ? lwa_in : [metrics adaptationLuminance]);
    double lda =        ldmax/sqrt(cmax);

    double logLwa =     log10(lwa*3.14e-4);
    double alpharw =    alpha(logLwa);
    double betarw =     beta(logLwa);

    double logLda =     log10(lda*3.14e-4);
    double alphad =     alpha(logLda);
    double betad =      beta(logLda);

    double lrwexponent = alpharw/alphad;
    double lrwmultip   = m_dd_pow(10.0,(betarw-betad)/alphad)/(3.14e-4*ldmax);
    double invcmax     = 1.0/cmax;

    if (lwa_in != 0.0)
        [reporter printf: "user specified adaption luminance [Lwa=%g]\n",
                        lwa_in];
    [ reporter beginAction
        :   "Tumblin & Rushmeier's mapping"
        ];

    [reporter printf :"Lda/Ldmax/Cmax = %g / %g / %g\n", lda, ldmax, cmax];
    [ reporter beginAction
        :   "performing..."
        ];

    for ( long y = 0; y < YC(size); y++ )
    {
        ArCIEXYZA * line = [ self line: y ];
        for ( long x = 0; x < XC(size); x++ )
        {
            ArCIExyY  temp_xyY;

            xyz_to_xyy( art_gv, & ARCIEXYZA_C(line[x]),  & temp_xyY );

            double pixLum = ARCIExyY_Y( temp_xyY );

            double disLum = m_dd_pow(pixLum*3.14e-4,lrwexponent) * lrwmultip
                            - invcmax;
            double m = (pixLum > 0.0 ? disLum/pixLum : 0.0);

            ARCIExyY_Y( temp_xyY ) *= m;

            xyy_to_xyz( art_gv, & temp_xyY, & ARCIEXYZA_C(line[x]) );
        }
    }
    [reporter endAction :"done."];
    [reporter endAction];
}

@end

// ===========================================================================
