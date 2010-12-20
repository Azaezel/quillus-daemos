cmake_minimum_required (VERSION 2.6) 

project (ChatProtocol)
set ( PROJECT_PREFIX "Community/ChatProtocol" )

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


	
set ( ChatProtocol_SRC
    Configuration.hpp
    I_ChannelModelUpdateMessage.hpp
    I_ChatProtocolManager.hpp
    I_CreateChannelRequest.hpp
    I_CreateChannelResponse.hpp
    I_DeleteChannelRequest.hpp
    I_DeleteChannelResponse.hpp
    I_JoinChannelRequest.hpp
    I_JoinChannelResponse.hpp
    I_Message.hpp
    I_ReadChannelRequest.hpp
    I_ReadChannelResponse.hpp
    I_SubscribeChannelModelRequest.hpp
    I_SubscribeChannelModelResponse.hpp
    I_UnsubscribeChannelModelRequest.hpp
    I_UnsubscribeChannelModelResponse.hpp
    I_UpdateChannelRequest.hpp
    I_UpdateChannelResponse.hpp
    src/ChannelModelUpdateMessage.cpp
    src/ChannelModelUpdateMessage.hpp
    src/ChatProtocolManager.cpp
    src/ChatProtocolManager.hpp
    src/CreateChannelRequest.cpp
    src/CreateChannelRequest.hpp
    src/CreateChannelResponse.cpp
    src/CreateChannelResponse.hpp
    src/DeleteChannelRequest.cpp
    src/DeleteChannelRequest.hpp
    src/DeleteChannelResponse.cpp
    src/DeleteChannelResponse.hpp
    src/I_ChannelModelUpdateMessage.cpp
    src/I_ChatProtocolManager.cpp
    src/I_CreateChannelRequest.cpp
    src/I_CreateChannelResponse.cpp
    src/I_DeleteChannelRequest.cpp
    src/I_DeleteChannelResponse.cpp
    src/I_JoinChannelRequest.cpp
    src/I_JoinChannelResponse.cpp
    src/I_Message.cpp
    src/I_ReadChannelRequest.cpp
    src/I_ReadChannelResponse.cpp
    src/I_SubscribeChannelModelRequest.cpp
    src/I_SubscribeChannelModelResponse.cpp
    src/I_UnsubscribeChannelModelRequest.cpp
    src/I_UnsubscribeChannelModelResponse.cpp
    src/I_UpdateChannelRequest.cpp
    src/I_UpdateChannelResponse.cpp
    src/JoinChannelRequest.cpp
    src/JoinChannelRequest.hpp
    src/JoinChannelResponse.cpp
    src/JoinChannelResponse.hpp
    src/Message.cpp
    src/Message.hpp
    src/MessageFactory.cpp
    src/MessageFactory.hpp
    src/ReadChannelRequest.cpp
    src/ReadChannelRequest.hpp
    src/ReadChannelResponse.cpp
    src/ReadChannelResponse.hpp
    src/Request.cpp
    src/Request.hpp
    src/Response.cpp
    src/Response.hpp
    src/SubscribeChannelModelRequest.cpp
    src/SubscribeChannelModelRequest.hpp
    src/SubscribeChannelModelResponse.cpp
    src/SubscribeChannelModelResponse.hpp
    src/UnsubscribeChannelModelRequest.cpp
    src/UnsubscribeChannelModelRequest.hpp
    src/UnsubscribeChannelModelResponse.cpp
    src/UnsubscribeChannelModelResponse.hpp
    src/UpdateChannelRequest.cpp
    src/UpdateChannelRequest.hpp
    src/UpdateChannelResponse.cpp
    src/UpdateChannelResponse.hpp

    )

add_definitions( -DCHATPROTOCOL_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ChatProtocol ${LIB_TYPE} ${ChatProtocol_SRC} )

add_dependencies(ChatProtocol ZenCore ZenEngine ZenEnterprise ChatCommon)

target_link_libraries(ChatProtocol ZenCore ZenEngine ZenEnterprise ChatCommon ${Boost_LIBRARIES} )

set_target_properties ( ChatProtocol
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ChatProtocol PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ChatProtocol PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_ChannelModelUpdateMessage.hpp
    I_ChatProtocolManager.hpp
    I_CreateChannelRequest.hpp
    I_CreateChannelResponse.hpp
    I_DeleteChannelRequest.hpp
    I_DeleteChannelResponse.hpp
    I_JoinChannelRequest.hpp
    I_JoinChannelResponse.hpp
    I_Message.hpp
    I_ReadChannelRequest.hpp
    I_ReadChannelResponse.hpp
    I_SubscribeChannelModelRequest.hpp
    I_SubscribeChannelModelResponse.hpp
    I_UnsubscribeChannelModelRequest.hpp
    I_UnsubscribeChannelModelResponse.hpp
    I_UpdateChannelRequest.hpp
    I_UpdateChannelResponse.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ChatProtocol
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)