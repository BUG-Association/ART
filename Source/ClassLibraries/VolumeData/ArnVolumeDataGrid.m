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

#define ART_MODULE_NAME ArnVolumeDataGrid

#import "ArnVolumeDataGrid.h"
#import "ARM_Trafo.h"

ART_MODULE_INITIALISATION_FUNCTION
(
     (void) art_gv;
     [ ArnVolumeDataGrid registerWithRuntime ];
)

ART_NO_MODULE_SHUTDOWN_FUNCTION_NECESSARY

@implementation ArnVolumeDataGrid

ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVolumeDataGrid)

- (id) copy
{
    ArnVolumeDataGrid  * copiedInstance = [ super copy ];

    copiedInstance->_xRes = _xRes;
    copiedInstance->_yRes = _yRes;
    copiedInstance->_zRes = _zRes;
    
    copiedInstance->_nChannels = _nChannels;
    
    copiedInstance->_xMin = _xMin;
    copiedInstance->_yMin = _yMin;
    copiedInstance->_zMin = _zMin;
    
    copiedInstance->_xMax = _xMax;
    copiedInstance->_yMax = _yMax;
    copiedInstance->_zMax = _zMax;
    
    copiedInstance->_max = _max;
    
    int n_data_el = _xRes * _yRes * _zRes * _nChannels;
    copiedInstance->_data = arfloatarray_init(n_data_el);
    
    memcpy(arfloatarray_array(&copiedInstance->_data),
           arfloatarray_array(&_data),
           n_data_el * sizeof(Float));
    
    copiedInstance->world2local = world2local;
    copiedInstance->local2world = local2world;

    return copiedInstance;
}

- (id) deepSemanticCopy
        : (ArnGraphTraversal *) traversal
{
    ArnVolumeDataGrid  * copiedInstance =
        [ super deepSemanticCopy
            :   traversal
            ];

    copiedInstance->_xRes = _xRes;
    copiedInstance->_yRes = _yRes;
    copiedInstance->_zRes = _zRes;
    
    copiedInstance->_nChannels = _nChannels;
    
    copiedInstance->_xMin = _xMin;
    copiedInstance->_yMin = _yMin;
    copiedInstance->_zMin = _zMin;
    
    copiedInstance->_xMax = _xMax;
    copiedInstance->_yMax = _yMax;
    copiedInstance->_zMax = _zMax;
    
    copiedInstance->_max = _max;
    
    int n_data_el = _xRes * _yRes * _zRes * _nChannels;
    copiedInstance->_data = arfloatarray_init(n_data_el);

    memcpy(arfloatarray_array(&copiedInstance->_data),
           arfloatarray_array(&_data),
           n_data_el * sizeof(Float));
    
    copiedInstance->world2local = world2local;
    copiedInstance->local2world = local2world;
    
    return copiedInstance;
}

- (void) dealloc
{
    arfloatarray_free_contents(&_data);
    [super dealloc];
}

+ (BOOL) isHeaderCorrect
        : (char*) header
{
    return header[0] == 'V' && header[1] == 'O' && header[2] == 'L';
}

- (void) max
    : (double *) value
{
    *value = _max;
}

- (BOOL) rayIntersect
        : (const Ray3D  *) ray
        : (      double *) near
        : (      double *) far
{
    *near = -MATH_HUGE_DOUBLE;
    *far  =  MATH_HUGE_DOUBLE;
    
    const double minP[3] = { _xMin, _yMin, _zMin };
    const double maxP[3] = { _xMax, _yMax, _zMax };
    
    Ray3D localRay;
    
    vec3d_v_htrafo3d_v(
        & ray->vector,
        & world2local,
        & RAY3D_VECTOR(localRay)
        );
    
    pnt3d_p_htrafo3d_p(
        & ray->point,
        & world2local,
        & RAY3D_POINT(localRay)
        );
    
    const double origin[3]    = { XC(localRay.point) , YC(localRay.point) , ZC(localRay.point)  };
    const double direction[3] = { XC(localRay.vector), YC(localRay.vector), ZC(localRay.vector) };
    
    for ( int i = 0; i < 3; i++ ) {
        const double minVal = minP[i], maxVal = maxP[i];
        
        if ( direction[i] == 0 )
        {
            // The ray is parallel to the planes
            if ( origin[i] < minVal || origin[i] > maxVal )
            {
                return NO;
            }
        }
        else
        {
            // Calculate intersection distances
            double t1 = (minVal - origin[i]) / direction[i];
            double t2 = (maxVal - origin[i]) / direction[i];
            
            if ( t1 > t2 )
            {
                // swap t1, t2
                double tmp = t1; t1 = t2; t2 = tmp;
            }
            
            *near = MAX(t1, *near);
            *far  = MIN(t2, *far);
            
            if ( ! (*near <= *far) )
            {
                return NO;
            }
        }
    }

    return YES;
}

- (BOOL) isInside
       : (const Pnt3D *) point_wordspace
{
    Pnt3D pointLocalSpace;
    
    pnt3d_p_htrafo3d_p(
          point_wordspace,
        & world2local,
        & pointLocalSpace
        );
    
    const double
        x = XC(pointLocalSpace),
        y = YC(pointLocalSpace),
        z = ZC(pointLocalSpace);

    return
        x >= _xMin && x < _xMax &&
        y >= _yMin && y < _yMax &&
        z >= _xMin && z < _zMax;
}

- (void) lookup
        : (const Pnt3D *) point_wordspace
        : (     double *) value
{
    
    Pnt3D pointLocalSpace;
    pnt3d_p_htrafo3d_p(
          point_wordspace,
        & world2local,
        & pointLocalSpace
        );
    
    const double
        x = XC(pointLocalSpace),
        y = YC(pointLocalSpace),
        z = ZC(pointLocalSpace);

    const int
        x_idx = round((x - _xMin) / (_xMax - _xMin) * (double)_xRes),
        y_idx = round((y - _yMin) / (_yMax - _yMin) * (double)_yRes),
        z_idx = round((z - _zMin) / (_zMax - _zMin) * (double)_zRes);

    // Check if we're inside the volume
    if (x_idx < 0 || x_idx >= _xRes ||
        y_idx < 0 || y_idx >= _yRes ||
        z_idx < 0 || z_idx >= _zRes){

        for (int i = 0; i < _nChannels; i++) {
            value[i] = 0;
        }
    } else {
        int offset = ((z_idx*_yRes + y_idx)*_xRes + x_idx)*_nChannels;
        
        for (int i = 0; i < _nChannels; i++) {
            value[i] = MAX(0, (double)ARARRAY_I(_data, offset + i) - FLT_EPSILON);
        }
    }
}

- (int) dimensions
{
    return _nChannels;
}

/**
 * Checks if all the nodes are ArpTrafo3D
 * \param n number of nodes in attr array
 * \param attr array of nodes to check
 * \return YES if all the nodes are conforming to ArpTrafo3D protocol, NO otherwise
 */
+ (BOOL) validTransformAttribute
        : (unsigned int) n
        : (ArNode **) attr
{
    BOOL retVal = true;
    
    for ( size_t i = 0; i < n && retVal; i++) {
        retVal = retVal && [ attr[i] conformsToArProtocol: ARPROTOCOL(ArpTrafo3D)];
    }
    
    return retVal;
}



- (id) trafo
        : (ArNode *) newTrafo
{
    if ( [ newTrafo conformsToArProtocol: ARPROTOCOL(ArpTrafo3D) ] )
    {
        [ (ArNode<ArpTrafo3D>*)newTrafo getHTrafo3Ds
             : & local2world
             : & world2local
             ];
    } else {
        ART_ERRORHANDLING_FATAL_ERROR(
            "required transformation protocol not supported"
            );
    }
    
    return self;
}

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
{
    return
        [ self trafo
             :   arntrafopair(
                              art_gv,
                              newTrafo0,
                              newTrafo1
                              )
             ];
}

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
{
    return
        [ self trafo
             :   arntrafopair(
                              art_gv,
                              arntrafopair(
                                           art_gv,
                                           newTrafo0,
                                           newTrafo1
                                           ),
                              newTrafo2
                              )
             ];
}

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        : (ArNode *) newTrafo3
{
    return
    [ self trafo
     :   arntrafopair(
                      art_gv,
                      arntrafopair(
                                   art_gv,
                                   newTrafo0,
                                   newTrafo1
                                   ),
                      arntrafopair(
                                   art_gv,
                                   newTrafo2,
                                   newTrafo3
                                   )
                      )
     ];
}

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        : (ArNode *) newTrafo3
        : (ArNode *) newTrafo4
{
    return
    [ self trafo
     :   arntrafopair(
                      art_gv,
                      arntrafopair(
                                   art_gv,
                                   arntrafopair(
                                                art_gv,
                                                newTrafo0,
                                                newTrafo1
                                                ),
                                   arntrafopair(
                                                art_gv,
                                                newTrafo2,
                                                newTrafo3
                                                )
                                   ),
                      newTrafo4
                      )
     ];
}

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        : (ArNode *) newTrafo3
        : (ArNode *) newTrafo4
        : (ArNode *) newTrafo5
{
    return
		[ self trafo
			:   arntrafopair(
							 art_gv,
							 arntrafopair(
										  art_gv,
										  arntrafopair(
													   art_gv,
													   newTrafo0,
													   newTrafo1
													   ),
										  arntrafopair(
													   art_gv,
													   newTrafo2,
													   newTrafo3
													   )
										  ),
							 arntrafopair(
										  art_gv,
										  newTrafo4,
                                   newTrafo5
										  )
							 
                      )
		  ];
}

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        : (ArNode *) newTrafo3
        : (ArNode *) newTrafo4
        : (ArNode *) newTrafo5
        : (ArNode *) newTrafo6
{
    return
    [ self trafo
     :   arntrafopair(
                      art_gv,
                      arntrafopair(
                                   art_gv,
                                   arntrafopair(
                                                art_gv,
                                                newTrafo0,
                                                newTrafo1
                                                ),
                                   arntrafopair(
                                                art_gv,
                                                newTrafo2,
                                                newTrafo3
                                                )
                                   ),
                      arntrafopair(
                                   art_gv,
                                   arntrafopair(
                                                art_gv,
                                                newTrafo4,
                                                newTrafo5
                                                ),
                                   newTrafo6
                                   )
                      
                      )
     ];
}

- (id) trafo
        : (ArNode *) newTrafo0
        : (ArNode *) newTrafo1
        : (ArNode *) newTrafo2
        : (ArNode *) newTrafo3
        : (ArNode *) newTrafo4
        : (ArNode *) newTrafo5
        : (ArNode *) newTrafo6
        : (ArNode *) newTrafo7
{
    return
    [ self trafo
     :   arntrafopair(
                      art_gv,
                      arntrafopair(
                                   art_gv,
                                   arntrafopair(
                                                art_gv,
                                                newTrafo0,
                                                newTrafo1
                                                ),
                                   arntrafopair(
                                                art_gv,
                                                newTrafo2,
                                                newTrafo3
                                                )
                                   ),
                      arntrafopair(
                                   art_gv,
                                   arntrafopair(
                                                art_gv,
                                                newTrafo4,
                                                newTrafo5
                                                ),
                                   arntrafopair(
                                                art_gv,
                                                newTrafo6,
                                                newTrafo7
                                                )
                                   )
                      
                      )
     ];
}


/**
 * This is design to mask the node apply method in order to catch
 * transforms that are applied to this volume
 */
- (ArNode *) apply
        : (ArNode *) attr0
{
	return
		[ self trafo
			: attr0
		  ];
}



- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
{
	return
		[ self trafo
			: attr0
			: attr1
		  ];
}

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
{
	return
		[ self trafo
			: attr0
			: attr1
			: attr2
		  ];
}

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
{
	return
		[ self trafo
			: attr0
			: attr1
			: attr2
			: attr3
		  ];
}

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
{
    return
		[ self trafo
			: attr0
			: attr1
			: attr2
			: attr3
			: attr4
		  ];
}

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
        : (ArNode *) attr5
{
    return
		[ self trafo
			: attr0
			: attr1
			: attr2
			: attr3
			: attr4
			: attr5
		  ];
}

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
        : (ArNode *) attr5
        : (ArNode *) attr6
{
    return
		[ self trafo
			: attr0
			: attr1
			: attr2
			: attr3
			: attr4
			: attr5
			: attr6
		  ];
}

- (ArNode *) apply
        : (ArNode *) attr0
        : (ArNode *) attr1
        : (ArNode *) attr2
        : (ArNode *) attr3
        : (ArNode *) attr4
        : (ArNode *) attr5
        : (ArNode *) attr6
        : (ArNode *) attr7
{
	return
		[ self trafo
			: attr0
			: attr1
			: attr2
			: attr3
			: attr4
			: attr5
			: attr6
			: attr7
		  ];
}

- (void) code
  		 : (ArcObject <ArpCoder> *) coder
{
    [ super code: coder ];
    
    [ coder codeInt: & _xRes ];
    [ coder codeInt: & _yRes ];
    [ coder codeInt: & _zRes ];
    [ coder codeInt: & _nChannels ];
    [ coder codeFloat: & _xMin ];
    [ coder codeFloat: & _yMin ];
    [ coder codeFloat: & _zMin ];
    [ coder codeFloat: & _xMax ];
    [ coder codeFloat: & _yMax ];
    [ coder codeFloat: & _zMax ];
    
    arpcoder_arfloatarray(coder, &_data);
    
    [ coder codeFloat: & _max ];
    
    [ coder codeHTrafo3D: & world2local ];
    [ coder codeHTrafo3D: & local2world ];
}

@end
    
// ===========================================================================
