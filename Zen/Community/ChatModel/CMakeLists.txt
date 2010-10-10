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
    I_ChatCommandDomainObject.hpp
    I_ChatEventDataCollection.hpp
    I_DenyGroupDomainObject.hpp
    I_DenyUserDomainObject.hpp
    I_ChatChannelDataMap.hpp
    I_ChatCommandDataCollection.hpp
    I_ChatEventDataMap.hpp
    I_ChatChannelDataCollection.hpp
    I_AllowGroupDataMap.hpp
    I_DenyGroupDataCollection.hpp
    I_AllowGroupDataCollection.hpp
    I_AllowGroupDomainObject.hpp
    I_ChatLogDomainObject.hpp
    I_ChatEventDomainObject.hpp
    I_DenyGroupDataMap.hpp
    I_ChatLogDataCollection.hpp
    I_ChatChannelDomainObject.hpp
    src/AllowUserDataCollection.hpp
    src/DenyGroupDataCollection.hpp
    src/DenyUserDataCollection.cpp
    src/I_AllowGroupDomainObject.cpp
    src/AllowUserDataMap.cpp
    src/DenyGroupDataMap.hpp
    src/DenyGroupDomainObject.hpp
    src/AllowGroupDataMap.cpp
    src/ChatCommandDataMap.hpp
    src/I_ChatCommandDataMap.cpp
    src/ChatCommandDomainObject.cpp
    src/DenyGroupDomainObject.cpp
    src/ChatCommandDataMap.cpp
    src/AllowUserDataCollection.cpp
    src/ChatEventDomainObject.hpp
    src/ChatEventDomainObject.cpp
    src/AllowUserDomainObject.hpp
    src/I_ChatCommandDataCollection.cpp
    src/ChatEventDataCollection.cpp
    src/I_DenyGroupDomainObject.cpp
    src/AllowGroupDataCollection.hpp
    src/I_ChatChannelDataCollection.cpp
    src/DenyUserDomainObject.hpp
    src/AllowGroupDataMap.hpp
    src/ChatChannelDomainObject.cpp
    src/I_ChatChannelDataMap.cpp
    src/I_ChatLogDataCollection.cpp
    src/ChatEventDataMap.hpp
    src/I_ChatEventDataCollection.cpp
    src/ChatEventDataCollection.hpp
    src/DenyUserDomainObject.cpp
    src/ChatChannelDataMap.cpp
    src/ChatCommandDataCollection.hpp
    src/ChatLogDomainObject.cpp
    src/I_ChatLogDomainObject.cpp
    src/I_AllowUserDomainObject.cpp
    src/DenyUserDataCollection.hpp
    src/ChatLogDataCollection.hpp
    src/I_DenyGroupDataMap.cpp
    src/ChatLogDataMap.hpp
    src/ChatLogDataCollection.cpp
    src/I_ChatChannelDomainObject.cpp
    src/I_ChatEventDataMap.cpp
    src/DenyUserDataMap.cpp
    src/ChatCommandDataCollection.cpp
    src/ChatChannelDataMap.hpp
    src/DenyGroupDataMap.cpp
    src/I_DenyGroupDataCollection.cpp
    src/AllowUserDataMap.hpp
    src/AllowUserDomainObject.cpp
    src/AllowGroupDataCollection.cpp
    src/I_AllowUserDataCollection.cpp
    src/I_DenyUserDataMap.cpp
    src/ChatChannelDataCollection.cpp
    src/DenyUserDataMap.hpp
    src/I_AllowGroupDataCollection.cpp
    src/ChatLogDomainObject.hpp
    src/I_ChatLogDataMap.cpp
    src/I_AllowGroupDataMap.cpp
    src/I_ChatCommandDomainObject.cpp
    src/DenyGroupDataCollection.cpp
    src/ChatLogDataMap.cpp
    src/ChatCommandDomainObject.hpp
    src/I_DenyUserDomainObject.cpp
    src/ChatChannelDomainObject.hpp
    src/ChatEventDataMap.cpp
    src/AllowGroupDomainObject.cpp
    src/I_DenyUserDataCollection.cpp
    src/I_ChatEventDomainObject.cpp
    src/AllowGroupDomainObject.hpp
    src/ChatChannelDataCollection.hpp
    src/I_AllowUserDataMap.cpp
    I_DenyUserDataCollection.hpp
    I_AllowUserDataMap.hpp
    I_AllowUserDomainObject.hpp
    I_AllowUserDataCollection.hpp
    I_ChatCommandDataMap.hpp
    Configuration.hpp
    I_ChatLogDataMap.hpp
    I_DenyUserDataMap.hpp

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
    I_ChatCommandDomainObject.hpp
    I_ChatEventDataCollection.hpp
    I_DenyGroupDomainObject.hpp
    I_DenyUserDomainObject.hpp
    I_ChatChannelDataMap.hpp
    I_ChatCommandDataCollection.hpp
    I_ChatEventDataMap.hpp
    I_ChatChannelDataCollection.hpp
    I_AllowGroupDataMap.hpp
    I_DenyGroupDataCollection.hpp
    I_AllowGroupDataCollection.hpp
    I_AllowGroupDomainObject.hpp
    I_ChatLogDomainObject.hpp
    I_ChatEventDomainObject.hpp
    I_DenyGroupDataMap.hpp
    I_ChatLogDataCollection.hpp
    I_ChatChannelDomainObject.hpp
    I_DenyUserDataCollection.hpp
    I_AllowUserDataMap.hpp
    I_AllowUserDomainObject.hpp
    I_AllowUserDataCollection.hpp
    I_ChatCommandDataMap.hpp
    Configuration.hpp
    I_ChatLogDataMap.hpp
    I_DenyUserDataMap.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ChatModel
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)