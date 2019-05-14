/* ===========================================================================
 
    Copyright (c) 1996-2018 The ART Development Team
    ------------------------------------------------

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

#define ART_MODULE_NAME     ArfMERL

#import "ArfMERL.h"

#import "ArnMERLSurfaceMaterial.h"
#import "merl.h"

static const char * arfmerl_magic_string =
    "";
static const char * arfmerl_short_class_name =
    "MERL";
static const char * arfmerl_long_class_name =
    "MERL measured BRDF";
const char * arfmerl_exts[] =
    { "binary", 0 };

ART_MODULE_INITIALISATION_FUNCTION
(
 [ ArfMERL registerWithFileProbe
  :   art_gv
  ];
 )

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArfMERL

ARPFILE_DEFAULT_IMPLEMENTATION( ArfMERL, arfiletypecapabilites_read )
ARPPARSER_AUXLIARY_NODE_DEFAULT_IMPLEMENTATION

+ (const char **) extensions
{
    return arfmerl_exts;
}

+ (const char*) magicString
{
    return arfmerl_magic_string;
}

- (const char*) shortClassName
{
    return arfmerl_short_class_name;
}

- (const char*) longClassName
{
    return arfmerl_long_class_name;
}

+ (ArFiletypeMatch) matchWithStream
        : (ArcObject <ArpStream> *) stream
{
    /*
    char  buffer[5];
    
    [ stream read
         :   buffer
         :   1
         :   4
         ];
    
    buffer[4] = 0;
    
    if ( strstr(buffer, [self magicString]) != 0 )
        return arfiletypematch_exact;
        else
    */
    return arfiletypematch_exact;
}

- initWithFile: (ArcFile *) newFile
{
    file = newFile;
    return self;
}

- (void) dealloc
{
    [ super dealloc ];
}

- (void) parseFile
        : (ArNode **) objectPtr
{
    [ self parseFileGetExternals
         :   objectPtr
         :   0
         ];
}

- (void) parseFileGetExternals
        : (ArNode **) objectPtr
        : (ArList *) externals
{
    ArnMERLSurfaceMaterial * newMaterial = [ ALLOC_INIT_OBJECT(ArnMERLSurfaceMaterial) ];

	FILE *f = fopen([file name], "rb");
	if (!f)
		return;

	int dims[3];
	fread(dims, sizeof(int), 3, f);
	int n = dims[0] * dims[1] * dims[2];
    
	if (n != MERL_SAMPLING_RES_THETA_H *
        MERL_SAMPLING_RES_THETA_D *
        MERL_SAMPLING_RES_PHI_D) 
	{
		fprintf(stderr, "Dimensions don't match\n");
		fclose(f);
		return;
	}

    newMaterial->brdf = ardoublearray_init(3 * n);
	fread(ardoublearray_array(&newMaterial->brdf), sizeof(double), 3*n, f);
               
	fclose(f);
    
    *objectPtr = newMaterial;
}

- (void) parseStream
        : (ArNode **) objectPtr
        : (ArcObject <ArpStream> *) stream
{
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

@end

// ===========================================================================
