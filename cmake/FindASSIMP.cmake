
# ASSIMP_FOUND
# ASSIMP_INCLUDE_PATH
# ASSIMP_LIBRARY

IF(X64 AND NOT APPLE)

    FIND_PATH(ASSIMP_INCLUDE_PATH assimp/Importer.hpp
        ${PROJECT_SOURCE_DIR}/3rdparty/assimp-3.0-win64/include
        # $ENV{TMR_HOME}/../3rdparty/glew-1.9.0-win64/include
        $ENV{ASSIMP_DIR}/include
        $ENV{ASSIMP_HOME}/include
        # $ENV{PROGRAMFILES}/GLEW/include
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        DOC "The directory where assimp/Importer.hpp etc. resides")

    FIND_LIBRARY(ASSIMP_LIBRARY
        NAMES assimp
        PATHS
        ${PROJECT_SOURCE_DIR}/3rdparty/assimp-3.0-win64/lib
        # $ENV{TMR_HOME}/../3rdparty/glew-1.9.0-win64/lib
        $ENV{ASSIMP_DIR}/lib
        $ENV{ASSIMP_HOME}/lib
        /usr/lib64
        /usr/local/lib64
        /sw/lib64
        /opt/local/lib64
        DOC "The Assimp library")

ELSE()
    
    FIND_PATH( ASSIMP_INCLUDE_PATH assimp/Importer.hpp
        ${PROJECT_SOURCE_DIR}/3rdparty/assimp-3.0/include
        # $ENV{TMR_HOME}/../3rdparty/glew-1.7.0/include
        $ENV{ASSIMP_DIR}/include
        $ENV{ASSIMP_HOME}/include
        # $ENV{PROGRAMFILES}/GLEW/include
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        DOC "The directory where assimp/Importer.hpp etc. resides")

    FIND_LIBRARY(ASSIMP_LIBRARY
        NAMES assimp
        PATHS
        ${PROJECT_SOURCE_DIR}/3rdparty/assimp-3.0/lib
        # $ENV{TMR_HOME}/../3rdparty/glew-1.7.0/lib
        $ENV{ASSIMP_DIR}/lib
        $ENV{ASSIMP_HOME}/lib
        /usr/lib
        /usr/local/lib
        /sw/lib
        /opt/local/lib
        DOC "The Assimp library")

ENDIF()
    
IF(ASSIMP_INCLUDE_PATH AND ASSIMP_LIBRARY)
    SET(ASSIMP_FOUND 1 CACHE STRING "Set to 1 if Assimp is found, 0 otherwise")
ELSE()
    SET(ASSIMP_FOUND 0 CACHE STRING "Set to 1 if Assimp is found, 0 otherwise")
    MESSAGE(WARNING "Note: an envvar like ASSIMP_HOME assists this script to locate Assimp.")
ENDIF()

MARK_AS_ADVANCED( ASSIMP_FOUND )
