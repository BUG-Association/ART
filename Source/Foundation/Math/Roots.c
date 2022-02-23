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

#include "Roots.h"

#include "Functions.h"

int roots_of_norm_poly2(
        const double * pol,
        double * root
        )
{
    double p_2 = pol[1] / 2.0;
    double d = p_2 * p_2 - pol[0];

    if (d < MATH_TINY_DOUBLE)                   /* exclude double roots */
        return (0);
    else
    {
        if (p_2 > 0.0)                          /* prevent cancellation */
        {
            root[0] = -(p_2 + sqrt(d));
            root[1] = pol[0] / root[0];         /* VIETA */
        }
        else
        {
            root[1] = sqrt(d) - p_2;
            root[0] = pol[0] / root[1];         /* VIETA */
        }
        return (2);
    }
}

int roots_of_poly2(
        const double * pol,
        double * root
        )
{
    double d = pol[1] * pol[1] - 4 * pol[2] * pol[0];

    if (d < MATH_TINY_DOUBLE)                   /* exclude double roots */
        return (0);
    else
    {
        if (pol[1] > 0.0)                       /* prevent cancellation */
        {
            d = -(sqrt(d) + pol[1]);
            if (pol[2] > 0.0)
            {
                root[0] = d / (pol[2] + pol[2]);    /* VIETA */
                root[1] = (pol[0] + pol[0]) / d;
            }
            else
            {
                root[1] = d / (pol[2] + pol[2]);    /* VIETA */
                root[0] = (pol[0] + pol[0]) / d;
            }
        }
        else
        {
            d = sqrt(d) - pol[1];
            if (pol[2] < 0.0)
            {
                root[0] = d / (pol[2] + pol[2]);    /* VIETA */
                root[1] = (pol[0] + pol[0]) / d;
            }
            else
            {
                root[1] = d / (pol[2] + pol[2]);    /* VIETA */
                root[0] = (pol[0] + pol[0]) / d;
            }
        }
    }
    return (2);
}

double one_root_of_norm_poly3(
        const double * pol
        )
{
    double  d, p, q, p_3;

    /* ---- polynom is normalized: x^3 + pol[2] x^2 + pol[1] x + pol[0] = 0 */

    /* ------- eliminate quadric term (x = y - pol[2]/3): x^3 + p x + q = 0 */
    d = pol[2] * pol[2];

    p = 1.0/3.0 * (-1.0/3.0 * d + pol[1]);
    q = 1.0/2.0 * ((2.0/27.0 * d - 1.0/3.0 * pol[1]) * pol[2] + pol[0]);

    /* ---------------------------------------------- use Cardano's formula */
    p_3 = p * p * p;
    d = q * q + p_3;

    if (m_d_is_tiny(d))
    {
        if (m_d_is_tiny(q))                     /* one triple solution */
            return (-1.0/3.0 * pol[2]);
        else                    /* one single and one double solution */
        {
            q = m_d_cbrt(-q);
            return (q + q - 1.0/3.0 * pol[2]);
        }
    }
    else if (d < 0.0)   /* Casus irreducibilis: three real solutions */
    {
        return (2.0 * sqrt(-p) * cos(1.0/3.0 * acos(-q / sqrt(-p_3)))
                                        - 1.0/3.0 * pol[2]);
    }
    else
    {
        d = sqrt(d);                    /* use d as a temporary variable */
        return (m_d_cbrt(d - q) - m_d_cbrt(d + q) - 1.0/3.0 * pol[2]);
    }
}

static void sort3_doubles(
        double * real
        )
{
    if (real[0] > real[1])
    {
        if (real[0] > real[2])          /* real[0] is biggest */
        {
            double help = real[2];
            real[2] = real[0];
            if (real[1] > help)
                real[0] = help;
            else
            {
                real[0] = real[1];
                real[1] = help;
            }
        }
        else
        {
            double help = real[1];
            real[1] = real[0];
            real[0] = help;
        }
    }
    else
    {
        if (real[1] > real[2])          /* real[1] is biggest */
        {
            double help = real[2];
            real[2] = real[1];
            if (real[0] > help)
            {
                real[1] = real[0];
                real[0] = help;
            }
            else
                real[1] = help;
        }
    }
}

static int merge_doubles(
    int n1,
    const double * r1,
    int n2,
    const double * r2,
    double * real
    )
{
    int n = 0;
    while (n1 > 0 && n2 > 0)
    {
        if (*r1 < *r2)
            { *real++ = *r1++; n1--; }
        else
            { *real++ = *r2++; n2--; }
        n++;
    }
    while (n1 > 0)
        { *real++ = *r1++; n1--; n++; }
    while (n2 > 0)
        { *real++ = *r2++; n2--; n++; }
    return (n);
}

int roots_of_norm_poly3(
        const double * pol,
        double * root
        )
{
    double  d, p, q, p_3;

    /* ---- polynom is normalized: x^3 + pol[2] x^2 + pol[1] x + pol[0] = 0 */

    /* ------- eliminate quadric term (x = y - pol[2]/3): x^3 + p x + q = 0 */
    d = pol[2] * pol[2];

    p = 1.0/3.0 * (-1.0/3.0 * d + pol[1]);
    q = 1.0/2.0 * ((2.0/27.0 * d  - 1.0/3.0 * pol[1]) * pol[2] + pol[0]);

    /* ---------------------------------------------- use Cardano's formula */
    p_3 = p * p * p;
    d = q * q + p_3;

    if (m_d_is_tiny(d))
    {
        if (m_d_is_tiny(q))                     /* one triple solution */
        {
            root[0] = - 1.0/3.0 * pol[2];
            return (1);
        }
        else                    /* one single and one double solution */
        {
            double u = m_d_cbrt(-q);
            root[0] = u + u - 1.0/3.0 * pol[2];
            return (1);
        }
    }
    else if (d < 0.0)       /* Casus irreducibilis: three real solutions */
    {
        double phi = 1.0/3.0 * acos(-q / sqrt(-p_3));
        double t = 2.0 * sqrt(-p);

        root[0] = t * cos(phi)                - 1.0/3.0 * pol[2];
        root[1] = - t * cos(phi + MATH_PI / 3.0) - 1.0/3.0 * pol[2];
        root[2] = - t * cos(phi - MATH_PI / 3.0) - 1.0/3.0 * pol[2];
        sort3_doubles(root);
        return (3);
    }
    else
    {
        d = sqrt(d);                  /* use d as a temporary variable */

        root[0] = m_d_cbrt(d - q) - m_d_cbrt(d + q) - 1.0/3.0 * pol[2];
        return (1);
    }
}

int roots_of_poly3(
        const double * pol,
        double * root
        )
{
    double  a, b, c, d, p, q, p_3;

    /* ----------------------------- normal form: x^3 + a x^2 + b x + c = 0 */
    a = pol[2] / pol[3];
    b = pol[1] / pol[3];
    c = pol[0] / pol[3];

    /* ------------ eliminate quadric term (x = y - a/3): x^3 + p x + q = 0 */
    d = a * a;

    p = 1.0/3.0 * (-1.0/3.0 * d + b);
    q = 1.0/2.0 * ((2.0/27.0 * d - 1.0/3.0 * b) * a + c);

    /* ---------------------------------------------- use Cardano's formula */
    p_3 = p * p * p;
    d = q * q + p_3;

    if (m_d_is_tiny(d))
    {
        if (m_d_is_tiny(q))                     /* one triple solution */
        {
            root[0] = - 1.0/3.0 * a;
            return (1);
        }
        else                    /* one single and one double solution */
        {
            double u;

            u = m_d_cbrt(-q);

            root[0] = u + u - 1.0/3.0 * a;
            return (1);
        }
    }
    else if (d < 0.0)           /* Casus irreducibilis: three real solutions */
    {
        double phi = 1.0/3.0 * acos(-q / sqrt(-p_3));
        double t = 2.0 * sqrt(-p);

        root[0] = t * cos(phi)                - 1.0/3.0 * a;
        root[1] = - t * cos(phi + MATH_PI / 3.0) - 1.0/3.0 * a;
        root[2] = - t * cos(phi - MATH_PI / 3.0) - 1.0/3.0 * a;
        sort3_doubles(root);
        return (3);
    }
    else
    {
        d = sqrt(d);                    /* use d as a temporary variable */

        root[0] = m_d_cbrt(d - q) - m_d_cbrt(d + q) - 1.0/3.0 * a;
        return (1);
    }
}

int roots_of_poly4(
        const double * pol,
        double * root
        )
{
    double  poly[4];
    double  a,b,c,d,e,p,q,r,u,v,z;
    int     i,num,n1,n2;

    /* --------------------- normal form: x^4 + a x^3 + b x^2 + c x + d = 0 */
    a = pol[3] / pol[4];
    b = pol[2] / pol[4];
    c = pol[1] / pol[4];
    d = pol[0] / pol[4];

    /* ----- eliminate cubic term (x = y - a /4): x^4 + p x^2 + q x + r = 0 */
    e = a * a;
    p = - 3.0/8.0 * e + b;
    q = (1.0/8.0 * e - 1.0/2.0 * b) * a + c;
    r = (1.0/16.0 * b - 3.0/256.0 * e) * e - 1.0/4.0 * a * c + d;

    if (m_d_is_tiny(r))
    {
        /* ------------------------ no absolute term: y (y^3 + p y + q) = 0 */
        poly[0] = q;
        poly[1] = p;
        poly[2] = 0.0;
                                        /* implicitly: poly[3] = 1.0; */

        num = roots_of_norm_poly3(poly,root);
        for (i = num; i > 0 && root[i - 1] > 0.0; i--)
            root[i] = root[i - 1];              /* insert root at 0.0 */
        root[i] = 0.0;
        num++;
    }
    else
    {
        /* ---------------------------------- solve the resolvent cubic ... */
        poly[0] = 1.0/2.0 * r * p - 1.0/8.0 * q * q;
        poly[1] = - r;
        poly[2] = -1.0/2.0 * p;
                                        /* implicitly: poly[3] = 1.0; */

        /* ------------------------- ... and take the one real solution ... */
        z = one_root_of_norm_poly3(poly);

        /* ------------------------- ... to build two quadric equations ... */
        u = z * z - r;
        v = 2.0 * z - p;

/* ---------------------------------------------------------------------------
        There is a problem with the coefficients of the resolvent quadrics.
        The original code used the following additional condition and...

--------------------------------------------------------------------------- */
        if (m_d_is_tiny(u))
            u = 0.0;
        else if (u > 0.0)
            u = sqrt(u);
        else
            return 0.0;
/* ---------------------------------------------------------------------------
        ... originally returned 0 here instead of setting u to 0.

        The same is of course true for the other resolvent as well:

--------------------------------------------------------------------------- */
        if (m_d_is_tiny(v))
            v = 0.0;
        else if (v > 0.0)
            v = sqrt(v);
        else
            return 0.0;
/* ---------------------------------------------------------------------------
        ... originally returned 0 here instead of setting v to 0.
--------------------------------------------------------------------------- */

        root[0] = z - u;                /* use root field for polynomial */
        root[1] = (q < 0.0) ? -v : v;
                                        /* implicitly: root[2] = 1.0; */

        n1 = roots_of_norm_poly2(root,poly); /* ... and poly field for roots */

        root[0] = z + u;                /* the same misuse again */
        root[1] = - root[1];

        n2 = roots_of_norm_poly2(root,poly + 2);

        num = merge_doubles(n1,poly,n2,poly + 2,root);
    }
    e = 1.0/4.0 * a;

    for (i = 0; i < num; i++)
        root[i] -= e;

    return (num);
}

int roots_of_norm_poly4(
        const double * pol,
        double * root
        )
{
    double  poly[4];
    double  e,p,q,r,u,v,z;
    int     i,num,n1,n2;

    /* -- normalized: x^4 + pol[3] x^3 + pol[2] x^2 + pol[1] x + pol[0] = 0 */

    /* eliminate cubic term (x = y - pol[3] /4):  x^4 + p x^2 + q x + r = 0 */
    e = pol[3] * pol[3];
    p = - 3.0/8.0 * e + pol[2];
    q = (1.0/8.0 * e - 1.0/2.0 * pol[2]) * pol[3] + pol[1];
    r = (1.0/16.0 * pol[2] - 3.0/256.0 * e) * e - 1.0/4.0 * pol[3] * pol[1]
        + pol[0];

    if (m_d_is_tiny(r))
    {
        /* ------------------------ no absolute term: y (y^3 + p y + q) = 0 */
        poly[0] = q;
        poly[1] = p;
        poly[2] = 0.0;
                                        /* implicitly: poly[3] = 1.0; */

        num = roots_of_norm_poly3(poly,root);
        for (i = num; i > 0 && root[i - 1] > 0.0; i--)
            root[i] = root[i - 1];              /* insert root at 0.0 */
        root[i] = 0.0;
        num++;
    }
    else
    {
        /* ---------------------------------- solve the resolvent cubic ... */
        poly[0] = 1.0/2.0 * r * p - 1.0/8.0 * q * q;
        poly[1] = - r;
        poly[2] = -1.0/2.0 * p;
                                        /* implicitly: poly[3] = 1.0; */

        /* ------------------------- ... and take the one real solution ... */
        z = one_root_of_norm_poly3(poly);

        /* ------------------------- ... to build two quadric equations ... */
        u = z * z - r;
        v = 2.0 * z - p;

        if (m_d_is_tiny(u))
            u = 0.0;
        else if (u > 0.0)
            u = sqrt(u);
        else
            u = 0.0;
/* ---------------------------------------------------------------------------
        There is a problem with the coefficients of the resolvent quadrics.
        The original code returned 0 instead of setting u to 0.
--------------------------------------------------------------------------- */

        if (m_d_is_tiny(v))
            v = 0.0;
        else if (v > 0.0)
            v = sqrt(v);
        else
            v = 0.0;
/* ---------------------------------------------------------------------------
        And here it returned 0 here instead of setting v to 0.
--------------------------------------------------------------------------- */

        root[0] = z - u;                /* use root field for polynomial */
        root[1] = (q < 0.0) ? -v : v;   /* implicitly: root[2] = 1.0; */

        n1 = roots_of_norm_poly2(root,poly); /* ...and poly field for roots */

        root[0] = z + u;                /* the same misuse again */
        root[1] = - root[1];

        n2 = roots_of_norm_poly2(root,poly + 2);

        num = merge_doubles(n1,poly,n2,poly + 2,root);
    }
    e = 1.0/4.0 * pol[3];

    for (i = 0; i < num; i++)
        root[i] -= e;

    return (num);
}

/* ======================================================================== */
