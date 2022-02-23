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

#include "ART_Foundation_System.h"

ART_MODULE_INTERFACE(Astro)

// Julian Day from a time (UT)
double JDE(      int year,
                 int month,
           const double day,
           const double hours,
           const double minutes,
           const double seconds);

// Sidereal Time, degrees
double SidTimeDeg(const double JD,
                  const double easternLongitude);

// COORDINATE TRANSFORMS. All angles in radians. Meeus, ch13.
// ecliptical to equatorial coordinates
void Ecl2Equ(const double lambdaRad,
             const double betaRad,
             const double epsilonRad,
                   double *alphaRad,
                   double *deltaRad);
// Hour Angle/declination to azimuth (counted from South over West!) and altitude
void Hd2Aa(const double HaRad,
           const double deltaRad,
           const double latitudeRad,
                 double *azimuthSWNERad,
                 double *altitudeRad);

// True geometric longitude of sun at mean equinox JDE
double SolarLongitudeRad(const double JDE);

// obliquity of the ecliptic. T are Julian Centuries from J2000.0
double epsilonRad(const double T);

// compute solar azimuth and elevation for date JDE at place(latitude, longitude)
void solarPosition(const double JDE,
                   const double eastLongitudeRad,
                   const double latitudeRad,
                         double *azimuthSWNERad,
                         double *elevationRad);

/* Example usage in SkyModel models, e.g. Preetham:
- (id) init
    : (double) JDE
    : (double) eastLongitudeRad
    : (double) latitudeRad
    : (double) newTurbidity
    : (BOOL) newPolarisedOutput
{ // TASK: compute solar azimuth/elevation
  double elevationRad, azimuthSWNERad;
  solarPosition(JDE, eastLongitudeRad, latitudeRad, &azimuthSWNERad, &elevationRad) {
  // Now just call the other init:
  // NOTE: Preetham and other models use an azimuth counted from South over East.

  [init: elevationRad : -azimuthSWNERad : newTurbidity : newPolarisedOutput]
}

*/

// ===========================================================================
