cmake_minimum_required (VERSION 2.6)

project (SessionClient)
set (PROJECT_PREFIX "EnterprisePlugins/Session/SessionClient")

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

set (SessionClient_SRC
        Configuration.hpp
    I_Session.hpp
    I_SessionService.hpp
    Protocol/I_LoginRequest.hpp
    Protocol/I_LoginResponse.hpp
    Protocol/I_LogoutRequest.hpp
    Protocol/I_LogoutResponse.hpp
    Protocol/I_Message.hpp
    Protocol/I_SessionProtocolManager.hpp
    src/I_Session.cpp
    src/I_SessionService.cpp
    src/Protocol/I_LoginRequest.cpp
    src/Protocol/I_LoginResponse.cpp
    src/Protocol/I_LogoutRequest.cpp
    src/Protocol/I_LogoutResponse.cpp
    src/Protocol/I_Message.cpp
    src/Protocol/I_SessionProtocolManager.cpp
    src/Protocol/LoginRequest.cpp
    src/Protocol/LoginRequest.hpp
    src/Protocol/LoginResponse.cpp
    src/Protocol/LoginResponse.hpp
    src/Protocol/LogoutRequest.cpp
    src/Protocol/LogoutRequest.hpp
    src/Protocol/LogoutResponse.cpp
    src/Protocol/LogoutResponse.hpp
    src/Protocol/Message.cpp
    src/Protocol/Message.hpp
    src/Protocol/MessageFactory.cpp
    src/Protocol/MessageFactory.hpp
    src/Protocol/Request.cpp
    src/Protocol/Request.hpp
    src/Protocol/Response.cpp
    src/Protocol/Response.hpp
    src/Protocol/SessionProtocolManager.cpp
    src/Protocol/SessionProtocolManager.hpp
    src/Session.cpp
    src/Session.hpp
    src/SessionService.cpp
    src/SessionService.hpp
    src/SessionServiceFactory.cpp
    src/SessionServiceFactory.hpp
    src/SessionServicePlugin.cpp

    )
    
add_definitions (-DSESSION_CLIENT_EXPORTS ${Zen_DEFINITIONS})

include_directories (${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT})

link_directories (${DEV_LIB} ${Boost_LIBRARY_DIRS})

add_library (SessionClient ${LIB_TYPE} ${SessionClient_SRC})

add_dependencies (SessionClient ZenCore ZenEnterprise)

target_link_libraries (SessionClient ZenCore ZenEnterprise ${Boost_LIBRARIES})

set_target_properties (SessionClient
    PROPERTIES
    DEBUG_POSTFIX "_d"
)

if (MSVC_IDE)
    set_target_properties (SessionClient PROPERTIES PREFIX "../../bin/")
    set_target_properties (SessionClient PROPERTIES IMPORT_PREFIX "../../lib/")
endif (MSVC_IDE)


install ( FILES 
    Protocol/I_LoginRequest.hpp
    Protocol/I_LoginResponse.hpp
    Protocol/I_LogoutRequest.hpp
    Protocol/I_LogoutResponse.hpp
    Protocol/I_Message.hpp
    Protocol/I_SessionProtocolManager.hpp
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
	SessionClient
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
