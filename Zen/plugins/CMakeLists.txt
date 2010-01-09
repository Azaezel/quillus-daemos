cmake_minimum_required (VERSION 2.6) 

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Use our provided modules for the CMake Find system first, if available

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of ZOSS plugins
#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# ZOgre, but online if Ogre is found

Find_Package(OGRE)

if (OGRE_FOUND)
	message("-- ZOgre")
	add_subdirectory("${ZEN_HOME}/plugins/ZOgre" ZOgre)
else(OGRE_FOUND)
	message("-- ZOgre will NOT be compiled.  OGRE was not found.")
endif(OGRE_FOUND)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# ZSQLite - Can always do this since the SQLite code is included

message("-- ZSQLite")
add_subdirectory("${ZEN_HOME}/plugins/ZSQLite" ZSQLite)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# ZBoostNetworking - Only depends on Boost, so always include it.

message("-- ZBoostNetworking")
add_subdirectory("${ZEN_HOME}/plugins/ZBoostNetworking" ZBoostNetworking)


#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# ZInput, but only if OIS is found

Find_Package(OIS)

if(OIS_FOUND)
    message("-- ZInput")
    add_subdirectory("${ZEN_HOME}/plugins/ZInput" ZInput)
else(OIS_FOUND)
    message("-- ZInput will NOT be compiled.  OIS was not found.")
endif(OIS_FOUND)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# ZLua, but only if Lua is found

Find_Package(Lua51)
set (LUA_FOUND ${LUA51_FOUND})

if (NOT LUA_FOUND)
	Find_Package(Lua50)
	set (LUA_FOUND ${LUA50_FOUND})
endif (NOT LUA_FOUND)

if (NOT LUA_FOUND)
	Find_Package(Lua)
endif (NOT LUA_FOUND)

if(LUA_FOUND)
    message("-- ZLua")
    add_subdirectory( ZLua )
else(NOT LUA_FOUND)
    message("-- ZLua will NOT be compiled.  Lua was not found.")
endif(LUA_FOUND)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# ZNewton, but online if Newton is found


#TODO Move this to FindNewton.cmake ?
find_path ( NEWTON_INCLUDE_DIR Newton.h HINTS ${INDIEZEN_PARENT}/newtonSDK/sdk /usr/include /usr/local/include )
find_library ( NEWTON_LIBRARIES Newton HINTS ${INDIEZEN_PARENT}/newtonSDK/sdk /usr/lib /usr/local/lib )

if (NEWTON_INCLUDE_DIR AND NEWTON_LIBRARIES)
	message("-- ZNewton")
	add_subdirectory( ZNewton )
else()
	message("-- ZNewton will NOT be compiled.  Newton was not found.")
endif()

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# ZCrazyEddie, but only if CEGUI is found
pkg_check_modules(CEGUI	CEGUI>=0.3.0)
pkg_check_modules(CEGUI_OGRE CEGUI-OGRE)
if (CEGUI_FOUND AND CEGUI_OGRE_FOUND)
	message("-- ZCrazyEddie")
	add_subdirectory( ZCrazyEddie )
else(NOT CEGUI_FOUND AND CEGUI_OGRE_FOUND)
	message("-- ZCrazyEddie will NOT be compiled.  CEGUI / CEGUI_OGRE was not found.")
endif (CEGUI_FOUND AND CEGUI_OGRE_FOUND)

message("-- ZTerrain")
add_subdirectory( ZTerrain )

message("-- ZSky")
add_subdirectory( ZSky )

# These have not yet been ported.
#  Please add them if you port them ;)
#add_subdirectory( BasicNetworking )
#add_subdirectory( Framework )
#add_subdirectory( GLRenderer )
#add_subdirectory( ZBasicCommandLineParser )
#add_subdirectory( ZBasicConsole )
#add_subdirectory( ZBasicGameRules )
#add_subdirectory( ZCam )
#add_subdirectory( ZCloud )
#add_subdirectory( ZCollada )
#add_subdirectory( ZDBDesigner )
#add_subdirectory( ZFMOD )
#add_subdirectory( ZMySQL )
#add_subdirectory( ZOpenAL )
#add_subdirectory( ZPhysX )
#add_subdirectory( ZPostgres )
#add_subdirectory( ZPython )
#add_subdirectory( ZRakNet )
