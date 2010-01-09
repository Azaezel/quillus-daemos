# Find the Lua headers and library
#
#  LUA_INCLUDE_DIR - location of lua.h
#  LUA_LIBRARIES   - List of libraries to link to when using Lua
#  LUA_FOUND       - True if Lua was found.
# LUA_CLIBS_DIR  - clibs directory on Windows
IF (LUA_INCLUDE_DIR)
  	SET(LUA_FIND_QUIETLY TRUE)
ENDIF (LUA_INCLUDE_DIR)

FIND_PATH(LUA_INCLUDE_DIR lua.h
	HINTS 
	${DEPENDENCIES_HOME}/lua/include
	/usr/include/lua
)

FIND_LIBRARY(LUA_LIBRARY lua5.1 
	HINTS
	${DEV_LIB}
	/usr/lib
)

# handle the QUIETLY and REQUIRED arguments and set LUA_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LUA DEFAULT_MSG LUA_LIBRARY LUA_INCLUDE_DIR)

IF(LUA_FOUND)
  SET( LUA_LIBRARIES ${LUA_LIBRARY} )
ELSE(LUA_FOUND)
  SET( LUA_LIBRARIES )
ENDIF(LUA_FOUND)

MARK_AS_ADVANCED( LUA_LIBRARY LUA_INCLUDE_DIR )
