cmake_minimum_required (VERSION 2.6) 

project (SessionClient)
set ( PROJECT_PREFIX "Community/SessionClient" )

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


	
set ( SessionClient_SRC
    Configuration.hpp
    src/Attribute.cpp
    src/Attribute.hpp
    src/Session.cpp
    src/Session.hpp
    src/SessionClientPlugin.cpp
    src/SessionService.cpp
    src/SessionService.hpp
    src/SessionServiceFactory.cpp
    src/SessionServiceFactory.hpp

    )

add_definitions( -DSESSIONCLIENT_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( SessionClient ${LIB_TYPE} ${SessionClient_SRC} )

add_dependencies(SessionClient ZenCore ZenEngine ZenEnterprise SessionCommon SessionProtocol)

target_link_libraries(SessionClient ZenCore ZenEngine ZenEnterprise SessionCommon SessionProtocol ${Boost_LIBRARIES} )

set_target_properties ( SessionClient
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( SessionClient PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( SessionClient PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	SessionClient
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)