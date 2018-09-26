SET(_include_paths /usr/GNUstep/System/Library/Headers/Foundation  /usr/GNUstep/Local/Library/Headers/Foundation /usr/include/ /usr/local/include/ /usr/GNUstep/System/Library/Headers /usr/GNUstep/Local/Library/Headers /usr/include/GNUstep /usr/include/GNUstep/Foundation /usr/lib64/GNUstep/Headers /usr/local/include/Foundation)

FIND_PATH(GNUSTEP-BASE_INCLUDE_DIR Foundation/Foundation.h ${_include_paths})
IF (NOT GNUSTEP-BASE_INCLUDE_DIR)
   FIND_PATH(GNUSTEP-BASE_INCLUDE_DIR Foundation.h ${_include_paths})
ENDIF (NOT GNUSTEP-BASE_INCLUDE_DIR)
FIND_LIBRARY(GNUSTEP-BASE_LIBRARY NAMES gnustep-base PATHS /usr/GNUstep/System/Library/Libraries /usr/GNUstep/Local/Library/Libraries/ /lib /usr/lib /usr/local/lib)

IF (GNUSTEP-BASE_INCLUDE_DIR AND GNUSTEP-BASE_LIBRARY)
   SET(GNUSTEP-BASE_FOUND TRUE)
ENDIF (GNUSTEP-BASE_INCLUDE_DIR AND GNUSTEP-BASE_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    GNUSTEP-BASE
    REQUIRED_VARS GNUSTEP-BASE_LIBRARY GNUSTEP-BASE_INCLUDE_DIR
    FAIL_MESSAGE "Could not find the GNUStep base library"
    )

