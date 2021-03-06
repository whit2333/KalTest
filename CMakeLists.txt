########################################################
# cmake file for building KalFit libraries
# @author Jan Engels, DESY
# @author F. Gaede, DESY
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 FATAL_ERROR )
########################################################


# project name
PROJECT( KalTest )


# project version
SET( KalTest_VERSION_MAJOR 2 )
SET( KalTest_VERSION_MINOR 2 )
SET( KalTest_VERSION_PATCH 1 )

cmake_policy(SET CMP0008 NEW)  

### DEPENDENCIES ############################################################

FIND_PACKAGE( ILCUTIL COMPONENTS ILCSOFT_CMAKE_MODULES REQUIRED )
FIND_PACKAGE( ROOT REQUIRED COMPONENTS Eve)
INCLUDE(${ROOT_USE_FILE})

LINK_LIBRARIES( ${ROOT_COMPONENT_LIBRARIES} )
INCLUDE(/opt/software/local/cmakemodules/MacroRootDict.cmake)

# load default settings from ILCSOFT_CMAKE_MODULES
INCLUDE( ilcsoft_default_settings )


# macros for generating root dict sources with rootcint
SET( ROOT_DICT_CINT_DEFINITIONS "-DHANDLE_DICT_EXCEPTIONS=IGNORED_FOR_CINT" )
#INCLUDE( ${ROOT_DICT_MACROS_FILE} )
INCLUDE(cmake/root.cmake)

INCLUDE( MacroRootDict )


# require proper c++
#ADD_DEFINITIONS( "-Wall -ansi -pedantic" )
#----- need long long for int64 for now ------
#ADD_DEFINITIONS( "-Wno-long-long -fno-strict-aliasing" )

### LIBRARY AND TOOLS #######################################################


#---- use 5 dimensional track state:
OPTION( BUILD_WITH_T0_FIT "Set to ON to build with t0 fit (kSdim=6)" OFF )
IF( NOT BUILD_WITH_T0_FIT)
 ADD_DEFINITIONS( -D __NOT0__ )
ENDIF()

ADD_SUBDIRECTORY( ./src )


# --------- kaltest examples -------------------------------------------------
# if this option is set to ON examples will be built with default target 'make'
# if set to OFF examples will only be built with target 'make examples'
OPTION( BUILD_EXAMPLES "Set to ON to build examples" OFF )
MESSAGE( STATUS "BUILD_EXAMPLES = ${BUILD_EXAMPLES}" )
ADD_SUBDIRECTORY( ./examples/kaltest )
# ----------------------------------------------------------------------------



# display some variables and write them to cache
DISPLAY_STD_VARIABLES()

# generate and install following configuration files
GENERATE_PACKAGE_CONFIGURATION_FILES( KalTestConfig.cmake KalTestConfigVersion.cmake KalTestLibDeps.cmake )

