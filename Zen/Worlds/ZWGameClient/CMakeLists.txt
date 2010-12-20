cmake_minimum_required (VERSION 2.6) 

project(ZWGameClient)
set ( PROJECT_PREFIX "Worlds/ZWGameClient" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED COMPONENTS system filesystem )
Find_Package(LibXml2 REQUIRED)

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of Zen Worlds Game Client

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( ZWGameClient_SRC
    src/GameClient.cpp
    src/GameClient.hpp
    src/GameClientFactory.cpp
    src/GameClientFactory.hpp
    src/GameClient_script.cpp
    src/GameObject.cpp
    src/GameObject.hpp
    src/GUIManager.cpp
    src/GUIManager.hpp
    src/ZWGameClientPlugin.cpp

    )

add_definitions( -DZWGAMECLIENT_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} ${LibXML2_LIBRARY_DIRS} )

add_library ( ZWGameClient ${LIB_TYPE} ${ZWGameClient_SRC} )

add_dependencies( ZWGameClient ZenCore ZenEngine ZenEnterprise BaseClient BaseCommon ZCrazyEddie )

target_link_libraries( ZWGameClient ZenCore ZenEngine ZenEnterprise BaseClient BaseCommon ZCrazyEddie ${Boost_LIBRARIES} ${LIBXML2_LIBRARIES} )

set_target_properties ( ZWGameClient 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZWGameClient PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZWGameClient PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZWGameClient
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
