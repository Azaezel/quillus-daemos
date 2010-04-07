cmake_minimum_required (VERSION 2.6) 

project (ChatClient)
set ( PROJECT_PREFIX "Community/ChatClient" )

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


	
set ( ChatClient_SRC
    Configuration.hpp
    src/Channel.cpp
    src/Channel.hpp
    src/ChannelController.cpp
    src/ChannelController.hpp
    src/ChannelMemberController.cpp
    src/ChannelMemberController.hpp
    src/ChannelMemberModelProxy.cpp
    src/ChannelMemberModelProxy.hpp
    src/ChannelModelProxy.cpp
    src/ChannelModelProxy.hpp
    src/ChatClientPlugin.cpp
    src/ChatService.cpp
    src/ChatService.hpp
    src/ChatServiceFactory.cpp
    src/ChatServiceFactory.hpp

    )

add_definitions( -DACCOUNTCLIENT_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ChatClient ${LIB_TYPE} ${ChatClient_SRC} )

add_dependencies(ChatClient ZenCore ZenEngine ZenEnterprise ZenObjectModel ChatCommon ChatProtocol)

target_link_libraries(ChatClient ZenCore ZenEngine ZenEnterprise ZenObjectModel ChatCommon ChatProtocol ${Boost_LIBRARIES} )

set_target_properties ( ChatClient
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ChatClient PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ChatClient PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ChatClient
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)