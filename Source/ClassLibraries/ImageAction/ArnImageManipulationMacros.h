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

    /* ------------------------------------------------------------------
         This file is only intended to be #included from .m-files which
         implement image manipulation actions derived from
         ArnSingleImageManipulationAction or
         ArnDualImageManipulationAction!

         All it does is provide macros which allow access to the source
         and destination scanline buffers provided by the parent class
         in a human-comprehensible way.
    ---------------------------------------------------------------aw- */


#define LIGHTALPHA_SOURCE_BUFFER_I(_x,_i) \
    ((ArnLightAlphaImage*)(sourceScanlineBuffer[_i]))->data[(_x)]

#define LIGHTALPHA_SOURCE_BUFFER(_x) \
    LIGHTALPHA_SOURCE_BUFFER_I(_x,0)

#define LIGHTALPHA_SOURCE_BUFFER_LIGHT(_x) \
    ARLIGHTALPHA_LIGHT(*LIGHTALPHA_SOURCE_BUFFER(_x))

#define LIGHTALPHA_SOURCE_BUFFER_LIGHT_I(_x,_i) \
    ARLIGHTALPHA_LIGHT(*LIGHTALPHA_SOURCE_BUFFER_I(_x,_i))

#define LIGHTALPHA_SOURCE_BUFFER_ALPHA(_x) \
    ARLIGHTALPHA_A(*LIGHTALPHA_SOURCE_BUFFER(_x))

#define LIGHTALPHA_SOURCE_BUFFER_ALPHA_I(_x,_i) \
    ARLIGHTALPHA_A(*LIGHTALPHA_SOURCE_BUFFER_I(_x,_i))

#define LIGHTALPHA_SOURCE_BUFFER_A(_x) \
    ((ArnLightAlphaImage*)sourceScanlineBufferA)->data[(_x)]

#define LIGHTALPHA_SOURCE_BUFFER_A_LIGHT(_x) \
    ARLIGHTALPHA_LIGHT(*((ArnLightAlphaImage*)sourceScanlineBufferA)->data[(_x)])

#define LIGHTALPHA_SOURCE_BUFFER_A_ALPHA(_x) \
    ARLIGHTALPHA_A(*((ArnLightAlphaImage*)sourceScanlineBufferA)->data[(_x)])

#define LIGHTALPHA_SOURCE_BUFFER_B(_x) \
    ((ArnLightAlphaImage*)sourceScanlineBufferB)->data[(_x)]

#define LIGHTALPHA_SOURCE_BUFFER_B_LIGHT(_x) \
    ARLIGHTALPHA_LIGHT(*((ArnLightAlphaImage*)sourceScanlineBufferB)->data[(_x)])

#define LIGHTALPHA_SOURCE_BUFFER_B_ALPHA(_x) \
    ARLIGHTALPHA_A(*((ArnLightAlphaImage*)sourceScanlineBufferB)->data[(_x)])

#define GREY_SOURCE_BUFFER(_x) \
    (((ArnGreyImage*)sourceScanlineBuffer[0])->data[(_x)])

#define GREY_SOURCE_BUFFER_G(_x) \
    ARGREY_G(((ArnGreyImage*)sourceScanlineBuffer[0])->data[(_x)])

#define GREYALPHA_SOURCE_BUFFER(_x) \
    (((ArnGreyAlphaImage*)sourceScanlineBuffer[0])->data[(_x)])

#define GREYALPHA_SOURCE_BUFFER_G(_x) \
    ARGREYALPHA_G(((ArnGreyAlphaImage*)sourceScanlineBuffer[0])->data[(_x)])

#define GREYALPHA_SOURCE_BUFFER_A(_x) \
    ARGREYALPHA_A(((ArnGreyAlphaImage*)sourceScanlineBuffer[0])->data[(_x)])

#define XYZA_SOURCE_BUFFER_XYZ(_x) \
    ARCIEXYZA_C(((ArnCIEXYZAImage*)sourceScanlineBuffer[0])->data[(_x)])

#define XYZA_SOURCE_BUFFER_A_XYZ(_x) \
    ARCIEXYZA_C(((ArnCIEXYZAImage*)sourceScanlineBufferA)->data[(_x)])

#define XYZA_SOURCE_BUFFER_B_XYZ(_x) \
    ARCIEXYZA_C(((ArnCIEXYZAImage*)sourceScanlineBufferB)->data[(_x)])

#define XYZA_SOURCE_BUFFER_A_ALPHA(_x) \
    ARCIEXYZA_A(((ArnCIEXYZAImage*)sourceScanlineBufferA)->data[(_x)])

#define XYZA_SOURCE_BUFFER_B_ALPHA(_x) \
    ARCIEXYZA_A(((ArnCIEXYZAImage*)sourceScanlineBufferB)->data[(_x)])

#define XYZA_SOURCE_BUFFER_ALPHA(_x) \
    ARCIEXYZA_A(((ArnCIEXYZAImage*)sourceScanlineBuffer[0])->data[(_x)])

#define RGB_SOURCE_BUFFER(_x) \
    (((ArnRGBImage*)sourceScanlineBuffer[0])->data[(_x)])

#define RGBA_SOURCE_BUFFER(_x) \
    (((ArnRGBAImage*)sourceScanlineBuffer[0])->data[(_x)])

#define RGBA_SOURCE_BUFFER_RGB(_x) \
    ARRGBA_C(((ArnRGBAImage*)sourceScanlineBuffer[0])->data[(_x)])

#define RGBA_SOURCE_BUFFER_ALPHA(_x) \
    ARRGBA_A(((ArnRGBAImage*)sourceScanlineBuffer[0])->data[(_x)])

#define RGB24_SOURCE_BUFFER(_x) \
    (((ArnRGB24Image*)sourceScanlineBuffer[0])->data[(_x)])

#define RGBA32_SOURCE_BUFFER(_x) \
    (((ArnRGBA32Image*)sourceScanlineBuffer[0])->data[(_x)])

#define RGBA32_SOURCE_BUFFER_ALPHA(_x) \
    ARRGBA32_A(((ArnRGBA32Image*)sourceScanlineBuffer[0])->data[(_x)])

#define RGB48_SOURCE_BUFFER(_x) \
    (((ArnRGB48Image*)sourceScanlineBuffer[0])->data[(_x)])

#define RGBA64_SOURCE_BUFFER(_x) \
    (((ArnRGBA64Image*)sourceScanlineBuffer[0])->data[(_x)])

#define RGBA64_SOURCE_BUFFER_ALPHA(_x) \
    ARRGBA64_A(((ArnRGBA64Image*)sourceScanlineBuffer[0])->data[(_x)])


#define LIGHTALPHA_DESTINATION_BUFFER(_x) \
    ((ArnLightAlphaImage*)destinationScanlineBuffer)->data[(_x)]

#define LIGHTALPHA_DESTINATION_BUFFER_LIGHT(_x) \
    ARLIGHTALPHA_LIGHT(*((ArnLightAlphaImage*)destinationScanlineBuffer)->data[(_x)])

#define LIGHTALPHA_DESTINATION_BUFFER_ALPHA(_x) \
    ARLIGHTALPHA_A(*((ArnLightAlphaImage*)destinationScanlineBuffer)->data[(_x)])

#define XYZA_DESTINATION_BUFFER_XYZ(_x) \
    ARCIEXYZA_C(((ArnCIEXYZAImage*)destinationScanlineBuffer)->data[(_x)])

#define XYZA_DESTINATION_BUFFER_ALPHA(_x) \
    ARCIEXYZA_A(((ArnCIEXYZAImage*)destinationScanlineBuffer)->data[(_x)])

#define GREY_DESTINATION_BUFFER(_x) \
    ((ArnGreyImage*)destinationScanlineBuffer)->data[(_x)]

#define GREY_DESTINATION_BUFFER_G(_x) \
    ARGREY_G(((ArnGreyImage*)destinationScanlineBuffer)->data[(_x)])

#define GREYALPHA_DESTINATION_BUFFER(_x) \
   ((ArnGreyAlphaImage*)destinationScanlineBuffer)->data[(_x)]

#define GREYALPHA_DESTINATION_BUFFER_G(_x) \
    ARGREYALPHA_G(((ArnGreyAlphaImage*)destinationScanlineBuffer)->data[(_x)])

#define GREYALPHA_DESTINATION_BUFFER_A(_x) \
    ARGREYALPHA_A(((ArnGreyAlphaImage*)destinationScanlineBuffer)->data[(_x)])

#define RGBA_DESTINATION_BUFFER_RGBA(_x) \
    ((ArnRGBAImage*)destinationScanlineBuffer)->data[(_x)]

#define RGBA_DESTINATION_BUFFER_RGB(_x) \
    ARRGBA_C(((ArnRGBAImage*)destinationScanlineBuffer)->data[(_x)])

#define RGBA_DESTINATION_BUFFER_ALPHA(_x) \
    ARRGBA_A(((ArnRGBAImage*)destinationScanlineBuffer)->data[(_x)])


// ===========================================================================
