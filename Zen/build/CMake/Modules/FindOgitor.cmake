# Find the Open Dynamics Engine headers and library
#
#  OGITOR_INCLUDE_DIR - location of Ogre.h
#  OGITOR_LIBRARIES   - List of libraries to link to when using Ogre
#  OGITOR_FOUND       - True if Ogre was found.
# OGITOR_DEFINITIONS - OpenDE definitions

include(FindPkgMacros)
include(PreprocessorUtils)

IF (OGITOR_INCLUDE_DIR)
  	SET(OGITOR_FIND_QUIETLY TRUE)
ENDIF (OGITOR_INCLUDE_DIR)

#try using PkgConfig first
use_pkgconfig(OGITOR Ogitor)

# If that didn't work, do it manually
if (NOT OGITOR_FOUND)
	FIND_PATH(OGITOR_INCLUDE_DIR Ogitors.h
		HINTS 
		${INDIEZEN_PARENT}/ogitor/Ogitor/include
		${DEPENDENCIES_HOME}/ogitor/Ogitor/include
		/usr/include
	)

	FIND_LIBRARY(OGITOR_RELEASE_LIBRARY Ogitor
		HINTS
		${DEV_LIB}
		${INDIEZEN_PARENT}/ogitor-build/Ogitor/lib/release
		${INDIEZEN_PARENT}/ogitor-build/Ogitor/lib
		/usr/lib
	)

	FIND_LIBRARY(OGITOR_DEBUG_LIBRARY Ogitor_d
		HINTS
		${DEV_LIB}
		${INDIEZEN_PARENT}/ogitor-build/Ogitor/lib/debug
		/usr/lib
	)

	if (OGITOR_INCLUDE_DIR AND OGITOR_RELEASE_LIBRARY)
		set(OGITOR_FOUND "True")
        if (MSVC_IDE)
            FIND_PATH(OGITOR_RUNPATH_BIN Ogitor_d.dll
                HINTS
                ${INDIEZEN_PARENT}/ogitor-build/RunPath/bin
            )
        endif (MSVC_IDE)
		if (OGITOR_DEBUG_LIBRARY)
			set(OGITOR_LIBRARIES debug ${OGITOR_DEBUG_LIBRARY} optimized ${OGITOR_RELEASE_LIBRARY} )
		else (OGITOR_DEBUG_LIBRARY)
			set(OGITOR_LIBRARIES ${OGITOR_RELEASE_LIBRARY} )
		endif (OGITOR_DEBUG_LIBRARY)
	else(OGITOR_INCLUDE_DIR AND OGITOR_RELEASE_LIBRARY)
		set(OGITOR_LIBRARIES "OGITOR-NOTFOUND")
		set(OGITOR_FOUND "False")
	endif (OGITOR_INCLUDE_DIR AND OGITOR_RELEASE_LIBRARY)

endif(NOT OGITOR_FOUND)



