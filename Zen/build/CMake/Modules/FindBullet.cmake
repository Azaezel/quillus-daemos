# Find the Open Dynamics Engine headers and library
#
#  OPENDE_INCLUDE_DIR - location of Ogre.h
#  OPENDE_LIBRARIES   - List of libraries to link to when using Ogre
#  OPENDE_FOUND       - True if Ogre was found.
# OPENDE_DEFINITIONS - OpenDE definitions

include(FindPkgMacros)
include(PreprocessorUtils)

IF (BULLET_INCLUDE_DIR)
  	SET(BULLET_FIND_QUIETLY TRUE)
ENDIF (BULLET_INCLUDE_DIR)

#try using PkgConfig first
use_pkgconfig(BULLET bullet)

# If that didn't work, do it manually
if (NOT BULLET_FOUND)
	FIND_PATH(BULLET_INCLUDE_DIR ode
		HINTS 
		${INDIEZEN_PARENT}/bullet/src
		${DEPENDENCIES_HOME}/bullet/src
		/usr/include
	)

	FIND_LIBRARY(BULLET_RELEASE_LIBRARY bullet
		HINTS
		${DEV_LIB}
		${INDIEZEN_PARENT}/bullet/lib
		${DEPENDENCIES_HOME}/bullet/lib
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



