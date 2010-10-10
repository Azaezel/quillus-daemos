cmake_minimum_required (VERSION 2.6) 

project (GameBuilderProtocol)
set ( PROJECT_PREFIX "StudioPlugins/GameBuilderProtocol" )

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


	
set ( Workbench_SRC
    I_Message.hpp
    I_DeleteChildNodeResponse.hpp
    I_DeleteChildNodeRequest.hpp
    src/GameBuilderProtocolManager.hpp
    src/CreateChildNodeRequest.hpp
    src/CreateChildNodeResponse.cpp
    src/DeleteChildNodeRequest.hpp
    src/DeleteChildNodeResponse.cpp
    src/Request.hpp
    src/I_CreateChildNodeResponse.cpp
    src/DeleteChildNodeResponse.hpp
    src/I_DeleteChildNodeResponse.cpp
    src/Request.cpp
    src/I_CreateChildNodeRequest.cpp
    src/Message.hpp
    src/MessageFactory.cpp
    src/I_Message.cpp
    src/I_GameBuilderProtocolManager.cpp
    src/Message.cpp
    src/I_DeleteChildNodeRequest.cpp
    src/GameBuilderProtocolManager.cpp
    src/Response.cpp
    src/Response.hpp
    src/DeleteChildNodeRequest.cpp
    src/CreateChildNodeRequest.cpp
    src/MessageFactory.hpp
    src/CreateChildNodeResponse.hpp
    I_CreateChildNodeResponse.hpp
    I_GameBuilderProtocolManager.hpp
    I_CreateChildNodeRequest.hpp
    Configuration.hpp

    )

add_definitions( -DGAMEBUILDERPROTOCOL_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library (GameBuilderProtocol ${LIB_TYPE} ${Workbench_SRC} )

add_dependencies(GameBuilderProtocol ZenCore ZenEngine ZenEnterprise WorkbenchCommon)

target_link_libraries( GameBuilderProtocol ZenCore ZenEngine ZenEnterprise WorkbenchCommon ${Boost_LIBRARIES} )

set_target_properties ( GameBuilderProtocol 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( Workbench PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( Workbench PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    I_Message.hpp
    I_DeleteChildNodeResponse.hpp
    I_DeleteChildNodeRequest.hpp
    I_CreateChildNodeResponse.hpp
    I_GameBuilderProtocolManager.hpp
    I_CreateChildNodeRequest.hpp
    Configuration.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	GameBuilderProtocol
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)