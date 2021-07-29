/* ===========================================================================

 Copyright (c) 1996-2021 The ART Development Team
 -------------------------------------------

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

#define ART_MODULE_NAME     ArnPDFSampling

#import "ArnPDFSampling.h"
#import "SurfaceMaterialMacros.h"

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    [ArnDiracPDF registerWithRuntime];
    [ArnUniformPDF registerWithRuntime];
    [ArnBlinnPDF registerWithRuntime];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

/* ===========================================================================
    'ArnDiracPDF'
=========================================================================== */

@implementation ArnDiracPDF

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnDiracPDF)

- (void) getRandomDirection
        : (Vec3D *)       localO
        : (Vec3D *)       localH
        : (double *)      pdfValue
    for : (const Vec3D *) localI
        : (const double)  u1
        : (const double)  u2
{
    (void) pdfValue;
    (void) u1;
    (void) u2;
    
    // For a Dirac impulse, there is only one possible direction, the
    // mirror direction

    ZC( *localO ) = -ZC( *localO );

    vec3d_vv_sub_v( localI, localO, localH );
    vec3d_norm_v(localH);
}

- (void) getRandomDirection
        : (Vec3D *)       localO
        : (double *)      pdfValue
    for : (const Vec3D *) localI
        : (const double)  u1
        : (const double)  u2
{
    (void) pdfValue;
    (void) localI;
    (void) u1;
    (void) u2;
    
    // For a Dirac impulse, there is only one possible direction, the
    // mirror direction

    ZC( *localO ) = -ZC( *localO );
}

- (double) valueForDirections
        : (const Vec3D *) localI
        : (const Vec3D *) localO
{
    (void) localI;
    (void) localO;
    
    // The PDF is always zero since it is staistically impossible
    // to sample a singe direction

    return 0.0;
}

// preferrably use this if you know the Halfway vector
- (double) valueForDirections
        : (const Vec3D *) localI
        : (const Vec3D *) localO
        : (const Vec3D *) localH
{
    (void) localI;
    (void) localO;
    (void) localH;
    
    // The PDF is always zero since it is staistically impossible
    // to sample a singe direction

    return 0.0;


}

@end

/* ===========================================================================
    'ArnUniformPDF'
=========================================================================== */

@implementation ArnUniformPDF

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnUniformPDF)

// return direction v from uniform samples [0...[1

- (void) getRandomDirection
        : (Vec3D *)       localO
        : (Vec3D *)       localH
        : (double *)      pdfValue
    for : (const Vec3D *) localI
        : (const double)  u1
        : (const double)  u2
{
    //Uniform sampling of a disk
    double sx, sy, r, theta;

    sx = 2.0 * u1 - 1;
    sy = 2.0 * u2 - 1; // Map square to ((r, [Theta]))

    // Handle degeneracy at the origin
    if ( (sx == 0) && (sy == 0))
    {
        r = 0.0;
        theta = 0.0;
    }
    else if ( sx >= (-sy) )
    {
        //Handle first region of disk
        if (sx > sy)
        {
            r = sx;
            theta=( (sy > 0.0) ? sy/r : 8.0 + sy/r );
        }
        // Handle second region of disk
        else
        {
            r = sy;
            theta = 2.0 - sx/r;
        }
    }
    else
    {
        // Handle third region of disk
        if (sx <= sy)
        {
            r=-sx;
            theta= 4.0 - sy/r;
        }
        //  Handle fourth region of disk
        else
        {
            r=-sy;
            theta= 6.0 + sx/r;
        }
    }

    theta *= MATH_PI_DIV_4;

    XC( *localO ) = r * cos( theta );
    YC( *localO ) = r * sin( theta );
    ZC( *localO ) = sqrt( M_MAX ( 0.0, 1.0 - M_SQR( XC(*localO) )
                        - M_SQR( YC(*localO) ) ) );

    vec3d_vv_sub_v( localI, localO, localH );
    vec3d_norm_v(localH);

    *pdfValue = ZC(*localO) * MATH_1_DIV_PI;
}

- (double) valueForDirections
        : (const Vec3D *) localI
        : (const Vec3D *) localO
{
    if ( SAME_HEMISPHERE( *localI, *localO ) )
        return ZC( *localO ) * MATH_1_DIV_PI;
    else
        return 0.0;
}

- (void) getRandomDirection
        : (Vec3D *)       localO
        : (double *)      pdfValue
    for : (const Vec3D *) localI
        : (const double)  u1
        : (const double)  u2
{
    // For a Dirac impulse, there is only one possible direction, the
    // mirror direction

        //Uniform sampling of a disk
    double sx, sy, r, theta;

    sx = 2.0 * u1 - 1;
    sy = 2.0 * u2 - 1; // Map square to ((r, [Theta]))

    // Handle degeneracy at the origin
    if ( (sx == 0) && (sy == 0))
    {
        r = 0.0;
        theta = 0.0;
    }
    else if ( sx >= (-sy) )
    {
        //Handle first region of disk
        if (sx > sy)
        {
            r = sx;
            theta=( (sy > 0.0) ? sy/r : 8.0 + sy/r );
        }
        // Handle second region of disk
        else
        {
            r = sy;
            theta = 2.0 - sx/r;
        }
    }
    else
    {
        // Handle third region of disk
        if (sx <= sy)
        {
            r=-sx;
            theta= 4.0 - sy/r;
        }
        //  Handle fourth region of disk
        else
        {
            r=-sy;
            theta= 6.0 + sx/r;
        }
    }

    theta *= MATH_PI_DIV_4;

    XC( *localO ) = r * cos( theta );
    YC( *localO ) = r * sin( theta );
    ZC( *localO ) = sqrt( M_MAX ( 0.0, 1.0 - M_SQR( XC(*localO) )
                        - M_SQR( YC(*localO) ) ) );

    *pdfValue = ZC(*localO) * MATH_1_DIV_PI;

}

- (double) valueForDirections
        : (const Vec3D *) localI
        : (const Vec3D *) localO
        : (const Vec3D *) localH
{
    if ( SAME_HEMISPHERE( *localI, *localO ) )
        return ZC( *localO ) * MATH_1_DIV_PI;
    else
        return 0.0;

}

@end

/* ===========================================================================
    'ArnBlinnPDF'
=========================================================================== */

@implementation ArnBlinnPDF

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnBlinnPDF)

- (id) init
        : (double) newExponent
{
    self = [super init];

    if ( self )
    {
        exponent = newExponent;
    }
    
    return self;
}

- (id) copy
{
    ArnBlinnPDF  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnBlinnPDF  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [super code :coder];
    [coder codeDouble :&exponent];
}

- (void) getRandomDirection
        : (Vec3D *)       localO
        : (double *)      pdfValue
    for : (const Vec3D *) localI
        : (const double)  u1
        : (const double)  u2
{
    // Compute sampled half-angle vector localH for Blinn distribution
    double cosThetaH = pow( u1, 1.0 / ( exponent + 1.0 ));
    assert( cosThetaH <= 1.0 );

    double sinThetaH = sqrt( 1.0 - M_SQR(cosThetaH) );
    double phi = u2 * MATH_2_MUL_PI;

    Vec3D localH = VEC3D( sinThetaH * cos( phi ), sinThetaH * sin( phi ), cosThetaH );

    if (vec3d_vv_dot( localI, &localH ) > 0.0 )
        vec3d_d_mul_v( -1.0, &localH );

    // find localO as localI reflected around localH.
    *localO = localH;

    // note the negative sign as consequence of localI direction pointing towards surface:
    vec3d_d_mul_v( -2.0 * vec3d_vv_dot( localI, &localH ), localO );
    vec3d_v_add_v( localI, localO );

    // Compute PDF for localO from Blinn distribution
    *pdfValue =  ( exponent + 2.0 ) * pow( ZC( localH ), exponent ) /
        ( MATH_2_MUL_PI * 4.0 * ( -vec3d_vv_dot( localI, &localH ) ) );
}

// use this preferably if you need the H vector afterwards
- (void) getRandomDirection
        : (Vec3D *)       localO
        : (Vec3D *)       localH
        : (double *)      pdfValue
    for : (const Vec3D *) localI
        : (const double)  u1
        : (const double)  u2
{   // like PBR p. 682, Blinn::Sample_f():

    assert( ZC(*localI) <= 0 );

    // Compute sampled half-angle vector localH for Blinn distribution

    double cosThetaH = pow( u1, 1.0 / (exponent + 1.0) );
    assert( cosThetaH <= 1.0 );

    double sinThetaH = sqrt(1.0 - M_SQR( cosThetaH ) );
    double phi = u2 * MATH_2_MUL_PI;

    *localH = VEC3D( sinThetaH * cos( phi ), sinThetaH * sin( phi ), cosThetaH );

    // Bring local vectors to unit length and evaluate model
    vec3d_norm_v( localH );

    if (vec3d_vv_dot( localI, localH ) > 0.0 )
        vec3d_d_mul_v( -1.0, localH );

    assert( vec3d_v_sqrlen( localH ) - 1.0 < 0.015 );
    assert( vec3d_v_sqrlen( localI ) - 1.0 < 0.015 );

    // find localO as localI reflected around localH.
    *localO = *localH;

    // note the negative sign as consequence of localI direction pointing towards surface:
    vec3d_d_mul_v( -2.0 * vec3d_vv_dot( localI, localH ), localO );
    vec3d_v_add_v( localI, localO );

    assert( vec3d_v_sqrlen(localO) - 1.0 < 0.015 );

    // Compute PDF for localO from Blinn distribution
    *pdfValue = ( exponent + 1.0 ) * pow( ZC(*localH), exponent ) /
        ( MATH_2_MUL_PI * 4.0 * ( -vec3d_vv_dot(localI, localH) ) );
}


- (double) valueForDirections
        : (const Vec3D *) localI
        : (const Vec3D *) localO
{
    assert(ZC(*localI) <= 0);
    assert(ZC(*localO) >= 0);

    DEFINE_LOCAL_HALFANGLE( localI, localO );

    // note the negation of the dot product as consequence
    // of localI direction pointing towards surface:
    return ( exponent + 1.0 ) * pow( ZC( localH ), exponent ) /
        ( MATH_2_MUL_PI * 4.0 * (-vec3d_vv_dot( localI, &localH ) ) );
}

// preferrably use this if you know the Halfway vector
- (double) valueForDirections
        : (const Vec3D *) localI
        : (const Vec3D *) localO
        : (const Vec3D *) localH
{
    // note the negation of the dot product as consequence of
    // localI direction pointing towards surface:
    assert( ZC( *localI ) <= 0 );

   // if ( ZC( *localH ) > 0  ) printf("%f ", ZC( *localH )  );

    return M_MAX( ( exponent + 1.0 ) * pow( ZC( *localH ), exponent ) /
        ( MATH_2_MUL_PI * 4.0 * (-vec3d_vv_dot( localI, localH )) ), 0.0 );

}


@end

/* ===========================================================================
    'ArnHTSGAdaptedPDF'
=========================================================================== */

@implementation ArnHTSGAdaptedPDF

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnHTSGAdaptedPDF)

- (id) copy
{
    ArnHTSGAdaptedPDF  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnHTSGAdaptedPDF  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

    return copiedInstance;
}

/*
    av1       - average constant for part of pdf corresponding to specular term
    av2       - average constant for part of pdf corresponding to dir.-diffuse term
    fact      - adaptation factor for the whole pdf
    k_norm    - normalization constant
*/
#define av1     1000.0
#define av2     100000.0
#define fact    150.0
double C1, expC1, C2, C3, k_norm;


- (id) init
        : (double) newTau
        : (double) newSigma0
        : (double) newLambdaAvg
        : (double) newD_omega_i
{
    self = [super init];

    if ( self )
    {
        tau = newTau;
        sigma0 = newSigma0;
        lambdaAvg = newLambdaAvg;
        d_omega_i = newD_omega_i;

        // constants to be used in pdf computations
        C1 = M_SQR(MATH_2_MUL_PI * sigma0 / lambdaAvg);
        expC1 = exp(-C1);
        C2 = M_SQR(tau / (2.0*lambdaAvg)) * MATH_PI * av2;
        C3 = av1 / d_omega_i;
    }
    
    return self;
}


- (void) code
        : (ArcObject <ArpCoder> *) coder
{
    [super code :coder];
    [coder codeDouble :&tau];
    [coder codeDouble :&sigma0];
    [coder codeDouble :&lambdaAvg];
    [coder codeDouble :&d_omega_i];
}


// function involved in cos_theta computation, by compute_cos_theta
double f_z(
    double cti,
    double cti2,
    double u1,
    double z
    )
{
    double z2 = z*z;

    return C3/cti
           * (   -m_dd_pow(expC1,cti2)/2*(2*z2-1)
               + m_dd_pow(expC1,-2*cti*z) / 4 / (C1*C1*cti2+1)
                 * (-2*C1*cti*z*sqrt(1-z2) - 2*z2 + 1)
               + m_dd_pow(expC1,z2)/2/C1
             )
           - C2/cti * z - u1;
}


// derivate involved in cos_theta computation, by compute_cos_theta
double f_dev_z(
    double cti,
    double cti2,
    double z
    )
{
    double z2 = z*z;

    return C3/cti
           * (   -2*z*m_dd_pow(expC1,cti2)
               + m_dd_pow(expC1,-2*cti*z) / 4 / (C1*C1*cti2+1)
                 * (4*C1*C1*cti2*z*sqrt(1-z2) + 4*C2*cti*z2 - 2*C1*cti
                    - 2*C1*cti*sqrt(1-z2) + 2*z2*C1*cti/sqrt(1-z2) - 4*z)
               - z*m_dd_pow(expC1,z2)
             )
           - C2/cti;
}


double compute_cos_theta(
    double cti,
    double cti2,
    double u1
    )
{
    double z, new_z;
    int ind;

    z = 0.99;
    new_z = z - f_z(cti,cti2,u1,z) / f_dev_z(cti,cti2,z);
    ind = 0;

    while (fabs(new_z - z) > 0.00001  &&  ind < 70)
    {
        z = new_z;
        new_z = z - f_z(cti,cti2,u1,z) / f_dev_z(cti,cti2,z);
        ind++;
    }

    return z;
}


- (void) getRandomDirection
        : (Vec3D *)       localO
        : (Vec3D *)       localH
        : (double *)      pdfValue
    for : (const Vec3D *) localI
        : (const double)  u1
        : (const double)  u2
{
    double cos_theta, sin_theta, phi;


    // compute localH by sampling
    double cti, cti2;
    cti = -ZC(*localI);
    cti2 = cti*cti;
    k_norm = 1.0 / MATH_2_MUL_PI
             / ( C3/cti *
                   (m_dd_pow(expC1,cti2) + (1- m_dd_pow(expC1,2*cti))/(4*m_dd_pow(C1,2)*cti2+4)
                    + 1/2/C1 - expC1/2/C1 )
                 + C2/cti
               );
    cos_theta = compute_cos_theta(cti, cti2, u1);
    phi = u2 / k_norm;

    cos_theta = M_MIN( cos_theta, 1.0 );
    sin_theta = sqrt(1.0 - M_SQR(cos_theta));


    *localH = VEC3D(sin_theta * cos(phi), sin_theta * sin(phi), cos_theta);
    vec3d_norm_v( localH );
    if (ZC(*localH) < 0.0) vec3d_d_mul_v(-1.0, localH);


    // compute localO by rotating localI around localH
    *localO = *localH;
    vec3d_d_mul_v( -2.0 * vec3d_vv_dot( localI, localH ), localO );
    vec3d_v_add_v( localI, localO );


    // compute PDF value
    *pdfValue= [ self valueForDirections :localI :localO :localH ];
}


- (double) valueForDirections
        : (const Vec3D *) localI
        : (const Vec3D *) localO
        : (const Vec3D *) localH
{
    double cti, pdf, cto;
    cti = -ZC(*localI);
    cto = vec3d_vv_dot( localO, localH );

    pdf = k_norm * (av1 * m_dd_pow(expC1,M_SQR(cti+cto)) / d_omega_i + C2 / cti / cto) * fact;
    pdf = pdf / (4.0 * vec3d_vv_dot( localO, localH ));

    return pdf;
}


- (void) getRandomDirection
        : (Vec3D *)       localO
        : (double *)      pdfValue
    for : (const Vec3D *) localI
        : (const double)  u1
        : (const double)  u2
{
    Vec3D localH;

    [ self getRandomDirection:   localO
                             : & localH
                             :   pdfValue
                         for :   localI
                             :   u1
                             :   u2
    ];
}

- (double) valueForDirections
        : (const Vec3D *) localI
        : (const Vec3D *) localO
{
    assert(ZC(*localI) <= 0);
    assert(ZC(*localO) >= 0);

    DEFINE_LOCAL_HALFANGLE(localI, localO)

    return [ self valueForDirections:   localI
                                    :   localO
                                    : & localH ];
}

@end




// ===========================================================================
