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

#define ART_MODULE_NAME     ArfVol

#import "ArfVol.h"

#import "ArnVolumeDataGrid.h"

static const char * arfvol_magic_string =
    "VOL";
static const char * arfvol_short_class_name =
    "VOL";
static const char * arfvol_long_class_name =
    "VOL volume data";
const char * arfvol_exts[] =
    { "vol", 0 };

ART_MODULE_INITIALISATION_FUNCTION
(
 [ ArfVol registerWithFileProbe
  :   art_gv
  ];
 )

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY


@implementation ArfVol

ARPFILE_DEFAULT_IMPLEMENTATION( ArfVol, arfiletypecapabilites_read )
ARPPARSER_AUXLIARY_NODE_DEFAULT_IMPLEMENTATION

+ (const char **) extensions
{
    return arfvol_exts;
}

+ (const char*) magicString
{
    return arfvol_magic_string;
}

- (const char*) shortClassName
{
    return arfvol_short_class_name;
}

- (const char*) longClassName
{
    return arfvol_long_class_name;
}

+ (ArFiletypeMatch) matchWithStream
        : (ArcObject <ArpStream> *) stream
{
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
        return arfiletypematch_impossible;
}

- (id) initWithFile: (ArcFile *) newFile
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

+ (BOOL) isHeaderCorrect
    : (char*) header
{
    return header[0] == 'V' && header[1] == 'O' && header[2] == 'L';
}

- (void) parseFileGetExternals
        : (ArNode **) objectPtr
        : (ArList *) externals
{
    ArnVolumeDataGrid * newVolume = [ ALLOC_INIT_OBJECT(ArnVolumeDataGrid) ];

    FILE *pFile = fopen( [ file name ], "rb" );
    
    if (pFile != NULL) {
        char header[3] = {0};
        char version = 0;
        int32_t encoding = 0;
        
        // Check the header
        fread(header, 1, 3, pFile);
        if (![ ArfVol isHeaderCorrect:header ]) {
            ART_ERRORHANDLING_FATAL_ERROR(
                "The VOL volume '%s' is not supported: Wrong header."
                ,   [ file name ]
                );
        }
        
        fread(&version, 1, 1, pFile);
        if (version != 3) {
            ART_ERRORHANDLING_FATAL_ERROR(
                "The VOL volume '%s' is not supported: Not supporting this version."
                ,   [ file name ]
                );
        }
        
        fread(&encoding, sizeof(int32_t), 1, pFile);
        if (encoding != 1) {
            ART_ERRORHANDLING_FATAL_ERROR(
                "The VOL volume '%s' is not supported: Not supporting this encoding format."
                ,   [ file name ]
                );
        }
        
        // Load volume size information
        fread(&(newVolume->_xRes), sizeof(int32_t), 1, pFile);
        fread(&(newVolume->_yRes), sizeof(int32_t), 1, pFile);
        fread(&(newVolume->_zRes), sizeof(int32_t), 1, pFile);
        fread(&(newVolume->_nChannels), sizeof(int32_t), 1, pFile);
        fread(&(newVolume->_xMin), sizeof(float), 1, pFile);
        fread(&(newVolume->_yMin), sizeof(float), 1, pFile);
        fread(&(newVolume->_zMin), sizeof(float), 1, pFile);
        fread(&(newVolume->_xMax), sizeof(float), 1, pFile);
        fread(&(newVolume->_yMax), sizeof(float), 1, pFile);
        fread(&(newVolume->_zMax), sizeof(float), 1, pFile);
        
        int n_data_el = newVolume->_xRes * newVolume->_yRes * newVolume->_zRes * newVolume->_nChannels;
        
        newVolume->_data = arfloatarray_init(n_data_el);
        fread(arfloatarray_array(&newVolume->_data), sizeof(Float), n_data_el, pFile);
        fclose(pFile);
        
        // find max
        // TODO: What is a max? Works in 1D then...
        newVolume->_max = 0;
        for (int i = 0; i < n_data_el; i++) {
            newVolume->_max = fmax(newVolume->_max, ARARRAY_I(newVolume->_data, i));
        }
    } else {
        ART_ERRORHANDLING_FATAL_ERROR(
                                      "cannot open VOL volume '%s'"
                                      ,   [ file name ]
                                      );
    }
    
    *objectPtr = newVolume;
}

- (void) parseStream
        : (ArNode **) objectPtr
        : (ArcObject <ArpStream> *) stream
{
    (void) objectPtr;
    (void) stream;
    
    ART__CODE_IS_WORK_IN_PROGRESS__EXIT_WITH_ERROR
}

@end

// ===========================================================================
