cmake_minimum_required (VERSION 2.6) 

project (SessionServer)
set ( PROJECT_PREFIX "Community/SessionServer" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of Zen Engine

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( SessionServer_SRC
    src/Attribute.cpp
    src/Attribute.hpp
    src/SessionServiceFactory.cpp
    src/SessionServerPlugin.cpp
    src/SessionService.cpp
    src/SessionService.hpp
    src/SessionServiceFactory.hpp
    src/AccountView.hpp
    src/Session.cpp
    src/Session.hpp
    src/AccountView.cpp
    Configuration.hpp

    )

add_definitions( -DSESSIONSERVER_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( SessionServer ${LIB_TYPE} ${SessionServer_SRC} )

add_dependencies(SessionServer ZenCore ZenEngine ZenEnterprise AccountCommon SessionCommon SessionProtocol SessionModel )

target_link_libraries(SessionServer ZenCore ZenEngine ZenEnterprise AccountCommon SessionCommon SessionProtocol SessionModel ${Boost_LIBRARIES} )

set_target_properties ( SessionServer
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( SessionServer PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( SessionServer PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    plugin.xml
    Configuration.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	SessionServer
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)