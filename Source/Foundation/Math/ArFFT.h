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

#ifndef _ART_MATH_FFT_H_
#define _ART_MATH_FFT_H_

typedef struct { double re; double im; }        ArFFTComplex;
typedef double  ArFFTReal;

typedef enum arfft_flag
{
    arfft_backward                      = 0x0,
    arfft_forward                       = 0x1
} arfft_flag;

#define ARFFTCOMPLEX_RE(_c)     (_c).re
#define ARFFTCOMPLEX_IM(_c)     (_c).im

int arfft2d_c2r(
    int n,
    int m,
    ArFFTComplex * input,
    ArFFTReal * output,
    arfft_flag flags
    );

int arfft2d_c2c(
    int n,
    int m,
    ArFFTComplex * input,
    ArFFTReal * output,
    arfft_flag flags
    );

void arcfft_initialize();

/* complex to real transform using halfcomplex data */
void arfft1dcr(double *buf, unsigned long size);

/* complex to complex 1d transform */
void arfft1d(double *bufr, double *bufi, unsigned long size, int sign);

/* complex to complex 2d transform */
void arfft2d(
    double *bufr,
    double *bufi,
    unsigned long nr,
    unsigned long nc,
    int sign
    );

void arfft2dcr(
    double *bufr,
    double *bufi,
    double *out,
    unsigned long nr,
    unsigned long nc
    );

#endif /* _ART_MATH_FFT_H_ */

// ===========================================================================
