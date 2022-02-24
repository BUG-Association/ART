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

ART_MODULE_INTERFACE(ExecutionEnvironment)

#import "ObjC_Runtime.h"

@class    ArcObject;
@protocol ArpReporter;

ArcObject <ArpReporter> * art_global_reporter(
        const ART_GV  * art_gv
        );

void art_set_global_reporter(
        ART_GV                   * art_gv,
        ArcObject <ArpReporter>  * newReporter
        );

unsigned int art_native_files_use_binary_io(
        const ART_GV  * art_gv
        );

void art_set_native_files_use_binary_io(
        ART_GV        * art_gv,
        unsigned int    binary
        );

unsigned int art_force_ar2mart_translation(
        const ART_GV  * art_gv
        );

void art_set_forcing_of_ar2mart_translation(
        ART_GV        * art_gv,
        unsigned int    force
        );

unsigned int art_maximum_number_of_working_threads(
        const ART_GV  * art_gv
        );

void art_set_maximum_number_of_working_threads(
        ART_GV        * art_gv,
        unsigned int    newThreadCap
        );


#define ART_GLOBAL_REPORTER     art_global_reporter(art_gv)

// ===========================================================================
