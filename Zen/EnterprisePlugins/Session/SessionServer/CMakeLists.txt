cmake_minimum_required (VERSION 2.6)

project (SessionServer)
set (PROJECT_PREFIX "EnterprisePlugins/Session/SessionServer")

if (NOT INDIEZEN_PARENT)
    find_path (INDIEZEN_PARENT Zen .. ../.. ../../.. ../../../.. ENV)
endif (NOT INDIEZEN_PARENT)

set (CMAKE_MODULE_PATH
    "{INDIEZEN_PARENT}/Zen/build/CMake/Modules"
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_MODULE_PATH}
    )
    
Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED)

SET (LIB_TYPE SHARED)

set (SessionServer_SRC
        Configuration.hpp
    I_Session.hpp
    I_SessionService.hpp
    Protocol/I_SessionMessage.hpp
    Protocol/I_SessionProtocolManager.hpp
    Protocol/I_SubscribeSessionsRequest.hpp
    Protocol/I_SubscribeSessionsResponse.hpp
    src/I_Session.cpp
    src/I_SessionService.cpp
    src/Protocol/I_SessionMessage.cpp
    src/Protocol/I_SessionProtocolManager.cpp
    src/Protocol/I_SubscribeSessionRequest.cpp
    src/Protocol/I_SubscribeSessionResponse.cpp
    src/Protocol/SessionMessage.cpp
    src/Protocol/SessionMessage.hpp
    src/Protocol/SessionProtocolManager.cpp
    src/Protocol/SessionProtocolManager.hpp
    src/Protocol/SubscribeSessionRequest.cpp
    src/Protocol/SubscribeSessionRequest.hpp
    src/Protocol/SubscribeSessionResponse.cpp
    src/Protocol/SubscribeSessionResponse.hpp
    src/Session.cpp
    src/Session.hpp
    src/SessionService.cpp
    src/SessionService.hpp
    src/SessionServiceFactory.cpp
    src/SessionServiceFactory.hpp
    src/SessionServicePlugin.cpp

    )
    
add_definitions (-DSESSION_SERVER_EXPORTS ${Zen_DEFINITIONS})

include_directories (${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT})

link_directories (${DEV_LIB} ${Boost_LIBRARY_DIRS})

add_library (SessionServer ${LIB_TYPE} ${SessionServer_SRC})

add_dependencies (SessionServer ZenCore ZenEnterprise SessionClient)

target_link_libraries (SessionServer ZenCore ZenEnterprise SessionClient ${Boost_LIBRARIES})

set_target_properties (SessionServer
    PROPERTIES
    DEBUG_POSTFIX "_d"
)

if (MSVC_IDE)
    set_target_properties (SessionServer PROPERTIES PREFIX "../../bin/")
    set_target_properties (SessionServer PROPERTIES IMPORT_PREFIX "../../lib/")
endif (MSVC_IDE)


install ( FILES 
    Protocol/I_SessionMessage.hpp
    Protocol/I_SessionProtocolManager.hpp
    Protocol/I_SubscribeSessionsRequest.hpp
    Protocol/I_SubscribeSessionsResponse.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Protocol/
)

install ( FILES 
    Configuration.hpp
    I_Session.hpp
    I_SessionService.hpp
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	SessionServer
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
