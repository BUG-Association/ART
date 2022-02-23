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

#define ART_MODULE_NAME     ArcBBoxCache

#import "ArcBBoxCache.h"

ART_NO_MODULE_INITIALISATION_FUNCTION_NECESSARY

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

typedef struct ArBBoxCacheEntry
{
    ArTreePath  * treepath;
    Box3D         bbox;
}
ArBBoxCacheEntry;

//TYPEDEF_ARHASH(ArBBoxCacheEntry, entry, data, ArBBoxCacheEntry);

//DEFINE_ARHASHTABLE_IMPLEMENTATION_FOR_TYPE(ArBBoxCacheEntry,bbce);

@implementation ArcBBoxCache


- (id) init
        : (unsigned int) initialSize
{
    self = [ super init ];

    if ( self )
    {
        bboxCache = artable_alloc_init_n( initialSize );
    }
    
    return self;
}

- (void) dealloc
{
    artable_free(bboxCache);
    
    [ super dealloc ];
}

- (id) copy
{
    ArcBBoxCache  * copiedInstance = [ super copy ];

    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR

//    artable_init_n( & copiedInstance->bboxCache, artable_size( & bboxCache ) );

    return copiedInstance;
}

- (void) addBBoxFor
        : (ArTreePath *) treepath
        : (Box3D *) inBBox
{
    (void) treepath;
    (void) inBBox;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
/*
    ArBBoxCacheEntry * hrln = NULL;

    hrln = ALLOC(ArBBoxCacheEntry);
    hrln->entry.hash = artreepath_hashvalue( treepath );
    hrln->data.treepath = artreepath_copy( treepath );
    hrln->data.bbox = *inBBox;
    arhashtable_add_entry(&bboxCache, hrln);
*/
}

- (BOOL) getBBoxObjectspaceFor
        : (ArTreePath *) treepath
        : (Box3D *) outBBox
{
    (void) treepath;
    (void) outBBox;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
/*
    ArBBoxCacheEntry * hrln = NULL;
    UInt32 hash;

    hash = artreepath_hashvalue( treepath );
    do
    {
        hrln = arhashtable_find_hash( &bboxCache, hash, hrln );
    }
    while (    hrln != NULL
            && !artreepath_equal( treepath, hrln->data.treepath ));

    if ( hrln )
    {
        *outBBox = hrln->data.bbox;
        return 1;
    }
*/
    return NO;
}

@end

// ===========================================================================
