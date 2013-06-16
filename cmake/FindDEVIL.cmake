
# GLEW_FOUND
# GLEW_INCLUDE_PATH
# GLEW_LIBRARY

FIND_PATH(DEVIL_INCLUDE_PATH
    /usr/include IL/il.h
    DOC "The directory where IL/il.h resides"
)

FIND_LIBRARY(DEVIL_LIBRARY
    NAMES IL IL
    PATHS
        /usr/lib
    DOC "The DevIL library"
)
    
IF(DEVIL_INCLUDE_PATH AND DEVIL_LIBRARY)
    SET(DEVIL_FOUND 1 CACHE STRING "Set to 1 if DEVIL is found, 0 otherwise")
ELSE()
    SET(DEVIL_FOUND 0 CACHE STRING "Set to 1 if DEVIL is found, 0 otherwise")
ENDIF()

MARK_AS_ADVANCED( DEVIL_FOUND )
