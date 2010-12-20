cmake_minimum_required (VERSION 2.6) 

project (AccountProtocol)
set ( PROJECT_PREFIX "Community/AccountProtocol" )

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


	
set ( AccountProtocol_SRC
    Configuration.hpp
    I_AccountModelUpdateMessage.hpp
    I_AccountProtocolManager.hpp
    I_CreateAccountRequest.hpp
    I_CreateAccountResponse.hpp
    I_CreateGroupRequest.hpp
    I_CreateGroupResponse.hpp
    I_DeleteAccountRequest.hpp
    I_DeleteAccountResponse.hpp
    I_DeleteGroupRequest.hpp
    I_DeleteGroupResponse.hpp
    I_GroupModelUpdateMessage.hpp
    I_Message.hpp
    I_ReadAccountRequest.hpp
    I_ReadAccountResponse.hpp
    I_ReadGroupRequest.hpp
    I_ReadGroupResponse.hpp
    I_SubscribeAccountModelRequest.hpp
    I_SubscribeAccountModelResponse.hpp
    I_SubscribeGroupModelRequest.hpp
    I_SubscribeGroupModelResponse.hpp
    I_UnsubscribeAccountModelRequest.hpp
    I_UnsubscribeAccountModelResponse.hpp
    I_UnsubscribeGroupModelRequest.hpp
    I_UnsubscribeGroupModelResponse.hpp
    I_UpdateAccountRequest.hpp
    I_UpdateAccountResponse.hpp
    I_UpdateGroupRequest.hpp
    I_UpdateGroupResponse.hpp
    src/AccountModelUpdateMessage.cpp
    src/AccountModelUpdateMessage.hpp
    src/AccountProtocolManager.cpp
    src/AccountProtocolManager.hpp
    src/CreateAccountRequest.cpp
    src/CreateAccountRequest.hpp
    src/CreateAccountResponse.cpp
    src/CreateAccountResponse.hpp
    src/CreateGroupRequest.cpp
    src/CreateGroupRequest.hpp
    src/CreateGroupResponse.cpp
    src/CreateGroupResponse.hpp
    src/DeleteAccountRequest.cpp
    src/DeleteAccountRequest.hpp
    src/DeleteAccountResponse.cpp
    src/DeleteAccountResponse.hpp
    src/DeleteGroupRequest.cpp
    src/DeleteGroupRequest.hpp
    src/DeleteGroupResponse.cpp
    src/DeleteGroupResponse.hpp
    src/GroupModelUpdateMessage.cpp
    src/GroupModelUpdateMessage.hpp
    src/I_AccountModelUpdateMessage.cpp
    src/I_AccountProtocolManager.cpp
    src/I_CreateAccountRequest.cpp
    src/I_CreateAccountResponse.cpp
    src/I_CreateGroupRequest.cpp
    src/I_CreateGroupResponse.cpp
    src/I_DeleteAccountRequest.cpp
    src/I_DeleteAccountResponse.cpp
    src/I_DeleteGroupRequest.cpp
    src/I_DeleteGroupResponse.cpp
    src/I_GroupModelUpdateMessage.cpp
    src/I_Message.cpp
    src/I_ReadAccountRequest.cpp
    src/I_ReadAccountResponse.cpp
    src/I_ReadGroupRequest.cpp
    src/I_ReadGroupResponse.cpp
    src/I_SubscribeAccountModelRequest.cpp
    src/I_SubscribeAccountModelResponse.cpp
    src/I_SubscribeGroupModelRequest.cpp
    src/I_SubscribeGroupModelResponse.cpp
    src/I_UnsubscribeAccountModelRequest.cpp
    src/I_UnsubscribeAccountModelResponse.cpp
    src/I_UnsubscribeGroupModelRequest.cpp
    src/I_UnsubscribeGroupModelResponse.cpp
    src/I_UpdateAccountRequest.cpp
    src/I_UpdateAccountResponse.cpp
    src/I_UpdateGroupRequest.cpp
    src/I_UpdateGroupResponse.cpp
    src/Message.cpp
    src/Message.hpp
    src/MessageFactory.cpp
    src/MessageFactory.hpp
    src/ReadAccountRequest.cpp
    src/ReadAccountRequest.hpp
    src/ReadAccountResponse.cpp
    src/ReadAccountResponse.hpp
    src/ReadGroupRequest.cpp
    src/ReadGroupRequest.hpp
    src/ReadGroupResponse.cpp
    src/ReadGroupResponse.hpp
    src/Request.cpp
    src/Request.hpp
    src/Response.cpp
    src/Response.hpp
    src/SubscribeAccountModelRequest.cpp
    src/SubscribeAccountModelRequest.hpp
    src/SubscribeAccountModelResponse.cpp
    src/SubscribeAccountModelResponse.hpp
    src/SubscribeGroupModelRequest.cpp
    src/SubscribeGroupModelRequest.hpp
    src/SubscribeGroupModelResponse.cpp
    src/SubscribeGroupModelResponse.hpp
    src/UnsubscribeAccountModelRequest.cpp
    src/UnsubscribeAccountModelRequest.hpp
    src/UnsubscribeAccountModelResponse.cpp
    src/UnsubscribeAccountModelResponse.hpp
    src/UnsubscribeGroupModelRequest.cpp
    src/UnsubscribeGroupModelRequest.hpp
    src/UnsubscribeGroupModelResponse.cpp
    src/UnsubscribeGroupModelResponse.hpp
    src/UpdateAccountRequest.cpp
    src/UpdateAccountRequest.hpp
    src/UpdateAccountResponse.cpp
    src/UpdateAccountResponse.hpp
    src/UpdateGroupRequest.cpp
    src/UpdateGroupRequest.hpp
    src/UpdateGroupResponse.cpp
    src/UpdateGroupResponse.hpp

    )

add_definitions( -DACCOUNTPROTOCOL_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( AccountProtocol ${LIB_TYPE} ${AccountProtocol_SRC} )

add_dependencies(AccountProtocol ZenCore ZenEngine ZenEnterprise AccountCommon)

target_link_libraries(AccountProtocol ZenCore ZenEngine ZenEnterprise AccountCommon ${Boost_LIBRARIES} )

set_target_properties ( AccountProtocol
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( AccountProtocol PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( AccountProtocol PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_AccountModelUpdateMessage.hpp
    I_AccountProtocolManager.hpp
    I_CreateAccountRequest.hpp
    I_CreateAccountResponse.hpp
    I_CreateGroupRequest.hpp
    I_CreateGroupResponse.hpp
    I_DeleteAccountRequest.hpp
    I_DeleteAccountResponse.hpp
    I_DeleteGroupRequest.hpp
    I_DeleteGroupResponse.hpp
    I_GroupModelUpdateMessage.hpp
    I_Message.hpp
    I_ReadAccountRequest.hpp
    I_ReadAccountResponse.hpp
    I_ReadGroupRequest.hpp
    I_ReadGroupResponse.hpp
    I_SubscribeAccountModelRequest.hpp
    I_SubscribeAccountModelResponse.hpp
    I_SubscribeGroupModelRequest.hpp
    I_SubscribeGroupModelResponse.hpp
    I_UnsubscribeAccountModelRequest.hpp
    I_UnsubscribeAccountModelResponse.hpp
    I_UnsubscribeGroupModelRequest.hpp
    I_UnsubscribeGroupModelResponse.hpp
    I_UpdateAccountRequest.hpp
    I_UpdateAccountResponse.hpp
    I_UpdateGroupRequest.hpp
    I_UpdateGroupResponse.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	AccountProtocol
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)