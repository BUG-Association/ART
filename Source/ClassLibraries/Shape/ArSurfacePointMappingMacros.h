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

//   NEVER include these macros in a header file - they are ONLY intended
//   for use within the .m files where 2D<->3D mappings are implemented!

#define IN_3D_HAS_INVALID_FACEINDEX \
    PNT3DE_FACEINDEX(*inPoint3D) < 0
#define IN_3D_FACEINDEX                 PNT3DE_FACEINDEX(*inPoint3D)
#define IN_3D_NORMAL                    PNT3DE_NORMAL(*inPoint3D)
#define IN_3D_COORD                     PNT3DE_COORD(*inPoint3D)
#define IN_3D_XC                        PNT3DE_XC(*inPoint3D)
#define IN_3D_YC                        PNT3DE_YC(*inPoint3D)
#define IN_3D_ZC                        PNT3DE_ZC(*inPoint3D)

#define OUT_2D_PATCHINDEX               PNT2DE_PATCHINDEX(*outPoint2D)
#define OUT_2D_COORD                    PNT2DE_COORD(*outPoint2D)
#define OUT_2D_U                        PNT2DE_U(*outPoint2D)
#define OUT_2D_V                        PNT2DE_V(*outPoint2D)

#define IN_2D_PATCHINDEX                PNT2DE_PATCHINDEX(*inPoint2D)
#define IN_2D_COORD                     PNT2DE_COORD(*inPoint2D)
#define IN_2D_U                         PNT2DE_U(*inPoint2D)
#define IN_2D_V                         PNT2DE_V(*inPoint2D)

#define OUT_3D_FACEINDEX                PNT3DE_FACEINDEX(*outPoint3D)
#define OUT_3D_COORD                    PNT3DE_COORD(*outPoint3D)
#define OUT_3D_NORMAL                   PNT3DE_NORMAL(*outPoint3D)
#define OUT_3D_XC                       PNT3DE_XC(*outPoint3D)
#define OUT_3D_YC                       PNT3DE_YC(*outPoint3D)
#define OUT_3D_ZC                       PNT3DE_ZC(*outPoint3D)
#define OUT_3D_XN                       PNT3DE_XN(*outPoint3D)
#define OUT_3D_YN                       PNT3DE_YN(*outPoint3D)
#define OUT_3D_ZN                       PNT3DE_ZN(*outPoint3D)

// ===========================================================================
