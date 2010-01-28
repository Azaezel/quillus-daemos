cmake_minimum_required (VERSION 2.6) 

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Use our provided modules for the CMake Find system first, if available

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

set ( ZEN_WORLDS_PROJECTS 
    WorldClient
    WorldCommon
#    WorldProtocol
#    WorldServer
    
#    ZoneClient
    ZoneCommon
#    ZoneProtocol
    ZoneServer
    
#    ZoneDataClient
#    ZoneDataCommon
#    ZoneDataProtocol
#    ZoneDataServer

	ZWGameClient
)

foreach(PROJECT_PATH ${ZEN_WORLDS_PROJECTS})
    message("-- ${PROJECT_PATH}") 
    add_subdirectory("${ZEN_HOME}/Worlds/${PROJECT_PATH}" "${PROJECT_PATH}")
endforeach(PROJECT_PATH)
