cmake_minimum_required (VERSION 2.6)

project (AccountServer)
set (PROJECT_PREFIX "EnterprisePlugins/Account/AccountServer")

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

set (AccountServer_SRC
        Configuration.hpp
    I_Account.hpp
    I_AccountGroup.hpp
    I_AccountService.hpp
    I_Group.hpp
    Protocol/I_AccountMessage.hpp
    Protocol/I_AccountProtocolManager.hpp
    Protocol/I_AddAccountToGroupRequest.hpp
    Protocol/I_AddAccountToGroupResponse.hpp
    Protocol/I_CreateAccountRequest.hpp
    Protocol/I_CreateAccountResponse.hpp
    Protocol/I_CreateGroupRequest.hpp
    Protocol/I_CreateGroupResponse.hpp
    Protocol/I_DeleteAccountRequest.hpp
    Protocol/I_DeleteAccountResponse.hpp
    Protocol/I_DeleteGroupRequest.hpp
    Protocol/I_DeleteGroupResponse.hpp
    Protocol/I_RemoveAccountFromGroupRequest.hpp
    Protocol/I_RemoveAccountFromGroupResponse.hpp
    Protocol/I_SubscribeAccountRequest.hpp
    Protocol/I_SubscribeAccountResponse.hpp
    Protocol/I_SubscribeGroupRequest.hpp
    Protocol/I_SubscribeGroupResponse.hpp
    Protocol/I_UpdateAccountRequest.hpp
    Protocol/I_UpdateAccountResponse.hpp
    Protocol/I_UpdateGroupRequest.hpp
    Protocol/I_UpdateGroupResponse.hpp
    src/Account.cpp
    src/Account.hpp
    src/AccountGroup.cpp
    src/AccountGroup.hpp
    src/AccountService.cpp
    src/AccountService.hpp
    src/AccountServiceFactory.cpp
    src/AccountServiceFactory.hpp
    src/AccountServicePlugin.cpp
    src/Group.cpp
    src/Group.hpp
    src/I_Account.cpp
    src/I_AccountGroup.cpp
    src/I_AccountService.cpp
    src/I_Group.cpp
    src/Protocol/AccountProtocolManager.cpp
    src/Protocol/AccountProtocolManager.hpp
    src/Protocol/AddAccountToGroupRequest.cpp
    src/Protocol/AddAccountToGroupRequest.hpp
    src/Protocol/AddAccountToGroupResponse.cpp
    src/Protocol/AddAccountToGroupResponse.hpp
    src/Protocol/CreateAccountRequest.cpp
    src/Protocol/CreateAccountRequest.hpp
    src/Protocol/CreateAccountResponse.cpp
    src/Protocol/CreateAccountResponse.hpp
    src/Protocol/CreateGroupRequest.cpp
    src/Protocol/CreateGroupRequest.hpp
    src/Protocol/CreateGroupResponse.cpp
    src/Protocol/CreateGroupResponse.hpp
    src/Protocol/DeleteAccountRequest.cpp
    src/Protocol/DeleteAccountRequest.hpp
    src/Protocol/DeleteAccountResponse.cpp
    src/Protocol/DeleteAccountResponse.hpp
    src/Protocol/DeleteGroupRequest.cpp
    src/Protocol/DeleteGroupRequest.hpp
    src/Protocol/DeleteGroupResponse.cpp
    src/Protocol/DeleteGroupResponse.hpp
    src/Protocol/I_AccountProtocolManager.cpp
    src/Protocol/I_AddAccountToGroupRequest.cpp
    src/Protocol/I_AddAccountToGroupResponse.cpp
    src/Protocol/I_CreateAccountRequest.cpp
    src/Protocol/I_CreateAccountResponse.cpp
    src/Protocol/I_CreateGroupRequest.cpp
    src/Protocol/I_CreateGroupResponse.cpp
    src/Protocol/I_DeleteAccountRequest.cpp
    src/Protocol/I_DeleteAccountResponse.cpp
    src/Protocol/I_DeleteGroupRequest.cpp
    src/Protocol/I_DeleteGroupResponse.cpp
    src/Protocol/I_RemoveAccountFromGroupRequest.cpp
    src/Protocol/I_RemoveAccountFromGroupResponse.cpp
    src/Protocol/I_SubscribeAccountRequest.cpp
    src/Protocol/I_SubscribeAccountResponse.cpp
    src/Protocol/I_SubscribeGroupRequest.cpp
    src/Protocol/I_SubscribeGroupResponse.cpp
    src/Protocol/I_UpdateAccountRequest.cpp
    src/Protocol/I_UpdateAccountResponse.cpp
    src/Protocol/I_UpdateGroupRequest.cpp
    src/Protocol/I_UpdateGroupResponse.cpp
    src/Protocol/RemoveAccountFromGroupRequest.cpp
    src/Protocol/RemoveAccountFromGroupRequest.hpp
    src/Protocol/RemoveAccountFromGroupResponse.cpp
    src/Protocol/RemoveAccountFromGroupResponse.hpp
    src/Protocol/SubscribeAccountRequest.cpp
    src/Protocol/SubscribeAccountRequest.hpp
    src/Protocol/SubscribeAccountResponse.cpp
    src/Protocol/SubscribeAccountResponse.hpp
    src/Protocol/SubscribeGroupRequest.cpp
    src/Protocol/SubscribeGroupRequest.hpp
    src/Protocol/SubscribeGroupResponse.cpp
    src/Protocol/SubscribeGroupResponse.hpp
    src/Protocol/UpdateAccountRequest.cpp
    src/Protocol/UpdateAccountRequest.hpp
    src/Protocol/UpdateAccountResponse.cpp
    src/Protocol/UpdateAccountResponse.hpp
    src/Protocol/UpdateGroupRequest.cpp
    src/Protocol/UpdateGroupRequest.hpp
    src/Protocol/UpdateGroupResponse.cpp
    src/Protocol/UpdateGroupResponse.hpp

    )
    
add_definitions (-DACCOUNT_SERVER_EXPORTS ${Zen_DEFINITIONS})

include_directories (${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT})

link_directories (${DEV_LIB} ${Boost_LIBRARY_DIRS})

add_library (AccountServer ${LIB_TYPE} ${AccountServer_SRC})

add_dependencies (AccountServer ZenCore ZenEnterprise AccountClient)

target_link_libraries (AccountServer ZenCore ZenEnterprise AccountClient ${Boost_LIBRARIES})

set_target_properties (AccountServer
    PROPERTIES
    DEBUG_POSTFIX "_d"
)

if (MSVC_IDE)
    set_target_properties (AccountServer PROPERTIES PREFIX "../../bin/")
    set_target_properties (AccountServer PROPERTIES IMPORT_PREFIX "../../lib/")
endif (MSVC_IDE)


install ( FILES 
    Protocol/I_AccountMessage.hpp
    Protocol/I_AccountProtocolManager.hpp
    Protocol/I_AddAccountToGroupRequest.hpp
    Protocol/I_AddAccountToGroupResponse.hpp
    Protocol/I_CreateAccountRequest.hpp
    Protocol/I_CreateAccountResponse.hpp
    Protocol/I_CreateGroupRequest.hpp
    Protocol/I_CreateGroupResponse.hpp
    Protocol/I_DeleteAccountRequest.hpp
    Protocol/I_DeleteAccountResponse.hpp
    Protocol/I_DeleteGroupRequest.hpp
    Protocol/I_DeleteGroupResponse.hpp
    Protocol/I_RemoveAccountFromGroupRequest.hpp
    Protocol/I_RemoveAccountFromGroupResponse.hpp
    Protocol/I_SubscribeAccountRequest.hpp
    Protocol/I_SubscribeAccountResponse.hpp
    Protocol/I_SubscribeGroupRequest.hpp
    Protocol/I_SubscribeGroupResponse.hpp
    Protocol/I_UpdateAccountRequest.hpp
    Protocol/I_UpdateAccountResponse.hpp
    Protocol/I_UpdateGroupRequest.hpp
    Protocol/I_UpdateGroupResponse.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Protocol/
)

install ( FILES 
    Configuration.hpp
    I_Account.hpp
    I_AccountGroup.hpp
    I_AccountService.hpp
    I_Group.hpp
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	AccountServer
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
