cmake_minimum_required (VERSION 2.6) 

project (WorkbenchProtocol)
set ( PROJECT_PREFIX "Studio/WorkbenchProtocol" )

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


	
set ( WorkbenchProtocol_SRC
    Configuration.hpp
    I_ChildNodeRequest.hpp
    I_ChildNodeResponse.hpp
    I_Message.hpp
    I_NewProjectRequest.hpp
    I_NewProjectResponse.hpp
    I_SubscribeModelRequest.hpp
    I_SubscribeModelResponse.hpp
    I_WorkbenchProtocolManager.hpp
    src/ChildNodeRequest.cpp
    src/ChildNodeRequest.hpp
    src/ChildNodeResponse.cpp
    src/ChildNodeResponse.hpp
    src/I_ChildNodeRequest.cpp
    src/I_ChildNodeResponse.cpp
    src/I_Message.cpp
    src/I_NewProjectRequest.cpp
    src/I_NewProjectResponse.cpp
    src/I_SubscribeModelRequest.cpp
    src/I_SubscribeModelResponse.cpp
    src/I_WorkbenchProtocolManager.cpp
    src/Message.cpp
    src/Message.hpp
    src/MessageFactory.cpp
    src/MessageFactory.hpp
    src/NewProjectRequest.cpp
    src/NewProjectRequest.hpp
    src/NewProjectResponse.cpp
    src/NewProjectResponse.hpp
    src/Request.cpp
    src/Request.hpp
    src/Response.cpp
    src/Response.hpp
    src/SubscribeModelRequest.cpp
    src/SubscribeModelRequest.hpp
    src/SubscribeModelResponse.cpp
    src/SubscribeModelResponse.hpp
    src/WorkbenchProtocolManager.cpp
    src/WorkbenchProtocolManager.hpp

    )

add_definitions( -DWORKBENCHPROTOCOL_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${LIBXML2_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} )

add_library ( WorkbenchProtocol ${LIB_TYPE} ${WorkbenchProtocol_SRC} )

add_dependencies(WorkbenchProtocol ZenCore ZenEnterprise)

target_link_libraries( WorkbenchProtocol ZenCore ZenEnterprise )

set_target_properties ( WorkbenchProtocol 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( WorkbenchProtocol PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( WorkbenchProtocol PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_ChildNodeRequest.hpp
    I_ChildNodeResponse.hpp
    I_Message.hpp
    I_NewProjectRequest.hpp
    I_NewProjectResponse.hpp
    I_SubscribeModelRequest.hpp
    I_SubscribeModelResponse.hpp
    I_WorkbenchProtocolManager.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	WorkbenchProtocol
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)