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
    I_UpdateChannelRequest.hpp
    I_CreateChannelRequest.hpp
    I_ChatProtocolManager.hpp
    I_UnsubscribeChannelModelRequest.hpp
    I_CreateChannelResponse.hpp
    I_DeleteChannelResponse.hpp
    I_Message.hpp
    I_ReadChannelResponse.hpp
    I_ReadChannelRequest.hpp
    I_UpdateChannelResponse.hpp
    I_UnsubscribeChannelModelResponse.hpp
    I_SubscribeChannelModelRequest.hpp
    I_JoinChannelResponse.hpp
    src/ChatProtocolManager.hpp
    src/CreateChannelResponse.cpp
    src/I_UnsubscribeChannelModelResponse.cpp
    src/I_UpdateChannelResponse.cpp
    src/SubscribeChannelModelRequest.hpp
    src/UnsubscribeChannelModelRequest.hpp
    src/I_DeleteChannelRequest.cpp
    src/JoinChannelRequest.hpp
    src/CreateChannelRequest.cpp
    src/SubscribeChannelModelRequest.cpp
    src/CreateChannelRequest.hpp
    src/I_ReadChannelRequest.cpp
    src/CreateChannelResponse.hpp
    src/I_CreateChannelRequest.cpp
    src/I_JoinChannelRequest.cpp
    src/Request.hpp
    src/DeleteChannelRequest.cpp
    src/ReadChannelResponse.hpp
    src/JoinChannelRequest.cpp
    src/UpdateChannelRequest.hpp
    src/I_UnsubscribeChannelModelRequest.cpp
    src/UnsubscribeChannelModelRequest.cpp
    src/ChannelModelUpdateMessage.cpp
    src/DeleteChannelResponse.cpp
    src/Request.cpp
    src/DeleteChannelResponse.hpp
    src/Message.hpp
    src/MessageFactory.cpp
    src/I_JoinChannelResponse.cpp
    src/I_Message.cpp
    src/UnsubscribeChannelModelResponse.cpp
    src/I_ChatProtocolManager.cpp
    src/Message.cpp
    src/ChatProtocolManager.cpp
    src/I_SubscribeChannelModelResponse.cpp
    src/I_SubscribeChannelModelRequest.cpp
    src/ReadChannelResponse.cpp
    src/SubscribeChannelModelResponse.cpp
    src/Response.cpp
    src/SubscribeChannelModelResponse.hpp
    src/ChannelModelUpdateMessage.hpp
    src/I_ReadChannelResponse.cpp
    src/UpdateChannelResponse.hpp
    src/Response.hpp
    src/ReadChannelRequest.cpp
    src/I_UpdateChannelRequest.cpp
    src/I_CreateChannelResponse.cpp
    src/MessageFactory.hpp
    src/JoinChannelResponse.hpp
    src/UpdateChannelResponse.cpp
    src/DeleteChannelRequest.hpp
    src/I_ChannelModelUpdateMessage.cpp
    src/I_DeleteChannelResponse.cpp
    src/ReadChannelRequest.hpp
    src/UnsubscribeChannelModelResponse.hpp
    src/UpdateChannelRequest.cpp
    src/JoinChannelResponse.cpp
    I_JoinChannelRequest.hpp
    I_ChannelModelUpdateMessage.hpp
    I_SubscribeChannelModelResponse.hpp
    Configuration.hpp
    I_DeleteChannelRequest.hpp

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
    I_UpdateChannelRequest.hpp
    I_CreateChannelRequest.hpp
    I_ChatProtocolManager.hpp
    I_UnsubscribeChannelModelRequest.hpp
    I_CreateChannelResponse.hpp
    I_DeleteChannelResponse.hpp
    I_Message.hpp
    I_ReadChannelResponse.hpp
    I_ReadChannelRequest.hpp
    I_UpdateChannelResponse.hpp
    I_UnsubscribeChannelModelResponse.hpp
    I_SubscribeChannelModelRequest.hpp
    I_JoinChannelResponse.hpp
    I_JoinChannelRequest.hpp
    I_ChannelModelUpdateMessage.hpp
    I_SubscribeChannelModelResponse.hpp
    Configuration.hpp
    I_DeleteChannelRequest.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ChatProtocol
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)