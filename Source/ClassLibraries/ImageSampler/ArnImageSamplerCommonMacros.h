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

#define  GATHERING_ESTIMATOR              \
((ArNode <ArpPathspaceIntegrator> *)ARNODEREF_POINTER(subnodeRefArray[0]))
#define  RECONSTRUCTION_KERNEL   \
((ArNode <ArpReconstructionKernel> *) ARNODEREF_POINTER(subnodeRefArray[1]))

#ifndef REPORTER
#define  REPORTER               ART_GLOBAL_REPORTER
#endif

#define INCOMPLETE               1
#define FINISHED                 0
#define THREAD_INDEX             threadIndex->value

#define PIXEL_SAMPLE_COUNT(__x,__y,__threadIndex,__imageNo)  \
    samplesPerPixel[ \
          (__threadIndex) * numberOfImagesToWrite * XC(imageSize) * YC(imageSize) \
        + (__imageNo) * XC(imageSize) * YC(imageSize) \
        + (__x) + (__y) * XC(imageSize) \
        ]

#define PIXEL_SAMPLE_VALUE(__x,__y,__threadIndex,__imageNo)  \
    resultImage[ (__threadIndex) * numberOfImagesToWrite + (__imageNo) ]->data[(__x) + (__y) * XC(imageSize)]

#define THREAD_RESULT_PIXEL(__x,__y,__imageNo) \
    PIXEL_SAMPLE_VALUE(__x,__y,THREAD_INDEX,__imageNo)

#define RGB_PIXEL_SAMPLE_VALUE(__x,__y)  \
    resultImage->data[(__x) + (__y) * XC(imageSize)]

#define THREAD_RANDOM_GENERATOR         randomGenerator[ THREAD_INDEX ]
#define THREAD_PATHSPACE_INTEGRATOR     pathspaceIntegrator[ THREAD_INDEX ]
#define THREAD_RAYCOUNTER               pathCounter[ THREAD_INDEX ]
