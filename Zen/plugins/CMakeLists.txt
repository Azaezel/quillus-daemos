cmake_minimum_required (VERSION 2.6) 

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Use our provided modules for the CMake Find system first, if available

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-

message("-- Framework")
add_subdirectory("${ZEN_HOME}/plugins/Framework" Framework)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of ZOSS plugins
#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# ZOgre, but online if Ogre is found

Find_Package(OGRE)

if (OGRE_FOUND)
	message("-- ZOgre")
	add_subdirectory("${ZEN_HOME}/plugins/ZOgre" ZOgre)
	message("-- ZOgreTerrain")
	add_subdirectory("${ZEN_HOME}/plugins/ZOgreTerrain" ZOgreTerrain)
else(OGRE_FOUND)
	message("-- ZOgre and ZOgreTerrain will NOT be compiled.  OGRE was not found.")
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

# Get the clibs directory to add to the .vcproj.user path
IF (MSVC_IDE)
    FIND_PATH(LUA_ROOT_DIR clibs
        HINTS
        ${DEPENDENCIES_HOME}/lua
        ${INDIEZEN_PARENT}/lua
    )
    IF (LUA_ROOT_DIR)
        set(LUA_CLIBS_DIR ${LUA_ROOT_DIR}/clibs CACHE FILEPATH "Lua clibs directory" )
        message("Lua cpath: ${LUA_CLIBS_DIR}")
    ENDIF (LUA_ROOT_DIR)
ENDIF(MSVC_IDE)

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
find_library ( NEWTON_LIBRARIES Newton HINTS ${INDIEZEN_PARENT}/newtonSDK/sdk/dll /usr/lib /usr/local/lib )

if (NEWTON_INCLUDE_DIR AND NEWTON_LIBRARIES)
	message("-- ZNewton")
	add_subdirectory( ZNewton )
else()
	message("-- ZNewton will NOT be compiled.  Newton was not found.")
endif()

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# ZCrazyEddie, but only if CEGUI is found
if (MSVC_IDE)
    # Hack for now -- going to go ahead and add ZCrazyEddie by force
    # and worry about finding CEGUI later.
    find_path( CEGUI_INCLUDE_DIR CEGUI/CEGUI.h 
        HINTS 
        ${INDIEZEN_PARENT}/cegui/cegui/include
    )
    find_path( CEGUI_LIBRARYDIR CEGUI/lib 
        HINTS
        ${INDIEZEN_PARENT}
    )
    set( CEGUI_INCLUDEDIR "${CEGUI_INCLUDE_DIR}" )
    set( CEGUI_LIBRARIES "debug;${CEGUI_LIBRARYDIR}/CEGUI/lib/CEGUIBase_d.lib;optimized;${CEGUI_LIBRARYDIR}/CEGUI/lib/CEGUIBase.lib;debug;${CEGUI_LIBRARYDIR}/CEGUI/lib/CEGUIOgreRenderer_d.lib;optimized;${CEGUI_LIBRARYDIR}/CEGUI/lib/CEGUIOgreRenderer.lib;debug;${CEGUI_LIBRARYDIR}/CEGUI/lib/CEGUILuaScriptModule_d.lib;optimized;${CEGUI_LIBRARYDIR}/CEGUI/lib/CEGUILuaScriptModule.lib" )
    if (CEGUI_INCLUDE_DIR AND CEGUI_LIBRARYDIR)
        message("-- ZCrazyEddie")
        add_subdirectory( ZCrazyEddie )
    else (CEGUI_INCLUDE_DIR AND CEGUI_LIBRARYDIR)
        message("-- ZCrazyEddie will NOT be compiled.  CEGUI / CEGUI_OGRE was not found.")
    endif (CEGUI_INCLUDE_DIR AND CEGUI_LIBRARYDIR)
else (NOT MSVC_IDE)
    pkg_check_modules(CEGUI	CEGUI>=0.3.0)
    pkg_check_modules(CEGUI_OGRE CEGUI-OGRE)
    if (CEGUI_FOUND AND CEGUI_OGRE_FOUND)
        message("-- ZCrazyEddie")
        add_subdirectory( ZCrazyEddie )
    else(NOT CEGUI_FOUND AND CEGUI_OGRE_FOUND)
        message("-- ZCrazyEddie will NOT be compiled.  CEGUI / CEGUI_OGRE was not found.")
    endif (CEGUI_FOUND AND CEGUI_OGRE_FOUND)
endif (MSVC_IDE)

message("-- ZTerrain")
add_subdirectory( ZTerrain )

message("-- ZSky")
add_subdirectory( ZSky )


#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# ZODE but only if Open Dynamics Engine is found
Find_Package(OpenDE)

if (OPENDE_FOUND)
    message("-- ZODE (${OPENDE_FOUND}, ${OPENDE_LIBRARIES})")
    add_subdirectory(ZODE)
else(OPENDE_FOUND)
    message("-- ZODE will NOT be compiled.  ODE was not found.")
endif(OPENDE_FOUND)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# ZBULLET but only if BULLET is found
Find_Package(BULLET)

if (BULLET_FOUND)
    message("-- ZBULLET (${BULLET_FOUND}, ${BULLET_LIBRARIES})")
    add_subdirectory(ZBULLET)
else(BULLET_FOUND)
    message("-- ZBULLET will NOT be compiled.  BULLET was not found.")
endif(BULLET_FOUND)


#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
message("-- ZMicroPather")
add_subdirectory(ZMicroPather)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# ZPython, but only if Python is found

Find_Package(Python)

if (PYTHON_FOUND)
	message("-- ZPython ")
	add_subdirectory(ZPython)
else(PYTHON_FOUND)
	message("ZPython will NOT be compiled. Python was not found.")
endif(PYTHON_FOUND)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-

Find_Package(OpenAL)
Find_Package(Alut)
Find_Package(Ogg)
Find_Package(Vorbis)

if (OPENAL_FOUND AND ALUT_FOUND AND OGG_FOUND AND VORBIS_FOUND)
    message("-- ZOpenAL")
    add_subdirectory(ZOpenAL)
else(OPENAL_FOUND AND ALUT_FOUND AND OGG_FOUND AND VORBIS_FOUND)
    message("ZOpenAL will NOT be compiled. Dependencies not found.")
endif(OPENAL_FOUND AND ALUT_FOUND AND OGG_FOUND AND VORBIS_FOUND)

#add_subdirectory( ZOpenAL )

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
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
#add_subdirectory( ZPhysX )
#add_subdirectory( ZPostgres )
#add_subdirectory( ZRakNet )
