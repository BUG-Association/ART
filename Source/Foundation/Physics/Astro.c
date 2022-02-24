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

#define ART_MODULE_NAME     Astro

#include "Astro.h"

#include "ART_Foundation_Math.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double JDE(int year, int month, const double day, const double hours, const double minutes, const double seconds) {
  // computation of Julian Day, according to Meeus, Astronomical Algorithms, 2nd ed. (1998)
  unsigned int gregorian; // gregorian are all dates starting with 1582-Oct-15
  // sanity check:
  if (year<-4712) {
    fprintf(stderr, "JDE failed: year must be >=-4712\n");
    exit(-1);
  }

  if (year>1582) gregorian=1;
  else if (year<1582) gregorian=0;
  else { // year==1582: go for month...
    if (month>10) gregorian=1;
    else if (month<10) gregorian=0;
    else { // month==10: go for day...
      if (day>14) gregorian=1;
      else gregorian=0;
    }
  }

  if (month<=2){
    year--;
    month+=12;
  }

  float A, B;
  if (gregorian) {
    A=floor((double)year/100.0);
    B=2.0-A+floor(A/4.0);
  }
  else B=0;

  double D=day+hours/24.0+minutes/(24.0*60.0)+seconds/86400.0;

  return floor(365.25*(year+4716))+floor(30.6001*(month+1))+D+B-1524.5;
}

double JD2T(const double JD){
  return (JD-2451545.0)/36525.0;
}

double epsilonRad(const double T){
  const double u=T/100.0;
  const double epsSec=(((((((((2.45*u + 5.79)*u + 27.87)*u + 7.12)*u -39.05)*u - 249.67)*u - 51.38)*u + 1999.25)*u - 1.55)*u - 4680.93)*u + 84381.448;
  return epsSec* (MATH_DEG_TO_RAD/3600.0);
}

double SidTimeDeg(const double JD, const double easternLongitudeDeg){
  // (mean) sidereal Time in Greenwich following Meeus, Astronomical Algorithms, 2nd ed. (1998)
  const double T=JD2T(JD);
  const double theta=(T/-38710000.0+0.000387933)*T*T+360.98564736629*(JD-2451545.0)+280.46061837; // 12.4
  return fmod(theta+easternLongitudeDeg, 360.0);
}

void Ecl2Equ(const double lambdaRad, const double betaRad, const double epsilonRad, double *alphaRad, double *deltaRad){
  // coordinate transform. All angles in radians. Meeus, ch13.
  *alphaRad=atan2(sin(lambdaRad)*cos(epsilonRad)-tan(betaRad)*sin(epsilonRad), cos(lambdaRad));
  *deltaRad=asin(sin(betaRad)*cos(epsilonRad)+cos(betaRad)*sin(epsilonRad)*sin(lambdaRad));
}

void Hd2Aa(const double HaRad, const double deltaRad, const double latitudeRad, double *azimuthSWNERad, double *altitudeRad){
  *azimuthSWNERad = atan2(sin(HaRad), cos(HaRad)*sin(latitudeRad)-tan(deltaRad)*cos(latitudeRad));
  *altitudeRad    = asin(sin(latitudeRad)*sin(deltaRad)+cos(latitudeRad)*cos(HaRad));
}


double SolarLongitudeRad(const double T) {
  double L0= ( 0.0003032*T+36000.76983)*T+280.46646;
  double M = (-0.0001537*T+35999.05029)*T+357.52911;
  //  double e = (-0.0000001267*T-0.000042037)*T+0.016708634;
  L0*=MATH_DEG_TO_RAD;
  M*= MATH_DEG_TO_RAD;
  double C = ((-0.000014*T-0.004817)*T+1.914602)*sin(M)
    +(0.019993-0.000101*T)*sin(2.0*M)
    +0.000289*sin(3.0*M);
  C*=MATH_DEG_TO_RAD;
  return L0+C;
}


// compute solar azimuth and elevation for date JDE at place(latitude, longitude)
void solarPosition(const double JDE, const double eastLongitudeRad, const double latitudeRad, double *azimuthSWNERad, double *elevationRad) {
  double RA, dec;
  const double T=JD2T(JDE);
  Ecl2Equ(SolarLongitudeRad(T), 0.0, epsilonRad(T), &RA, &dec);
  const double eastLongitudeDeg=eastLongitudeRad*MATH_RAD_TO_DEG;
  Hd2Aa(SidTimeDeg(JDE, eastLongitudeDeg)*MATH_DEG_TO_RAD - RA, dec, latitudeRad, azimuthSWNERad, elevationRad);
}

// ===========================================================================
