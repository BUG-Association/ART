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

#include "ArPolynomial.h"

#include <math.h>               /* fabs */

double arpolynomial_eval(
        ArPolynomial * p,
        double x
        )
{
    double * fp = &p->coef[p->ord];
    double f = *fp;

    for (fp--; fp >= p->coef; fp--)
        f = x * f + *fp;

    return f;
}

/* ---------------------------------------------------------------------------
    'arpolynomial_modrf'
--------------------------------------------------------------------------- */
int arpolynomial_modrf(
        ArPolynomial * p,
        double a,
        double b,
        double *val
        )
{
    int         its;
    double      fa, fb, x, fx, lfx;
    double      * fp;

    double * scoef = p->coef;
    double * ecoef = &p->coef[p->ord];
    fx = 0;
    fb = fa = *ecoef;
    for (fp = ecoef - 1; fp >= scoef; fp--) {
        fa = a * fa + *fp;
        fb = b * fb + *fp;
    }

    /* --------------- if there is no sign difference the method won't work */
    if (fa * fb > 0.0)
        return(0);

    if (fabs(fa) < ARPOLYNOMIAL_RELERROR) {
            *val = a;
            return(1);
    }

    if (fabs(fb) < ARPOLYNOMIAL_RELERROR) {
            *val = b;
            return(1);
    }

        lfx = fa;


    for (its = 0; its < ARPOLYNOMIAL_MAXIT; its++)
    {
        x = (fb * a - fa * b) / (fb - fa);

        fx = *ecoef;
        for (fp = ecoef - 1; fp >= scoef; fp--)
            fx = x * fx + *fp;

        if (fabs(x) > ARPOLYNOMIAL_RELERROR)
        {
            if (fabs(fx / x) < ARPOLYNOMIAL_RELERROR)
            {
                *val = x;
                return(1);
            }
        }
        else if (fabs(fx) < ARPOLYNOMIAL_RELERROR)
        {
            *val = x;
            return(1);
        }

        if ((fa * fx) < 0)
        {
            b = x;
            fb = fx;
            if ((lfx * fx) > 0)
                    fa /= 2;
        }
        else
        {
            a = x;
            fa = fx;
            if ((lfx * fx) > 0)
                    fb /= 2;
        }

        lfx = fx;
    }

    fprintf(stderr, "modrf overflow %f %f %f\n", a, b, fx);

    return(0);
}

/* ---------------------------------------------------------------------------
    'arpolynomial_modp'
--------------------------------------------------------------------------- */
int arpolynomial_modp(
        ArPolynomial * u,
        ArPolynomial * v,
        ArPolynomial * r
        )
{
    int         k, j;
    double      *nr, *end, *uc;

    nr = r->coef;
    end = &u->coef[u->ord];

    uc = u->coef;
    while (uc <= end)
        *nr++ = *uc++;

    if (v->coef[v->ord] < 0.0)
    {
        for (k = u->ord - v->ord - 1; k >= 0; k -= 2)
            r->coef[k] = -r->coef[k];

        for (k = u->ord - v->ord; k >= 0; k--)
            for (j = v->ord + k - 1; j >= k; j--)
                r->coef[j] = -r->coef[j] - r->coef[v->ord+k] * v->coef[j-k];
    }
    else
    {
        for (k = u->ord - v->ord; k >= 0; k--)
            for (j = v->ord + k - 1; j >= k; j--)
                r->coef[j] -= r->coef[v->ord + k] * v->coef[j - k];
    }

    k = v->ord - 1;
    while (k >= 0 && fabs(r->coef[k]) < ARPOLYNOMIAL_SMALL_ENOUGH)
    {
        r->coef[k] = 0.0;
        k--;
    }

    r->ord = (k < 0) ? 0 : k;

    return(r->ord);
}

/* ---------------------------------------------------------------------------
    'arpolynomial_buildsturm'
--------------------------------------------------------------------------- */
int arpolynomial_buildsturm (
        int ord,
        ArPolynomial * sseq
        )
{
    int         i;
    double      f;
    double * fp;
    double * fc;
    ArPolynomial * sp;

    sseq[0].ord = ord;
    sseq[1].ord = ord - 1;


    /* ---- calculate the derivative and normalise the leading coefficient. */
    f = fabs(sseq[0].coef[ord] * ord);
    fp = sseq[1].coef;
    fc = sseq[0].coef + 1;
    for (i = 1; i <= ord; i++)
        *fp++ = *fc++ * i / f;

    /* --------------------------- construct the rest of the Sturm sequence */
    for (sp = sseq + 2; arpolynomial_modp(sp - 2, sp - 1, sp); sp++)
    {
        /* --------------------------------- reverse the sign and normalise */
        f = -fabs(sp->coef[sp->ord]);
        for (fp = &sp->coef[sp->ord]; fp >= sp->coef; fp--)
            *fp /= f;
    }

    sp->coef[0] = -sp->coef[0];         /* reverse the sign */

    return(sp - sseq);
}

/* ---------------------------------------------------------------------------
    'arpolynomial_numroots'
--------------------------------------------------------------------------- */
int arpolynomial_numroots(
        int np,
        ArPolynomial * sseq,
        int * atneg,
        int * atpos
        )
{
    int atposinf = 0;
    int atneginf = 0;
    ArPolynomial * s;
    double f;

    /* --------------------------------------- changes at positive infinity */
    double lf = sseq[0].coef[sseq[0].ord];

    for (s = sseq + 1; s <= sseq + np; s++)
    {
        f = s->coef[s->ord];
        if (lf == 0.0 || lf * f < 0)
            atposinf++;
        lf = f;
    }

    /* --------------------------------------- changes at negative infinity */
    if (sseq[0].ord & 1)
        lf = -sseq[0].coef[sseq[0].ord];
    else
        lf = sseq[0].coef[sseq[0].ord];

    for (s = sseq + 1; s <= sseq + np; s++)
    {
        if (s->ord & 1)
            f = -s->coef[s->ord];
        else
            f = s->coef[s->ord];
        if (lf == 0.0 || lf * f < 0)
            atneginf++;
        lf = f;
    }

    if (atneg) *atneg = atneginf;
    if (atpos) *atpos = atposinf;

    return(atneginf - atposinf);
}

/* ---------------------------------------------------------------------------
    'arpolynomial_numchanges'
--------------------------------------------------------------------------- */
int arpolynomial_numchanges(
        int np,
        ArPolynomial * sseq,
        double a
        )

{
    int changes = 0;
    double f;
    ArPolynomial * s;

    double lf = arpolynomial_eval(&sseq[0], a);

    for (s = sseq + 1; s <= sseq + np; s++)
    {
        f = arpolynomial_eval(s, a);
        if (lf == 0.0 || lf * f < 0)
            changes++;
        lf = f;
    }

    return(changes);
}

/* ---------------------------------------------------------------------------
    'arpolynomial_sbisect'
--------------------------------------------------------------------------- */
void arpolynomial_sbisect(
        int np,
        ArPolynomial * sseq,
        double min,
        double max,
        int atmin,
        int atmax,
        double * roots
        )
{
    double mid;
    int atmid;
    int n1 = 0, n2 = 0, its, nroot;

    if ((nroot = atmin - atmax) == 1)
    {

        /* --------------------------- first try a less expensive technique */
        if (arpolynomial_modrf(sseq, min, max, &roots[0]))
                return;


        /* -------------------------------------------------------------------
            If we get here we have to evaluate the root the hard way by using
            the Sturm sequence.
        ------------------------------------------------------------------- */
        for (its = 0; its < ARPOLYNOMIAL_MAXIT; its++)
        {
            mid = (min + max) / 2;
            atmid = arpolynomial_numchanges(np, sseq, mid);

            if (fabs(mid) > ARPOLYNOMIAL_RELERROR)
            {
                if (fabs((max - min) / mid) < ARPOLYNOMIAL_RELERROR)
                {
                    roots[0] = mid;
                    return;
                }
            }
            else if (fabs(max - min) < ARPOLYNOMIAL_RELERROR)
            {
                roots[0] = mid;
                return;
            }

            if ((atmin - atmid) == 0)
                    min = mid;
            else
                    max = mid;
        }

        if (its == ARPOLYNOMIAL_MAXIT)
        {
            fprintf(stderr, "arpolynomial_sbisect: overflow min %f max %f\
                    diff %e nroot %d n1 %d n2 %d\n",
                    min, max, max - min, nroot, n1, n2);
            roots[0] = mid;
        }

        return;
    }

    /* ----------- more than one root in the interval, we have to bisect... */
    for (its = 0; its < ARPOLYNOMIAL_MAXIT; its++)
    {
        mid = (min + max) / 2;
        atmid = arpolynomial_numchanges(np, sseq, mid);

        n1 = atmin - atmid;
        n2 = atmid - atmax;

        if (n1 != 0 && n2 != 0)
        {
            arpolynomial_sbisect(np, sseq, min, mid, atmin, atmid, roots);
            arpolynomial_sbisect(np, sseq, mid, max, atmid, atmax, roots+n1);
            break;
        }

        if (n1 == 0)
            min = mid;
        else
            max = mid;
    }

    if (its == ARPOLYNOMIAL_MAXIT)
    {
        fprintf(stderr, "arpolynomial_sbisect: roots too close together\n");
        fprintf(stderr, "arpolynomial_sbisect: overflow min %f max %f diff %e\
                nroot %d n1 %d n2 %d\n",
                min, max, max - min, nroot, n1, n2);
        for (n1 = atmax; n1 < atmin; n1++)
        roots[n1 - atmax] = mid;
    }
}

#ifdef NEVERMORE

void driver()
{
    ArPolynomial        sseq[MAX_ORDER];
    double      min, max, roots[MAX_ORDER];
    int         i, j, order, nroots, nchanges, np, atmin, atmax;

    /*
          * get the details...
          */

    printf("Please enter order of polynomial: ");
    scanf("%d", &order);

    printf("\n");

    for (i = order; i >= 0; i--)
    {
        printf("Please enter coefficient number %d: ", i);
        scanf("%lf", &sseq[0].coef[i]);
    }

    printf("\n");

    /*
          * build the Sturm sequence
          */
    np = buildsturm(order, sseq);

    printf("Sturm sequence for:\n");

    for (i = order; i >= 0; i--)
                    printf("%lf ", sseq[0].coef[i]);

    printf("\n\n");

    for (i = 0; i <= np; i++) {
                    for (j = sseq[i].ord; j >= 0; j--)
                            printf("%lf ", sseq[i].coef[j]);
                    printf("\n");
    }

    printf("\n");


    /*
          * get the number of real roots
          */
    nroots = numroots(np, sseq, &atmin, &atmax);

    if (nroots == 0) {
                    printf("solve: no real roots\n");
                    exit(0);
    }

    /*
          * calculate the bracket that the roots live in
          */
    min = -1.0;
    nchanges = arpolynomial_numchanges(np, sseq, min);
    for (i = 0; nchanges != atmin && i != MAXPOW; i++) {
                    min *= 10.0;
                    nchanges = arpolynomial_numchanges(np, sseq, min);
    }

    if (nchanges != atmin) {
                    printf("solve: unable to bracket all negative roots\n");
                    atmin = nchanges;
    }

    max = 1.0;
    nchanges = arpolynomial_numchanges(np, sseq, max);
    for (i = 0; nchanges != atmax && i != MAXPOW; i++) {
                    max *= 10.0;
                    nchanges = arpolynomial_numchanges(np, sseq, max);
    }

    if (nchanges != atmax) {
                    printf("solve: unable to bracket all positive roots\n");
                    atmax = nchanges;
    }

    nroots = atmin - atmax;

    /*
          * perform the bisection.
          */
    arpolynomial_sbisect (np, sseq, min, max, atmin, atmax, roots);


    /*
          * write out the roots...
          */
    if (nroots == 1) {
                    printf("\n1 distinct real root at x = %f\n", roots[0]);
    } else {
                    printf("\n%d distinct real roots for x: ", nroots);

                    for (i = 0; i != nroots; i++)
                            printf("%f ", roots[i]);
                    printf("\n");
    }
}

#endif

/* ======================================================================== */
