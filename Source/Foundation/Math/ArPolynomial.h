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

#ifndef _ART_MATH_ARPOLYNOMIAL_H_
#define _ART_MATH_ARPOLYNOMIAL_H_

#include "ART_Foundation_System.h"

/* ---------------------------------------------------------------------------
    'ARPOLYNOMIAL_MAX_ORDER'
        Maximum order for a polynomial.
--------------------------------------------------------------------------- */
#define ARPOLYNOMIAL_MAX_ORDER          12

/* ---------------------------------------------------------------------------
    'ARPOLYNOMIAL_RELERROR'
        Smallest relative error we want.
--------------------------------------------------------------------------- */
#define ARPOLYNOMIAL_RELERROR           1.0e-14

/* ---------------------------------------------------------------------------
    'ARPOLYNOMIAL_MAXPOW'
        Maximum power of 10 we wish to search to.
--------------------------------------------------------------------------- */
#define ARPOLYNOMIAL_MAXPOW             32

/* ---------------------------------------------------------------------------
    'ARPOLYNOMIAL_MAXIT'
        Maximum number of iterations for root searches.
--------------------------------------------------------------------------- */
#define ARPOLYNOMIAL_MAXIT              800
/* max number of iterations */

/* ---------------------------------------------------------------------------
    'ARPOLYNOMIAL_SMALL_ENOUGH'
        A coefficient smaller than this constant is considered to
        be zero (0.0).
--------------------------------------------------------------------------- */
#define ARPOLYNOMIAL_SMALL_ENOUGH       1.0e-12

/* ---------------------------------------------------------------------------
    'ArPolynomial'
        The coefficients are stored in order of increasing powers, i.e.
                p(x) = p->coef[0] + x * p->coef[1] + x^2 * p->coef[2]
                       + ... + x^p->ord * p->coef[p->ord]
--------------------------------------------------------------------------- */
typedef struct  ArPolynomial
{
    int ord;
    double      coef[ARPOLYNOMIAL_MAX_ORDER];
}
ArPolynomial;

/* ---------------------------------------------------------------------------
    'arpolynomial_eval'
        Evaluate polynomial 'p' at 'x' and return the value.
--------------------------------------------------------------------------- */
double arpolynomial_eval(
        ArPolynomial * p,
        double x
        );

/* ---------------------------------------------------------------------------
    'arpolynomial_modrf'
        Uses the modified regula-falsi method to evaluate the root in
        interval [a,b] of the polynomial described in coef. The root is
        returned is returned in *val. The routine returns zero if it can't
        converge.
--------------------------------------------------------------------------- */
int arpolynomial_modrf(
        ArPolynomial * p,
        double a,
        double b,
        double * val
        );

/* ---------------------------------------------------------------------------
    'arpolynomial_modp'
        Calculates the modulus of u(x) / v(x) leaving it in r, it returns 0
        if r(x) is a constant. Note: this function assumes the leading
        coefficient of v is 1 or -1
--------------------------------------------------------------------------- */
int arpolynomial_modp(
        ArPolynomial * u,
        ArPolynomial * v,
        ArPolynomial * r
        );

/* ---------------------------------------------------------------------------
    'arpolynomial_buildsturm'
        Build up a sturm sequence for a polynomial in smat, returning the
        number of polynomials in the sequence
--------------------------------------------------------------------------- */
int arpolynomial_buildsturm (
        int ord,
        ArPolynomial * sseq
        );

/* ---------------------------------------------------------------------------
    'arpolynomial_numroots'
        Return the number of distinct real roots of the polynomial described
        in 'sseq'.  Optionally the sign changes at negative infinity and
        positive infinity are returned via the integer pointer 'atneg' and
        'atpos'.
--------------------------------------------------------------------------- */
int arpolynomial_numroots(
        int np,
        ArPolynomial * sseq,
        int * atneg,
        int * atpos
        );

/* ---------------------------------------------------------------------------
    'arpolynomial_numchanges'
        Return the number of sign changes in the Sturm sequence in 'sseq' at
        the value 'a'.
--------------------------------------------------------------------------- */
int arpolynomial_numchanges(
        int np,
        ArPolynomial * sseq,
        double a
        );
/* ---------------------------------------------------------------------------
    'arpolynomial_sbisect'
        Uses a bisection based on the sturm sequence for the polynomial
        described in sseq to isolate intervals in which roots occur, the
        roots are returned in the roots array in order of magnitude.
--------------------------------------------------------------------------- */
void arpolynomial_sbisect(
        int np,
        ArPolynomial * sseq,
        double min,
        double max,
        int atmin,
        int atmax,
        double * roots
        );

#endif /* _ART_MATH_ARPOLYNOMIAL_H_ */
/* ======================================================================== */
