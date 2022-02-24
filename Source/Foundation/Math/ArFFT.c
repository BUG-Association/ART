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

#include "ART_Foundation_Math.h"

#include "ArFFT.h"

#ifdef ARFFT_USE_FFTW

int arfft2d_c2r(
    int n,
    int m,
    ArFFTComplex * input,
    ArFFTReal * output,
    arfft_flag flags
    )
{
/*    fftwnd_plan plan =
        rfftw2d_create_plan(n, m, FFTW_COMPLEX_TO_REAL, FFTW_ESTIMATE);

    rfftwnd_one_complex_to_real(plan, input, output);

    rfftwnd_destroy_plan(plan); */

    return 0;
}

int arfft2d_c2c(
    int n,
    int m,
    ArFFTComplex * input,
    ArFFTReal * output,
    arfft_flag flags
    )
{
/*    int i, j;

    fftwnd_plan plan =
        fftw2d_create_plan(n, m, FFTW_BACKWARD, FFTW_IN_PLACE);

    fftwnd_one(plan, input, NULL);

    for(i = 0; i < n; i++)
      for(j = 0; j < m; j++)
        output[i * n + j] = input[i * n + j].re;

    fftwnd_destroy_plan(plan); */

    return 0;
}

#endif /* ARFFT_USE_FFTW */

static inline void sumdiff(double a, double b, double *s, double *d)
{
  *s = a + b;
  *d = a - b;
}

static inline void sumdiff05(double a, double b, double *s, double *d)
{
  *s = (a + b) * 0.5;
  *d = (a - b) * 0.5;
}

static inline void skip_copy(
    double *src,
    double *dst,
    unsigned long n,
    unsigned long d
    )
{
  unsigned long i, j;

  for(i = 0, j = 0; j < n; i += d, j++)
    dst[j] = src[i];
}

static inline void skip_copy_back(
    double *src,
    double *dst,
    unsigned long n,
    unsigned long d
    )
{
  unsigned long i, j;

  for(i = 0, j = 0; j < n; i += d, j++)
    dst[i] = src[j];
}

static void skip_fft(
    double *bufr,
    double *bufi,
    unsigned long n,
    unsigned long d,
    double *wr,
    double *wi,
    int sign)
{
  skip_copy(bufr, wr, n, d);
  skip_copy(bufi, wi, n, d);

  arfft1d(wr, wi, n, sign);

  skip_copy_back(wr, bufr, n, d);
  skip_copy_back(wi, bufi, n, d);
}

void arfft1dcr(double *buf, unsigned long size)
{
  unsigned long i, j;
  double t;

  for(i = 1, j = size - 1; i < j; i++, j--) {
    t = buf[i] - buf[j];

    buf[j] += buf[i];
    buf[i] = t;
  }

  vector_hartley_transform(buf, size, 1);
}

void arfft1d(double *bufr, double *bufi, unsigned long size, int sign)
{
  unsigned long i, j;
  double q, r, s, t;

  /* do it */

  vector_hartley_transform(bufr, size, 1);
  vector_hartley_transform(bufi, size, 1);

  /* fht_fft_conversion */

  if (sign > 0) {
    for(i = 1, j = size - 1; i < j; i++, j--) {
      sumdiff(bufr[i], bufr[j], &q, &r);
      sumdiff(bufi[i], bufi[j], &s, &t);
      sumdiff05(q, t, &bufr[j], &bufr[i]);
      sumdiff05(s, r, &bufi[i], &bufi[j]);
    }
  } else {
    for(i = 1, j = size - 1; i < j; i++, j--) {
      sumdiff(bufr[j], bufr[i], &q, &r);
      sumdiff(bufi[j], bufi[i], &s, &t);
      sumdiff05(q, t, &bufr[j], &bufr[i]);
      sumdiff05(s, r, &bufi[i], &bufi[j]);
    }
  }

}

void arfft2d(
    double *bufr,
    double *bufi,
    unsigned long nr,
    unsigned long nc, int sign
    )
{
  unsigned long size, i;
  double *wr;
  double *wi;

  size = nr * nc;

  /* fft over rows */
  for(i = 0; i < size; i += nc)
    arfft1d(bufr + i, bufi + i, nc, sign);

  /* fft over columns */
  wr = (double *) malloc(sizeof(double) * 2 * nr);
  wi = wr + nr;

  for(i = 0; i < nc; i++)
    skip_fft(bufr + i, bufi + i, nr, nc, wr, wi, sign);

  free(wr);
}


void arfft2dcr(
    double *bufr,
    double *bufi,
    double *out,
    unsigned long nr,
    unsigned long nc
    )
{
  double *mybufr, *mybufi;

  mybufr = (double *) malloc(sizeof(double) * nr * nc);
  mybufi = (double *) malloc(sizeof(double) * nr * nc);


  /* treat first row special */
  mybufr[0] = bufr[0];
  mybufi[0] = -bufi[0];

  for(unsigned long j = 1; j < nc / 2; j++) {
    mybufr[j] = bufr[j];
    mybufi[j] = -bufi[j];

    mybufr[nc  - j] = bufr[j];
    mybufi[nc  - j] = bufi[j];
  }

  mybufr[nc/2] = bufr[nc/2];
  mybufi[nc/2] = -bufi[nc/2];

  /* now do the other rows */

  for(unsigned long i = 1; i < nr; i++) {

    mybufr[i * nc] = bufr[i * (nc / 2 + 1)];
    mybufi[i * nc] = -bufi[i * (nc / 2 + 1)];

    for(unsigned long j = 1; j < nc / 2; j++) {
      mybufr[i * nc + j] = bufr[i * (nc / 2 + 1) + j];
      mybufi[i * nc + j] = -bufi[i * (nc / 2 + 1) + j];

      mybufr[(nr - i) * nc + (nc - j)] = bufr[i * (nc / 2 + 1) + j];
      mybufi[(nr - i) * nc + (nc - j)] = bufi[i * (nc / 2 + 1) + j];
    }

    mybufr[i * nc + nc/2] = bufr[i * (nc / 2 + 1) + nc/2];
    mybufi[i * nc + nc/2] = -bufi[i * (nc / 2 + 1) + nc/2];
  }

  arfft2d(mybufr, mybufi, nr, nc, -1);

  if (out != NULL) {
    for(unsigned long i = 0; i < nr; i++) {
      for(unsigned long j = 0; j < nc; j++) {
        out[i * nc + j] = mybufr[i * nc + j];
      }
    }
  } else {
    for(unsigned long i = 0; i < nr; i++) {
      for(unsigned long j = 0; j < nc; j++) {
        bufr[i * nc + j] = mybufr[i * nc + j];
      }
    }
  }
}

// ===========================================================================
