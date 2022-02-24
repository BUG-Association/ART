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

#ifndef _ART_MATH_UNITS_H_
#define _ART_MATH_UNITS_H_

#define KELVIN                  * 1.0

#define KILOMETER               * 1.0E3
#define METER                   * 1.0
#define CENTIMETER              * 1.0E-2
#define MILLIMETER              * 1.0E-3
#define MICROMETER              * 1.0E-6

/* ---------------------------------------------------------------------------
    'NANOMETER'
        Used mainly in spectra.
--------------------------------------------------------------------------- */
#define NANOMETER               * 1.0E-9

#define KILOMETERS              KILOMETER
#define METERS                  METER
#define CENTIMETERS             CENTIMETER
#define MILLIMETERS             MILLIMETER
#define MICROMETERS             MICROMETER
#define NANOMETERS              NANOMETER

//   There is no definition of "M" for meter anymore since this proved to
//   be too generic and caused conflicts with external libraries.        (aw)

#define KM                      KILOMETER
#define CM                      CENTIMETER
#define MM                      MILLIMETER
#define NM                      NANOMETER

#define INCH                    * 0.0254
#define FOOT                    * 0.3048
#define YARD                    * 0.9144

#define INCHES                  INCH
#define FEET                    FOOT
#define YARDS                   YARD

#define LIGHTYEAR               * 9.4605300E15
#define PARSEC                  * 3.0856780E16
#define ASTRONOMICAL_UNIT       * 1.4959780E11

#define LIGHTYEARS              LIGHTYEAR
#define PARSECS                 PARSEC
#define ASTRONOMICAL_UNITS      ASTRONOMICAL_UNIT

#define LY                      LIGHTYEAR
#define AU                      ASTRONOMICAL_UNIT

#define KILO_FROM_UNIT(__len)           ((__len) * 1.0E-3)
#define KILO_FROM_MILLI(__len)          ((__len) * 1.0E-6)
#define KILO_FROM_MICRO(__len)          ((__len) * 1.0E-9)
#define KILO_FROM_NANO(__len)           ((__len) * 1.0E-12)

#define UNIT_FROM_KILO(__len)           ((__len) * 1.0E3)
#define UNIT_FROM_MILLI(__len)          ((__len) * 1.0E-3)
#define UNIT_FROM_MICRO(__len)          ((__len) * 1.0E-6)
#define UNIT_FROM_NANO(__len)           ((__len) * 1.0E-9)

#define MILLI_FROM_KILO(__len)          ((__len) * 1.0E6)
#define MILLI_FROM_UNIT(__len)          ((__len) * 1.0E3)
#define MILLI_FROM_MICRO(__len)         ((__len) * 1.0E-3)
#define MILLI_FROM_NANO(__len)          ((__len) * 1.0E-6)

#define MICRO_FROM_KILO(__len)          ((__len) * 1.0E9)
#define MICRO_FROM_UNIT(__len)          ((__len) * 1.0E6)
#define MICRO_FROM_MILLI(__len)         ((__len) * 1.0E3)
#define MICRO_FROM_NANO(__len)          ((__len) * 1.0E-3)

#define NANO_FROM_KILO(__len)           ((__len) * 1.0E12)
#define NANO_FROM_UNIT(__len)           ((__len) * 1.0E9)
#define NANO_FROM_MILLI(__len)          ((__len) * 1.0E6)
#define NANO_FROM_MICRO(__len)          ((__len) * 1.0E3)

/* ---------------------------------------------------------------------------
    'DEGREES'
        Defines a postfix operator with the precedence of the multiplication
        operator, that converts degrees into radians.  Use it like this:

                cos(42 DEGREES)

        But beware:

                cos(40 + 2 DEGREES)

        means '40 RADIANS + 2 DEGREES'.
--------------------------------------------------------------------------- */
#define DEGREES                 * MATH_DEG_TO_RAD

/* ---------------------------------------------------------------------------
    'DEGREE'
        A companion to the 'DEGREES' macro to allow 'cos(1 DEGREE)'.
--------------------------------------------------------------------------- */
#define DEGREE                  DEGREES

/* ---------------------------------------------------------------------------
    'RADIANS'
        Defined to allow expressions like:

                cos(40 RADIANS + 2 DEGREES)
--------------------------------------------------------------------------- */
#define RADIANS

/* ---------------------------------------------------------------------------
    'RADIAN
        A companion to the 'RADIANS' macro to allow 'cos(1 RADIAN)'.
--------------------------------------------------------------------------- */
#define RADIAN                  RADIANS

/* ---------------------------------------------------------------------------
    'PERCENT'
        To enable suitably wordy parameters to be passed in unit measure.
--------------------------------------------------------------------------- */
#define PERCENT                 * 0.01


#endif /* _ART_MATH_UNITS_H_ */
/* ======================================================================== */
