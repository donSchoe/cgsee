
cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)


# PROJECT CONFIG

set(META_PROJECT_NAME "CGSee")

set(META_VERSION_MAJOR "0")
set(META_VERSION_MINOR "0")

set(META_AUTHOR_ORGANIZATION "CGSee Group")
set(META_AUTHOR_DOMAIN "https://github.com/cgcostume/cgsee/")


option(OPTION_LIMIT_CONFIGS "Generate limited configs (Release; Debug)" ON)
option(OPTION_LOCAL_INSTALL "Install to local directory instead of system" OFF)


if(OPTION_LIMIT_CONFIGS)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited Configs" FORCE)
endif()

set(PROJECT_NAME ${META_PROJECT_NAME})  
project(${PROJECT_NAME} C CXX)

# check if x64 and provide this information

set(X64 OFF)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(X64 ON)
endif()

# check for linux

if(UNIX AND NOT APPLE)
    set(LINUX 1)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# root the bin and lib dirs to the project dir

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/install)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})


set(CMAKE_MODULE_PATH 
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


# RETRIEVE REPOSITORY VERSION AND GENERATE META HEADER

# add a revision file containing the git-head tag for cpack and install

include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

# generate a shorter, googlelike variation for rev

string(SUBSTRING ${GIT_SHA1} 0 12 GIT_REV)
file(WRITE ${CMAKE_BINARY_DIR}/revision ${GIT_REV})
install(FILES ${CMAKE_BINARY_DIR}/revision DESTINATION .)


# THIRD PARTIES

find_package(OpenGL REQUIRED)
find_package(DEVIL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLEW REQUIRED)
find_package(ASSIMP REQUIRED)

set(CMAKE_AUTOMOC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)

# Probably works in the next cmake release -> http://www.cmake.org/Bug/view.php?id=13788
# What we do not want is automocs beside the project -> http://www.cmake.org/Bug/view.php?id=13688

set_property(GLOBAL PROPERTY AUTOMOC_FOLDER CMakeAutomocTargets)

# try to find qt5
# good resource: http://www.kdab.com/using-cmake-with-qt-5/

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED) # this is important in order to get uic workin


# SOURCES AND INCLUDES

include_directories(
    ${CMAKE_BINARY_DIR} # for Meta.h
    ${OPENGL_INCLUDE_DIR}
    ${DEVIL_INCLUDE_DIR}
    ${GLM_INCLUDE_PATH}
    ${GLEW_INCLUDE_PATH}
    ${ASSIMP_INCLUDE_PATH}
    ${CMAKE_SOURCE_DIR}/libs)

set(CGSEE_LIB_FOLDER "CGSee Libs")
set(CGSEE_APP_FOLDER "CGsee Apps")

set(CGSEE_HEADER_GROUP "Header Files (API)")
set(CGSEE_SOURCE_GROUP "Source Files")
set(CGSEE_UIWRAP_GROUP "Ui-Wrap Files")


# setup platform specifics (compile flags, etc., ...)

if(MSVC)
    message(STATUS "Configuring for platform Windows/MSVC.")
    include(${CMAKE_SOURCE_DIR}/cmake/PlatformWindowsMSVC.cmake)
elseif(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Configuring for platform Windows/GCC.")
    include(${CMAKE_SOURCE_DIR}/cmake/PlatformWindowsGCC.cmake)
elseif(LINUX AND CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Configuring for platform Linux/GCC.")
    include(${CMAKE_SOURCE_DIR}/cmake/PlatformLinuxGCC.cmake)
elseif(APPLE)
    message(STATUS "Configuring for platform MacOS.")
    include(${CMAKE_SOURCE_DIR}/cmake/PlatformMacOS.cmake)
else()
    # TODO configure for apple        
    message(WARNING "Unsupported platform/compiler combination")
endif()


include(cmake/Custom.cmake)

add_subdirectory("libs")
add_subdirectory("apps")


# CPACK DEPLOY

set(CPACK_GENERATOR "ZIP")
set(CPACK_SOURCE_GENERATOR "ZIP")

set(CPACK_PACKAGE_VERSION_MAJOR ${META_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${META_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${GIT_REV})

set(CPACK_INSTALL_DIRECTORIES ${CMAKE_BINARY_DIR}/install)
set(CPACK_IGNORE_FILES "/.pdb/;/.ilk/;/.svn/;/.hg/;/.git/;.swp$;.#;/#")
set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_BINARY_DIR}/install)

include(CPack)
