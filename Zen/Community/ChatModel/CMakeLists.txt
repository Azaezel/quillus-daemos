cmake_minimum_required (VERSION 2.6) 

project (ChatModel)
set ( PROJECT_PREFIX "Community/ChatModel" )

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


	
set ( ChatModel_SRC
    Configuration.hpp
    I_AllowGroupDataCollection.hpp
    I_AllowGroupDataMap.hpp
    I_AllowGroupDomainObject.hpp
    I_AllowUserDataCollection.hpp
    I_AllowUserDataMap.hpp
    I_AllowUserDomainObject.hpp
    I_ChatChannelDataCollection.hpp
    I_ChatChannelDataMap.hpp
    I_ChatChannelDomainObject.hpp
    I_ChatCommandDataCollection.hpp
    I_ChatCommandDataMap.hpp
    I_ChatCommandDomainObject.hpp
    I_ChatEventDataCollection.hpp
    I_ChatEventDataMap.hpp
    I_ChatEventDomainObject.hpp
    I_ChatLogDataCollection.hpp
    I_ChatLogDataMap.hpp
    I_ChatLogDomainObject.hpp
    I_DenyGroupDataCollection.hpp
    I_DenyGroupDataMap.hpp
    I_DenyGroupDomainObject.hpp
    I_DenyUserDataCollection.hpp
    I_DenyUserDataMap.hpp
    I_DenyUserDomainObject.hpp
    src/AllowGroupDataCollection.cpp
    src/AllowGroupDataCollection.hpp
    src/AllowGroupDataMap.cpp
    src/AllowGroupDataMap.hpp
    src/AllowGroupDomainObject.cpp
    src/AllowGroupDomainObject.hpp
    src/AllowUserDataCollection.cpp
    src/AllowUserDataCollection.hpp
    src/AllowUserDataMap.cpp
    src/AllowUserDataMap.hpp
    src/AllowUserDomainObject.cpp
    src/AllowUserDomainObject.hpp
    src/ChatChannelDataCollection.cpp
    src/ChatChannelDataCollection.hpp
    src/ChatChannelDataMap.cpp
    src/ChatChannelDataMap.hpp
    src/ChatChannelDomainObject.cpp
    src/ChatChannelDomainObject.hpp
    src/ChatCommandDataCollection.cpp
    src/ChatCommandDataCollection.hpp
    src/ChatCommandDataMap.cpp
    src/ChatCommandDataMap.hpp
    src/ChatCommandDomainObject.cpp
    src/ChatCommandDomainObject.hpp
    src/ChatEventDataCollection.cpp
    src/ChatEventDataCollection.hpp
    src/ChatEventDataMap.cpp
    src/ChatEventDataMap.hpp
    src/ChatEventDomainObject.cpp
    src/ChatEventDomainObject.hpp
    src/ChatLogDataCollection.cpp
    src/ChatLogDataCollection.hpp
    src/ChatLogDataMap.cpp
    src/ChatLogDataMap.hpp
    src/ChatLogDomainObject.cpp
    src/ChatLogDomainObject.hpp
    src/DenyGroupDataCollection.cpp
    src/DenyGroupDataCollection.hpp
    src/DenyGroupDataMap.cpp
    src/DenyGroupDataMap.hpp
    src/DenyGroupDomainObject.cpp
    src/DenyGroupDomainObject.hpp
    src/DenyUserDataCollection.cpp
    src/DenyUserDataCollection.hpp
    src/DenyUserDataMap.cpp
    src/DenyUserDataMap.hpp
    src/DenyUserDomainObject.cpp
    src/DenyUserDomainObject.hpp
    src/I_AllowGroupDataCollection.cpp
    src/I_AllowGroupDataMap.cpp
    src/I_AllowGroupDomainObject.cpp
    src/I_AllowUserDataCollection.cpp
    src/I_AllowUserDataMap.cpp
    src/I_AllowUserDomainObject.cpp
    src/I_ChatChannelDataCollection.cpp
    src/I_ChatChannelDataMap.cpp
    src/I_ChatChannelDomainObject.cpp
    src/I_ChatCommandDataCollection.cpp
    src/I_ChatCommandDataMap.cpp
    src/I_ChatCommandDomainObject.cpp
    src/I_ChatEventDataCollection.cpp
    src/I_ChatEventDataMap.cpp
    src/I_ChatEventDomainObject.cpp
    src/I_ChatLogDataCollection.cpp
    src/I_ChatLogDataMap.cpp
    src/I_ChatLogDomainObject.cpp
    src/I_DenyGroupDataCollection.cpp
    src/I_DenyGroupDataMap.cpp
    src/I_DenyGroupDomainObject.cpp
    src/I_DenyUserDataCollection.cpp
    src/I_DenyUserDataMap.cpp
    src/I_DenyUserDomainObject.cpp

    )

add_definitions( -DCHATMODEL_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${LIBXML2_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} )

add_library ( ChatModel ${LIB_TYPE} ${ChatModel_SRC} )

add_dependencies(ChatModel ZenCore ZenEnterprise)

target_link_libraries( ChatModel ZenCore ZenEnterprise )

set_target_properties ( ChatModel 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ChatModel PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ChatModel PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_AllowGroupDataCollection.hpp
    I_AllowGroupDataMap.hpp
    I_AllowGroupDomainObject.hpp
    I_AllowUserDataCollection.hpp
    I_AllowUserDataMap.hpp
    I_AllowUserDomainObject.hpp
    I_ChatChannelDataCollection.hpp
    I_ChatChannelDataMap.hpp
    I_ChatChannelDomainObject.hpp
    I_ChatCommandDataCollection.hpp
    I_ChatCommandDataMap.hpp
    I_ChatCommandDomainObject.hpp
    I_ChatEventDataCollection.hpp
    I_ChatEventDataMap.hpp
    I_ChatEventDomainObject.hpp
    I_ChatLogDataCollection.hpp
    I_ChatLogDataMap.hpp
    I_ChatLogDomainObject.hpp
    I_DenyGroupDataCollection.hpp
    I_DenyGroupDataMap.hpp
    I_DenyGroupDomainObject.hpp
    I_DenyUserDataCollection.hpp
    I_DenyUserDataMap.hpp
    I_DenyUserDomainObject.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ChatModel
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)