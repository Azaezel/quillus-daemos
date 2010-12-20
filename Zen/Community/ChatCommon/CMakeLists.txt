cmake_minimum_required (VERSION 2.6) 

project (ChatCommon)
set ( PROJECT_PREFIX "Community/ChatCommon" )

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


	
set ( ChatCommon_SRC
    Configuration.hpp
    I_Channel.hpp
    I_ChannelController.hpp
    I_ChannelMemberController.hpp
    I_ChannelMemberModel.hpp
    I_ChannelMemberView.hpp
    I_ChannelModel.hpp
    I_ChannelView.hpp
    I_ChatCommand.hpp
    I_ChatCommandController.hpp
    I_ChatCommandModel.hpp
    I_ChatCommandView.hpp
    I_ChatEvent.hpp
    I_ChatEventController.hpp
    I_ChatEventModel.hpp
    I_ChatEventView.hpp
    I_ChatService.hpp
    src/I_Channel.cpp
    src/I_ChannelController.cpp
    src/I_ChannelMemberController.cpp
    src/I_ChannelMemberModel.cpp
    src/I_ChannelMemberView.cpp
    src/I_ChannelModel.cpp
    src/I_ChannelView.cpp
    src/I_ChatCommand.cpp
    src/I_ChatCommandController.cpp
    src/I_ChatCommandModel.cpp
    src/I_ChatCommandView.cpp
    src/I_ChatEvent.cpp
    src/I_ChatEventController.cpp
    src/I_ChatEventModel.cpp
    src/I_ChatEventView.cpp
    src/I_ChatService.cpp
    subscription.hpp

    )

add_definitions( -DCHATCOMMON_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ChatCommon ${LIB_TYPE} ${ChatCommon_SRC} )

add_dependencies(ChatCommon ZenCore ZenEngine ZenEnterprise)

target_link_libraries(ChatCommon ZenCore ZenEngine ZenEnterprise AccountCommon ${Boost_LIBRARIES} )

set_target_properties ( ChatCommon
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ChatCommon PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ChatCommon PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_Channel.hpp
    I_ChannelController.hpp
    I_ChannelMemberController.hpp
    I_ChannelMemberModel.hpp
    I_ChannelMemberView.hpp
    I_ChannelModel.hpp
    I_ChannelView.hpp
    I_ChatCommand.hpp
    I_ChatCommandController.hpp
    I_ChatCommandModel.hpp
    I_ChatCommandView.hpp
    I_ChatEvent.hpp
    I_ChatEventController.hpp
    I_ChatEventModel.hpp
    I_ChatEventView.hpp
    I_ChatService.hpp
    subscription.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ChatCommon
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)