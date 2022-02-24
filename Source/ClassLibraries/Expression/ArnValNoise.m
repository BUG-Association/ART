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

#define ART_MODULE_NAME     ArnValNoise

#import "ArnValNoise.h"

static void arnperlinnoise_init_tables();
void arnperlinturbulence_init_matrix();

ART_MODULE_INITIALISATION_FUNCTION
(
    (void) art_gv;
    arnperlinnoise_init_tables();
    arnperlinturbulence_init_matrix();

    [ ArnVal_p3_perlin_noise_dv3        registerWithRuntime ];
    [ ArnVal_p3idd_fBm_noise_dv3        registerWithRuntime ];
    [ ArnVal_p3idd_turbulence_noise_dv3 registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


#define ARNNOISE_TABLESIZE 4096
#define ARNNOISE_TABLEMASK (ARNNOISE_TABLESIZE - 1)

static int      permutation[2 * ARNNOISE_TABLESIZE + 2];
static Vec3D    vec3d_table[2 * ARNNOISE_TABLESIZE + 2];

static void arnperlinnoise_init_tables()
{
    ArRandomNR1 rnd;            /* no need for a long period */
    int i;

    arrandom_nr1_seed(&rnd, 1340721968);

    for (i = 0; i < ARNNOISE_TABLESIZE; i++)
        permutation[i] = i;

    for (i = 0; i < ARNNOISE_TABLESIZE; i++)
    {
        int j = (int)(arrandom_nr1_double(&rnd) * ARNNOISE_TABLESIZE);

        // swap values
        int h = permutation[i];
        permutation[i] = permutation[j];
        permutation[j] = h;
    }

    i = 0;
    while (i < ARNNOISE_TABLESIZE)
    {
        Vec3D vec;
        double len;

        XC(vec) = 2.0 * arrandom_nr1_double(&rnd) - 1.0;
        YC(vec) = 2.0 * arrandom_nr1_double(&rnd) - 1.0;
        ZC(vec) = 2.0 * arrandom_nr1_double(&rnd) - 1.0;

        len = vec3d_v_sqrlen(&vec);

        if (len > 0.001 && len < 1.0)
        {
            len = sqrt(len);
            vec3d_dv_div_v(len, &vec, vec3d_table+i);
            i++;
        }
    }

    for (i = 0; i < ARNNOISE_TABLESIZE + 2; i++)
    {
        permutation[i + ARNNOISE_TABLESIZE] = permutation[i];
        vec3d_table[i + ARNNOISE_TABLESIZE] = vec3d_table[i];
    }

//    TR_CODE(
//        for (i = 0; i < ARNNOISE_TABLESIZE; i++)
//        {
//            if (permutation[i] < 0
//                || permutation[i] >= ARNNOISE_TABLESIZE)
//            {
//                EH_FATAL(arnode_errors,
//                         ("permutation[%d] = %d", i, permutation[i]));
//            }
//        }
//    )
}

/* ---------------------------------------------------------------------------
    'twist_trafo'
        This transformation is used to rotate and shift the coordinate frames
        of the different octaves of fBm noise and turbulence noise. It consists
        of various transformations (rotations and translation) with irrational
        parameters so that no two octaves line up evenly.
 --------------------------------------------------------------------------- */

static HTrafo3D twist_trafo;

void arnperlinturbulence_init_matrix()
{
    double x0 = MATH_SQRT_2;
    double x1 = MATH_GOLDEN;
    double x2 = MATH_E;

    HTrafo3D temp = HTRAFO3D_UNIT;
    RotX3D r0 = ROTX3D(x0);
    RotY3D r1 = ROTY3D(x1);
    RotZ3D r2 = ROTZ3D(x2);
    Translation3D tr = TRANSLATION3D(x0, x1, x2);

    trafo3d_rotx_h_mul_h(&r0, &temp, &twist_trafo);
    trafo3d_roty_h_mul_h(&r1, &twist_trafo, &temp);
    trafo3d_rotz_h_mul_h(&r2, &temp, &twist_trafo);
    trafo3d_tr_h_mul_h(&tr, &temp, &twist_trafo);
}

/* ---------------------------------------------------------------------------
     Ken Perlin's noise function...
     SIGGRAPH '92 tutorial 23  course notes
 --------------------------------------------------------------------------- */

static __inline__ void setup_base_frac(
        double    value,
        int     * base_0,
        double  * frac_0,
        int     * base_1,
        double  * frac_1
        )
{
    unsigned int base;
    m_d_floor_frac_ud(value,&base,frac_0);
    (*frac_1) = (*frac_0) - 1.0;
    (*base_0) = base & ARNNOISE_TABLEMASK;
    (*base_1) = (base + 1) & ARNNOISE_TABLEMASK;
}

static __inline__ double dot_product(
        double          x0,
        double          x1,
        double          x2,
        const Vec3D   * vec
        )
{
    return x0 * XC(*vec) + x1 * YC(*vec) + x2 * ZC(*vec);
}

static __inline__ double cubic_curve(
        double t
        )
{
    return t * t * (3.0 - 2.0 * t);
}

double perlin_noise_eval(
        const Pnt3D * pnt
        )
{
    int       base_x0, base_x1, base_y0, base_y1, base_z0, base_z1;
    int       b00, b10, b01, b11;
    double    fx0, fx1, fy0, fy1, fz0, fz1;
    double    sx, sy, sz, a, b, c, d, u, v;
    int       i, j;


    setup_base_frac(XC(*pnt), &base_x0, &fx0, &base_x1, &fx1);
    setup_base_frac(YC(*pnt), &base_y0, &fy0, &base_y1, &fy1);
    setup_base_frac(ZC(*pnt), &base_z0, &fz0, &base_z1, &fz1);

    i = permutation[base_x0];
    j = permutation[base_x1];

    b00 = permutation[i + base_y0];
    b10 = permutation[j + base_y0];
    b01 = permutation[i + base_y1];
    b11 = permutation[j + base_y1];

    sx = cubic_curve(fx0);
    sy = cubic_curve(fy0);
    sz = cubic_curve(fz0);

    u = dot_product(fx0, fy0, fz0, &vec3d_table[b00 + base_z0]);
    v = dot_product(fx1, fy0, fz0, &vec3d_table[b10 + base_z0]);
    a = M_INTERPOL(u, v, sx);

    u = dot_product(fx0, fy1, fz0, &vec3d_table[b01 + base_z0]);
    v = dot_product(fx1, fy1, fz0, &vec3d_table[b11 + base_z0]);
    b = M_INTERPOL(u, v, sx);

    c = M_INTERPOL(a, b, sy);               // interpolate in y at low x

    u = dot_product(fx0, fy0, fz1, &vec3d_table[b00 + base_z1]);
    v = dot_product(fx1, fy0, fz1, &vec3d_table[b10 + base_z1]);
    a = M_INTERPOL(u, v, sx);

    u = dot_product(fx0, fy1, fz1, &vec3d_table[b01 + base_z1]);
    v = dot_product(fx1, fy1, fz1, &vec3d_table[b11 + base_z1]);
    b = M_INTERPOL(u, v, sx);

    d = M_INTERPOL(a, b, sy);               // interpolate in y at high x

    return( 1.5 * M_INTERPOL(c, d, sz));    // interpolate in z
}

void perlin_noise_vector_eval(
        const Pnt3D * pnt,
        Vec3D       * vec
        )
{
    int     base_x0, base_x1, base_y0, base_y1, base_z0, base_z1;
    int     b00, b10, b01, b11;
    double  fx0, fx1, fy0, fy1, fz0, fz1;
    double  sx, sy, sz;
    Vec3D   a, b, c, d;
    Vec3D   *u, *v;
    int     i, j;


    setup_base_frac(XC(*pnt), &base_x0, &fx0, &base_x1, &fx1);
    setup_base_frac(YC(*pnt), &base_y0, &fy0, &base_y1, &fy1);
    setup_base_frac(ZC(*pnt), &base_z0, &fz0, &base_z1, &fz1);

    i = permutation[base_x0];
    j = permutation[base_x1];

    b00 = permutation[i + base_y0];
    b10 = permutation[j + base_y0];
    b01 = permutation[i + base_y1];
    b11 = permutation[j + base_y1];

    sx = cubic_curve(fx0);
    sy = cubic_curve(fy0);
    sz = cubic_curve(fz0);

    u = &vec3d_table[b00 + base_z0];
    v = &vec3d_table[b10 + base_z0];
    vec3d_dvv_interpol_v(sx, u, v, &a);

    u = &vec3d_table[b01 + base_z0];
    v = &vec3d_table[b11 + base_z0];
    vec3d_dvv_interpol_v(sx, u, v, &b);

    vec3d_dvv_interpol_v(sy, &a, &b, &c);     // interpolate in y at low x

    u = &vec3d_table[b00 + base_z1];
    v = &vec3d_table[b10 + base_z1];
    vec3d_dvv_interpol_v(sx, u, v, &a);

    u = &vec3d_table[b01 + base_z1];
    v = &vec3d_table[b11 + base_z1];
    vec3d_dvv_interpol_v(sx, u, v, &b);

    vec3d_dvv_interpol_v(sy, &a, &b, &d);     // interpolate in y at high x

    vec3d_dvv_interpol_v(sz, &c, &d, vec);    // interpolate in z
}

double fBm_noise_eval(
        const Pnt3D   * pnt,
        unsigned int    octaves,
        double          lacunarity,
        double          gain
        )
{
    Pnt3D local_pnt = *pnt;
    double amplitude = 1.0;
    double noise_sum = 0.0;
    unsigned int octave;

    for (octave = 0; octave < octaves; octave++)
    {
        noise_sum += perlin_noise_eval(&local_pnt) * amplitude;

        Pnt3D pnt_twisted;
        pnt3d_p_htrafo3d_p(&local_pnt, &twist_trafo, &pnt_twisted);
        Scale3D coord_scale = SCALE3D(lacunarity, lacunarity, lacunarity);
        pnt3d_p_scale_p(&coord_scale, &pnt_twisted, &local_pnt);

        amplitude *= gain;
    }
    return M_CLAMP(noise_sum,-1.0,1.0);
}

void fBm_noise_vector_eval(
        const Pnt3D   * pnt,
        unsigned int    octaves,
        double          lacunarity,
        double          gain,
        Vec3D         * noise_sum
        )
{
    Pnt3D local_pnt = *pnt;
    double amplitude = 1.0;
    unsigned int octave;

    *noise_sum = VEC3D(0.0, 0.0, 0.0);
    for (octave = 0; octave < octaves; octave++)
    {
        Vec3D octave_noise;
        perlin_noise_vector_eval(&local_pnt, &octave_noise);
        vec3d_d_mul_v(amplitude, &octave_noise);
        vec3d_v_add_v(&octave_noise, noise_sum);

        Pnt3D pnt_twisted;
        pnt3d_p_htrafo3d_p(&local_pnt, &twist_trafo, &pnt_twisted);
        Scale3D coord_scale = SCALE3D(lacunarity, lacunarity, lacunarity);
        pnt3d_p_scale_p(&coord_scale, &pnt_twisted, &local_pnt);

        amplitude *= gain;
    }
}

double turbulence_noise_eval(
        const Pnt3D   * pnt,
        unsigned int    octaves,
        double          lacunarity,
        double          gain
        )
{
    Pnt3D local_pnt = *pnt;
    double amplitude = 1.0;
    double noise_sum = 0.0;
    unsigned int octave;

    for (octave = 0; octave < octaves; octave++)
    {
        noise_sum += fabs(perlin_noise_eval(&local_pnt)) * amplitude;

        Pnt3D pnt_twisted;
        pnt3d_p_htrafo3d_p(&local_pnt, &twist_trafo, &pnt_twisted);
        Scale3D coord_scale = SCALE3D(lacunarity, lacunarity, lacunarity);
        pnt3d_p_scale_p(&coord_scale, &pnt_twisted, &local_pnt);

        amplitude *= gain;
    }
    return noise_sum;
}

void turbulence_noise_vector_eval(
        const Pnt3D   * pnt,
        unsigned int    octaves,
        double          lacunarity,
        double          gain,
        Vec3D         * noise_sum
        )
{
    Pnt3D local_pnt = *pnt;
    double amplitude = 1.0;
    unsigned int octave;

    *noise_sum = VEC3D(0.0, 0.0, 0.0);
    for (octave = 0; octave < octaves; octave++)
    {
        Vec3D octave_noise;
        perlin_noise_vector_eval(&local_pnt, &octave_noise);
        XC(octave_noise) = fabs(XC(octave_noise));
        YC(octave_noise) = fabs(YC(octave_noise));
        ZC(octave_noise) = fabs(ZC(octave_noise));
        vec3d_d_mul_v(amplitude, &octave_noise);
        vec3d_v_add_v(&octave_noise, noise_sum);

        Pnt3D pnt_twisted;
        pnt3d_p_htrafo3d_p(&local_pnt, &twist_trafo, &pnt_twisted);
        Scale3D coord_scale = SCALE3D(lacunarity, lacunarity, lacunarity);
        pnt3d_p_scale_p(&coord_scale, &pnt_twisted, &local_pnt);

        amplitude *= gain;
    }
}

/* ===========================================================================
    Perlin noise
 =========================================================================== */
#define P3_PERLIN_NOISE_D(out, in) \
    out = 0.5 + 0.5 * perlin_noise_eval(&in);
#define P3_PERLIN_NOISE_V3(out, in) \
    perlin_noise_vector_eval(&in, &out);
// TODO: signed vs. unsigned noise

ARNVAL_UNARY_EXPR_2_TYPES_IMPL(
        Pnt3D,
        p3_perlin_noise_dv3,
        Double,
        P3_PERLIN_NOISE_D(MOV, MIV),
        P3_PERLIN_NOISE_D(SOV, SIV),
        Vec3D,
        P3_PERLIN_NOISE_V3(MOV, MIV),
        P3_PERLIN_NOISE_V3(SOV, SIV)
        )
// ...

/* ===========================================================================
    Fractional Brownian motion noise
 =========================================================================== */
#define P3_FBM_NOISE_D(out, in0, in1, in2, in3) \
    out = 0.5 + 0.5 * fBm_noise_eval(&in0, in1, in2, in3);
#define P3_FBM_NOISE_V3(out, in0, in1, in2, in3) \
    fBm_noise_vector_eval(&in0, in1, in2, in3, &out);
// TODO: signed vs. unsigned noise

ARNVAL_QUATERNARY_EXPR_2_TYPES_IMPL(
        Pnt3D, Int, Double, Double,
        p3idd_fBm_noise_dv3,
        Double,
        P3_FBM_NOISE_D(MOV, MIV0, MIV1, MIV2, MIV3),
        P3_FBM_NOISE_D(SOV, SIV0, SIV1, SIV2, SIV3),
        Vec3D,
        P3_FBM_NOISE_V3(MOV, MIV0, MIV1, MIV2, MIV3),
        P3_FBM_NOISE_V3(SOV, SIV0, SIV1, SIV2, SIV3)
        )
// ...

/* ===========================================================================
    Turbulence noise
 =========================================================================== */
#define P3_TURBULENCE_NOISE_D(out, in0, in1, in2, in3) \
    out = turbulence_noise_eval(&in0, in1, in2, in3);
#define P3_TURBULENCE_NOISE_V3(out, in0, in1, in2, in3) \
    turbulence_noise_vector_eval(&in0, in1, in2, in3, &out);

ARNVAL_QUATERNARY_EXPR_2_TYPES_IMPL(
        Pnt3D, Int, Double, Double,
        p3idd_turbulence_noise_dv3,
        Double,
        P3_TURBULENCE_NOISE_D(MOV, MIV0, MIV1, MIV2, MIV3),
        P3_TURBULENCE_NOISE_D(SOV, SIV0, SIV1, SIV2, SIV3),
        Vec3D,
        P3_TURBULENCE_NOISE_V3(MOV, MIV0, MIV1, MIV2, MIV3),
        P3_TURBULENCE_NOISE_V3(SOV, SIV0, SIV1, SIV2, SIV3)
        )
// ...

// ===========================================================================
