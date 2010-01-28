# Find the Python headers and library
#
#  PYTHON_INCLUDE_DIR - location of Python.h
#  PYTHON_LIBRARIES   - List of libraries to link to when using Python
#  PYTHON_FOUND       - True if Python was found.
#  PYTHON_LIBS_DIR    - libs directory on Windows
IF (PYTHON_INCLUDE_DIR)
  	SET(PYTHON_FIND_QUIETLY TRUE)
ENDIF (PYTHON_INCLUDE_DIR)

FIND_PATH(PYTHON_INCLUDE_DIR python.h
	HINTS 
	${DEPENDENCIES_HOME}/python/include
	/usr/include/python
)

FIND_LIBRARY(PYTHON_LIBRARY python26 
	HINTS
	${DEV_LIB}
	/usr/lib
)

# handle the QUIETLY and REQUIRED arguments and set PYTHON_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PYTHON DEFAULT_MSG PYTHON_LIBRARY PYTHON_INCLUDE_DIR)

IF(PYTHON_FOUND)
  SET( PYTHON_LIBRARIES ${PYTHON_LIBRARY} )
ELSE(PYTHON_FOUND)
  SET( PYTHON_LIBRARIES )
ENDIF(PYTHON_FOUND)

MARK_AS_ADVANCED( PYTHON_LIBRARY PYTHON_INCLUDE_DIR )
