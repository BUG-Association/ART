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

#include "ART_Foundation.h"

ART_MODULE_INTERFACE(ArpSpectrum)

/* ---------------------------------------------------------------------------
    'ArpSpectrum' protocol

    ArpSpectrum is the protocol which is implemented by all nodes that provide
    colour information and/or wavelength-dependent data. The idea behind its
    multi-type capabilites is that any kind of node that fits into the broad
    definition from the previous sentence should be able to return its
    contents in a variety of forms suited to different purposes.

    Basically this amounts to each node being capable of returning a colour
    space, light, attenuation, accurate spectral or wavelength sample version of
    itself. See the individual methods for specifics of their return types.

    Most methods in this protocol take two arguments: a locationInfo which
    contains information about the exact location and specifics for the
    evaluation which is taking place - (u,v) texture coordinates are an
    example of this; some of the more sophisticated colour nodes which are
    used for procedural texturing need this kind of information - and the
    return data type.
------------------------------------------------------------------------aw- */

@class ArcPointContext;

@protocol ArpSpectrum

/* ---------------------------------------------------------------------------
    'getSpectrum'

    Returns a ArSpectrum version of the underlying data. This is a semantically
    unconstrained data structure, the exact contents of which are dependent
    on the colour type the renderer is compiled for.
------------------------------------------------------------------------aw- */

- (void) getSpectrum
        : (ArcPointContext *) locationInfo
        : (ArSpectrum *) outSpectrum
        ;
- (void) getHiresSpectrum
        : (ArcPointContext *) locationInfo
        : (ArSpectrum500 *) outSpectrum
        ;

- (void) getSpectralSample
        : (const ArcPointContext *) locationInfo
        : (const ArWavelength *) wavelength
        : (      ArSpectralSample *) outSpectrum
        ;

- (void) getAttenuation
        : (ArcPointContext *) locationInfo
        : (ArAttenuation *) outAttenuation
        ;

- (void) getAttenuationSample
        : (const ArcPointContext *) locationInfo
        : (const ArWavelength *) wavelength
        : (      ArAttenuationSample *) outAttenuation
        ;

- (void) getDepolarisingAttenuation
        : (ArcPointContext *) locationInfo
        : (ArReferenceFrame *) refframeEntry
        : (ArReferenceFrame *) refframeExit
        : (ArAttenuation *) outAttenuation
        ;

- (void) getDepolarisingAttenuationSample
        : (const ArcPointContext *) locationInfo
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (const ArWavelength *) wavelength
        : (      ArAttenuationSample *) outAttenuation
        ;

- (void) getNonpolarisingAttenuation
        : (ArcPointContext *) locationInfo
        : (ArReferenceFrame *) refframeEntry
        : (ArReferenceFrame *) refframeExit
        : (ArAttenuation *) outAttenuation
        ;

- (void) getNonpolarisingAttenuationSample
        : (const ArcPointContext *) locationInfo
        : (const ArReferenceFrame *) refframeEntry
        : (const ArReferenceFrame *) refframeExit
        : (const ArWavelength *) wavelength
        : (      ArAttenuationSample *) outAttenuation
        ;


/* ---------------------------------------------------------------------------
    'getNewPSSpectrum'

    Returns an ArPSSpectrum version of the underlying data; the PSSpectrum
    struct contains dynamically allocated memory which has to be freed by the
    caller once he is done with the struct (this is why this method has 'new'
    in its name).

    The result is assumed to be the most accurate spectral representation you
    can get from this node.
------------------------------------------------------------------------aw- */

- (void) getNewPSSpectrum
        : (ArcPointContext *) locationInfo
        : (ArPSSpectrum *) outPSSpectrum
        ;


/* ---------------------------------------------------------------------------
    'valueAtWavelength'

    Returns the value of the node at a given wavelength.
------------------------------------------------------------------------aw- */

- (double) valueAtWavelength
        : (ArcPointContext *) locationInfo
        : (const double) wavelength
        ;

- (BOOL) isFluorescent
        ;

@end

@protocol ArpISRChange

- (void) prepareForISRChange
        ;

- (void) reinitialiseAfterISRChange
        ;

@end

// ===========================================================================
