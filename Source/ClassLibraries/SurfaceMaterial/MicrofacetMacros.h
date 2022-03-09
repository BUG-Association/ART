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

#define MICROFACET_DISTRIBUTION    ((ArNode <ArpMicrofacetDistribution>*) ARNUNARY_SUBNODE)

// can be used if there is a ArDirectionCosine microfacetNormalDirection variable
#define MICROFACET_NORMAL_WORLDSPACE ARDIRECTIONCOSINE_VECTOR(microfacetNormalDirection)
#define MICROFACET_COSINE_WORLDSPACE ARDIRECTIONCOSINE_COSINE(microfacetNormalDirection)



// The following functions have since been deprecated through the MicrofacetDistributions module


/* ---------------------------------------------------------------------------
    Microfacet surface model evaluation functions.

    halfVector is the vector between incoming and outgoing light, in
    the surface coordinate system, so its z component equals its dot
    product with the surface normal.  The anisotropic model (aka
    "brushed metal") uses minor and major axes of a reflection ellipse
    aligned parallel to the surface coordinate system.

    Ref: PBR=Pharr/Humphreys, Physically Based Rendering, Section 9.4.2,
    Page 440ff.
---------------------------------------------------------------------------gz- */

double microfacet_geometric_attenuation( const Vec3D *localI,
                                         const Vec3D *localO,
                                         const Vec3D *localH )
{
    // note the mix of o/i, caused by differences with the book
    double normalH = M_ABS( ZC(*localH) );
    double normalI = M_ABS( ZC(*localI) );
    double normalO = M_ABS( ZC(*localO) );

    double OdotH = M_ABS( vec3d_vv_dot(localO, localH) );
    double IdotH = M_ABS( vec3d_vv_dot(localI, localH) );

    return M_MIN3(   1.0,
                   ( 2.0 * normalH*normalO/OdotH ),
                   ( 2.0 * normalH*normalI/IdotH )
                 );
}

// MicrofacetDistribution::D()

double blinn_isotropic_microfacet_value(
        const Vec3D   * localH,
        const double  * exponent
        )
{
    return ( *exponent + 2.0 ) * MATH_1_DIV_2_PI *
            pow( M_MAX( 0.0, M_ABS( ZC( *localH ) ) ), *exponent );
}
#ifdef NEVERMORE
// This is Aniosotropic::D(), but with a correct normalisation factor
double blinn_anisotropic_microfacet_value(const Vec3D *localH,
                                                     const double *ex,
                                                     const double *ey)
{
    double cosThetaH=ZC(*localH);
    double e=( *ex * XC(*localH)*XC(*localH) +
               *ey * YC(*localH)*YC(*localH))
               / (1.0-cosThetaH*cosThetaH);
    // GZ: added M_MAX and a fix to the normalisation factor over the definition given in the book:
    return sqrt((*ex+2)*(*ey+2))*MATH_1_DIV_2_PI *  pow(M_MAX(0.0, ZC(*localH)), e);
}

// Another usable microfacet distribution from
// http://www.unc.edu/courses/2004spring/comp/236/001/Lecture12.pdf
// beta is angle between n and h,
// m is root-mean-square of slope
// Note that $m_{Beckmann}=0.2 \approx e_{Blinn}=20, m_{Beckmann}=0.6 \approx e_{Blinn}=4$,
// so the roles are shifted. Also, Beckmann is slightly more retroreflecting.
// Unfortunately, the function is not revertible, so proper importance sampling is not possible.

ART_INLINE double beckmann_isotropic_microfacet_value(const Vec3D *localH,
                                                      const double *m)
{
    double cosBeta=(ZC(*localH));
    return pow(MATH_E, -M_SQR(tan(acos(cosBeta)) / *m )) / (4.0*M_SQR(*m)*M_SQR(M_SQR(cosBeta)));
}


#undef ART_INLINE
#endif

