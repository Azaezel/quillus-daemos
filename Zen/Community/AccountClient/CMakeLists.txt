cmake_minimum_required (VERSION 2.6) 

project (AccountClient)
set ( PROJECT_PREFIX "Community/AccountClient" )

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


	
set ( AccountClient_SRC
    Configuration.hpp
    src/Account.cpp
    src/Account.hpp
    src/AccountClientPlugin.cpp
    src/AccountController.cpp
    src/AccountController.hpp
    src/AccountModelProxy.cpp
    src/AccountModelProxy.hpp
    src/AccountService.cpp
    src/AccountService.hpp
    src/AccountServiceFactory.cpp
    src/AccountServiceFactory.hpp
    src/Group.cpp
    src/Group.hpp
    src/GroupController.cpp
    src/GroupController.hpp
    src/GroupModelProxy.cpp
    src/GroupModelProxy.hpp

    )

add_definitions( -DACCOUNTCLIENT_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( AccountClient ${LIB_TYPE} ${AccountClient_SRC} )

add_dependencies(AccountClient ZenCore ZenEngine ZenEnterprise ZenObjectModel AccountCommon AccountProtocol)

target_link_libraries(AccountClient ZenCore ZenEngine ZenEnterprise ZenObjectModel AccountCommon AccountProtocol ${Boost_LIBRARIES} )

set_target_properties ( AccountClient
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( AccountClient PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( AccountClient PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	AccountClient
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)