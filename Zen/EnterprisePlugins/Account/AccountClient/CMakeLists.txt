cmake_minimum_required (VERSION 2.6)

project (AccountClient)
set (PROJECT_PREFIX "EnterprisePlugins/Account/AccountClient")

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

set (AccountClient_SRC
        Configuration.hpp
    I_Account.hpp
    I_AccountService.hpp
    I_Group.hpp
    Protocol/I_AccountProtocolManager.hpp
    Protocol/I_AuthenticateRequest.hpp
    Protocol/I_AuthenticateResponse.hpp
    Protocol/I_Message.hpp
    src/Account.cpp
    src/Account.hpp
    src/AccountService.cpp
    src/AccountService.hpp
    src/AccountServiceFactory.cpp
    src/AccountServiceFactory.hpp
    src/AccountServicePlugin.cpp
    src/Group.cpp
    src/Group.hpp
    src/I_Account.cpp
    src/I_AccountService.cpp
    src/I_Group.cpp
    src/Protocol/AccountProtocolManager.cpp
    src/Protocol/AccountProtocolManager.hpp
    src/Protocol/AuthenticateRequest.cpp
    src/Protocol/AuthenticateRequest.hpp
    src/Protocol/AuthenticateResponse.cpp
    src/Protocol/AuthenticateResponse.hpp
    src/Protocol/I_AccountProtocolManager.cpp
    src/Protocol/I_AuthenticateRequest.cpp
    src/Protocol/I_AuthenticateResponse.cpp
    src/Protocol/I_Message.cpp
    src/Protocol/Message.cpp
    src/Protocol/Message.hpp
    src/Protocol/MessageFactory.cpp
    src/Protocol/MessageFactory.hpp
    src/Protocol/Request.cpp
    src/Protocol/Request.hpp
    src/Protocol/Response.cpp
    src/Protocol/Response.hpp

    )
    
add_definitions (-DACCOUNT_CLIENT_EXPORTS ${Zen_DEFINITIONS})

include_directories (${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT})

link_directories (${DEV_LIB} ${Boost_LIBRARY_DIRS})

add_library (AccountClient ${LIB_TYPE} ${AccountClient_SRC})

add_dependencies (AccountClient ZenCore ZenEnterprise)

target_link_libraries (AccountClient ZenCore ZenEnterprise ${Boost_LIBRARIES})

set_target_properties (AccountClient
    PROPERTIES
    DEBUG_POSTFIX "_d"
)

if (MSVC_IDE)
    set_target_properties (AccountClient PROPERTIES PREFIX "../../bin/")
    set_target_properties (AccountClient PROPERTIES IMPORT_PREFIX "../../lib/")
endif (MSVC_IDE)


install ( FILES 
    Protocol/I_AccountProtocolManager.hpp
    Protocol/I_AuthenticateRequest.hpp
    Protocol/I_AuthenticateResponse.hpp
    Protocol/I_Message.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Protocol/
)

install ( FILES 
    Configuration.hpp
    I_Account.hpp
    I_AccountService.hpp
    I_Group.hpp
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
