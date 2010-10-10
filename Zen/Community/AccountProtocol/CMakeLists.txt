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
    I_ReadGroupRequest.hpp
    I_UpdateAccountRequest.hpp
    I_DeleteAccountResponse.hpp
    I_UnsubscribeAccountModelResponse.hpp
    I_CreateAccountResponse.hpp
    I_UpdateGroupRequest.hpp
    I_SubscribeGroupModelRequest.hpp
    I_ReadAccountResponse.hpp
    I_UpdateAccountResponse.hpp
    I_Message.hpp
    I_SubscribeAccountModelRequest.hpp
    I_AccountProtocolManager.hpp
    I_UnsubscribeGroupModelRequest.hpp
    src/ReadGroupResponse.cpp
    src/AccountModelUpdateMessage.cpp
    src/UpdateGroupResponse.hpp
    src/I_DeleteAccountResponse.cpp
    src/SubscribeGroupModelRequest.hpp
    src/I_ReadGroupRequest.cpp
    src/ReadAccountResponse.cpp
    src/UnsubscribeGroupModelRequest.hpp
    src/UpdateGroupRequest.hpp
    src/CreateGroupRequest.cpp
    src/I_ReadAccountResponse.cpp
    src/UnsubscribeGroupModelResponse.hpp
    src/SubscribeGroupModelResponse.cpp
    src/I_DeleteAccountRequest.cpp
    src/SubscribeAccountModelResponse.hpp
    src/CreateGroupRequest.hpp
    src/AccountModelUpdateMessage.hpp
    src/DeleteAccountResponse.cpp
    src/I_GroupModelUpdateMessage.cpp
    src/DeleteGroupRequest.hpp
    src/I_AccountProtocolManager.cpp
    src/I_UpdateGroupRequest.cpp
    src/DeleteAccountResponse.hpp
    src/Request.hpp
    src/I_CreateGroupResponse.cpp
    src/UnsubscribeAccountModelResponse.hpp
    src/I_ReadAccountRequest.cpp
    src/UpdateGroupResponse.cpp
    src/Request.cpp
    src/CreateAccountResponse.cpp
    src/SubscribeAccountModelRequest.hpp
    src/I_UnsubscribeGroupModelResponse.cpp
    src/I_CreateAccountResponse.cpp
    src/AccountProtocolManager.hpp
    src/I_SubscribeGroupModelResponse.cpp
    src/Message.hpp
    src/MessageFactory.cpp
    src/UpdateAccountResponse.cpp
    src/CreateGroupResponse.hpp
    src/I_CreateGroupRequest.cpp
    src/CreateAccountResponse.hpp
    src/GroupModelUpdateMessage.cpp
    src/I_Message.cpp
    src/ReadAccountRequest.hpp
    src/I_SubscribeAccountModelResponse.cpp
    src/I_DeleteGroupRequest.cpp
    src/UpdateAccountRequest.hpp
    src/I_UpdateAccountRequest.cpp
    src/ReadAccountResponse.hpp
    src/SubscribeGroupModelRequest.cpp
    src/I_SubscribeAccountModelRequest.cpp
    src/Message.cpp
    src/I_DeleteGroupResponse.cpp
    src/DeleteGroupResponse.cpp
    src/I_UnsubscribeAccountModelResponse.cpp
    src/CreateGroupResponse.cpp
    src/UnsubscribeGroupModelResponse.cpp
    src/UpdateAccountRequest.cpp
    src/ReadGroupRequest.hpp
    src/UnsubscribeAccountModelRequest.cpp
    src/Response.cpp
    src/I_SubscribeGroupModelRequest.cpp
    src/UnsubscribeAccountModelResponse.cpp
    src/I_ReadGroupResponse.cpp
    src/ReadAccountRequest.cpp
    src/Response.hpp
    src/GroupModelUpdateMessage.hpp
    src/UnsubscribeAccountModelRequest.hpp
    src/CreateAccountRequest.cpp
    src/UnsubscribeGroupModelRequest.cpp
    src/DeleteGroupResponse.hpp
    src/I_UpdateGroupResponse.cpp
    src/UpdateGroupRequest.cpp
    src/MessageFactory.hpp
    src/I_CreateAccountRequest.cpp
    src/CreateAccountRequest.hpp
    src/SubscribeAccountModelRequest.cpp
    src/DeleteAccountRequest.cpp
    src/I_UnsubscribeGroupModelRequest.cpp
    src/I_UpdateAccountResponse.cpp
    src/SubscribeAccountModelResponse.cpp
    src/DeleteGroupRequest.cpp
    src/ReadGroupResponse.hpp
    src/UpdateAccountResponse.hpp
    src/ReadGroupRequest.cpp
    src/AccountProtocolManager.cpp
    src/I_UnsubscribeAccountModelRequest.cpp
    src/I_AccountModelUpdateMessage.cpp
    src/DeleteAccountRequest.hpp
    src/SubscribeGroupModelResponse.hpp
    I_UnsubscribeAccountModelRequest.hpp
    I_CreateGroupRequest.hpp
    I_CreateAccountRequest.hpp
    I_CreateGroupResponse.hpp
    I_AccountModelUpdateMessage.hpp
    I_ReadGroupResponse.hpp
    I_GroupModelUpdateMessage.hpp
    I_ReadAccountRequest.hpp
    Configuration.hpp
    I_UnsubscribeGroupModelResponse.hpp
    I_DeleteAccountRequest.hpp
    I_UpdateGroupResponse.hpp
    I_DeleteGroupRequest.hpp
    I_DeleteGroupResponse.hpp
    I_SubscribeAccountModelResponse.hpp
    I_SubscribeGroupModelResponse.hpp

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
    I_ReadGroupRequest.hpp
    I_UpdateAccountRequest.hpp
    I_DeleteAccountResponse.hpp
    I_UnsubscribeAccountModelResponse.hpp
    I_CreateAccountResponse.hpp
    I_UpdateGroupRequest.hpp
    I_SubscribeGroupModelRequest.hpp
    I_ReadAccountResponse.hpp
    I_UpdateAccountResponse.hpp
    I_Message.hpp
    I_SubscribeAccountModelRequest.hpp
    I_AccountProtocolManager.hpp
    I_UnsubscribeGroupModelRequest.hpp
    I_UnsubscribeAccountModelRequest.hpp
    I_CreateGroupRequest.hpp
    I_CreateAccountRequest.hpp
    I_CreateGroupResponse.hpp
    I_AccountModelUpdateMessage.hpp
    I_ReadGroupResponse.hpp
    I_GroupModelUpdateMessage.hpp
    I_ReadAccountRequest.hpp
    Configuration.hpp
    I_UnsubscribeGroupModelResponse.hpp
    I_DeleteAccountRequest.hpp
    I_UpdateGroupResponse.hpp
    I_DeleteGroupRequest.hpp
    I_DeleteGroupResponse.hpp
    I_SubscribeAccountModelResponse.hpp
    I_SubscribeGroupModelResponse.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	AccountProtocol
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)