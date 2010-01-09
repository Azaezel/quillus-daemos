# Find the Open Dynamics Engine headers and library
#
#  OPENDE_INCLUDE_DIR - location of Ogre.h
#  OPENDE_LIBRARIES   - List of libraries to link to when using Ogre
#  OPENDE_FOUND       - True if Ogre was found.
# OPENDE_DEFINITIONS - OpenDE definitions

include(FindPkgMacros)
include(PreprocessorUtils)

IF (OPENDE_INCLUDE_DIR)
  	SET(OPENDE_FIND_QUIETLY TRUE)
ENDIF (OPENDE_INCLUDE_DIR)

#try using PkgConfig first
use_pkgconfig(OPENDE ode)

# If that didn't work, do it manually
if (NOT OPENDE_FOUND)
	FIND_PATH(OPENDE_INCLUDE_DIR ode
		HINTS 
		${INDIEZEN_PARENT}/opende/include
		${DEPENDENCIES_HOME}/opende/include
		/usr/include
	)

	FIND_LIBRARY(OPENDE_RELEASE_LIBRARY ode_single
		HINTS
		${DEV_LIB}
		${INDIEZEN_PARENT}/opende/lib/releasesingledll
		${DEPENDENCIES_HOME}/opende/lib/releasesingledll
		/usr/lib
	)

	FIND_LIBRARY(OPENDE_DEBUG_LIBRARY ode_singled
		HINTS
		${DEV_LIB}
		${INDIEZEN_PARENT}/opende/lib/debugsingledll
		${DEPENDENCIES_HOME}/opende/lib/debugsingledll
		/usr/lib
	)

	if (OPENDE_INCLUDE_DIR AND OPENDE_RELEASE_LIBRARY)
		set(OPENDE_FOUND "True")
		set(OPENDE_LIBRARIES debug ${OPENDE_DEBUG_LIBRARY} optimized ${OPENDE_RELEASE_LIBRARY} )
		set(OPENDE_DEFINITIONS "-DdSINGLE")
	else(OPENDE_INCLUDE_DIR AND OPENDE_RELEASE_LIBRARY)
		set(OPENDE_LIBRARIES "OPENDE-NOTFOUND")
		set(OPENDE_FOUND "False")
	endif (OPENDE_INCLUDE_DIR AND OPENDE_RELEASE_LIBRARY)

endif(NOT OPENDE_FOUND)



