# Find the FMOD headers and library
#
#  FMOD_INCLUDE_DIR     - location of fmod.h
#  FMOD_LIBRARIES       - List of libraries to link to when using FMOD
#  FMOD_DEBUG_LIBRARY   - List of debug libraries to link to when using FMOD
#  FMOD_FOUND           - True if FMOD was found.

IF (FMOD_INCLUDE_DIR)
  	SET(FMOD_FIND_QUIETLY TRUE)
ENDIF (FMOD_INCLUDE_DIR)

FIND_PATH(FMOD_INCLUDE_DIR fmod.h
   HINTS
   ${INDIEZEN_PARENT}/FMOD/FMOD_Ex_4_16_09/api/inc
   ${INDIEZEN_PARENT}/FMOD/api/inc
   ${DEPENDENCIES_HOME}/FMOD/FMOD_Ex_4_16_09/api/inc
   ${DEPENDENCIES_HOME}/FMOD/api/inc
   /usr/include/fmod
)

if (${MINGW} OR ${CYGWIN})
   #message("creating configuration for MinGW/Cygwin")
   FIND_LIBRARY(FMOD_LIBRARY libfmodex
      HINTS
      ${INDIEZEN_PARENT}/FMOD/FMOD_Ex_4_16_09/api/lib
      ${INDIEZEN_PARENT}/FMOD/api/lib
      ${DEPENDENCIES_HOME}/FMOD/FMOD_Ex_4_16_09/api/lib
      ${DEPENDENCIES_HOME}/FMOD/api/lib
      /usr/lib
   )
   FIND_LIBRARY(FMOD_DEBUG_LIBRARY libfmodexL
      HINTS
      ${INDIEZEN_PARENT}/FMOD/FMOD_Ex_4_16_09/api/lib
      ${INDIEZEN_PARENT}/FMOD/api/lib
      ${DEPENDENCIES_HOME}/FMOD/FMOD_Ex_4_16_09/api/lib
      ${DEPENDENCIES_HOME}/FMOD/api/lib
      /usr/lib
   )
ENDIF ()

if (${BORLAND})
   #message("creating configuration for Borland")
   FIND_LIBRARY(FMOD_LIBRARY fmodex_bc
      HINTS
      ${INDIEZEN_PARENT}/FMOD/FMOD_Ex_4_16_09/api/lib
      ${INDIEZEN_PARENT}/FMOD/api/lib
      ${DEPENDENCIES_HOME}/FMOD/FMOD_Ex_4_16_09/api/lib
      ${DEPENDENCIES_HOME}/FMOD/api/lib
      /usr/lib
   )
   FIND_LIBRARY(FMOD_DEBUG_LIBRARY fmodexL_bc
      HINTS
      ${INDIEZEN_PARENT}/FMOD/FMOD_Ex_4_16_09/api/lib
      ${INDIEZEN_PARENT}/FMOD/api/lib
      ${DEPENDENCIES_HOME}/FMOD/FMOD_Ex_4_16_09/api/lib
      ${DEPENDENCIES_HOME}/FMOD/api/lib
      /usr/lib
   )
ENDIF ()

IF(CMAKE_GENERATOR MATCHES "Visual Studio")
   #message("creating configuration for MSVC")
   FIND_LIBRARY(FMOD_LIBRARY fmodex_vc
      HINTS
      ${INDIEZEN_PARENT}/FMOD/FMOD_Ex_4_16_09/api/lib
      ${INDIEZEN_PARENT}/FMOD/api/lib
      ${DEPENDENCIES_HOME}/FMOD/FMOD_Ex_4_16_09/api/lib
      ${DEPENDENCIES_HOME}/FMOD/api/lib
      /usr/lib
   )
   FIND_LIBRARY(FMOD_DEBUG_LIBRARY fmodexL_vc
      HINTS
      ${INDIEZEN_PARENT}/FMOD/FMOD_Ex_4_16_09/api/lib
      ${INDIEZEN_PARENT}/FMOD/api/lib
      ${DEPENDENCIES_HOME}/FMOD/FMOD_Ex_4_16_09/api/lib
      ${DEPENDENCIES_HOME}/FMOD/api/lib
      /usr/lib
   )
ENDIF ()

SET(FMOD_FOUND "NO")
IF (FMOD_LIBRARY AND FMOD_DEBUG_LIBRARY AND FMOD_INCLUDE_DIR)
   SET(FMOD_FOUND "YES")
ENDIF(FMOD_LIBRARY AND FMOD_DEBUG_LIBRARY AND FMOD_INCLUDE_DIR)

IF(FMOD_FOUND)
  SET( FMOD_LIBRARIES ${FMOD_LIBRARY} )
ELSE(FMOD_FOUND)
  SET( FMOD_LIBRARIES )
ENDIF(FMOD_FOUND)

# handle the QUIETLY and REQUIRED arguments and set FMOD_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FMOD DEFAULT_MSG FMOD_LIBRARY FMOD_INCLUDE_DIR)

