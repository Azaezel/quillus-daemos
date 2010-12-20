cmake_minimum_required (VERSION 2.6) 

project (TerrainBuilderCommon)
set ( PROJECT_PREFIX "StudioPlugins/TerrainBuilderCommon" )

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


	
set ( TerrainBuilderCommon_SRC
    Configuration.hpp
    I_Folder.hpp
    I_Project.hpp
    I_ProjectListener.hpp
    I_ProjectService.hpp
    I_TerrainBuilderService.hpp
    I_TerrainChunkController.hpp
    I_TerrainChunkDocument.hpp
    I_TerrainChunkNode.hpp
    I_TerrainChunkView.hpp
    I_TerrainGroupController.hpp
    I_TerrainGroupDocument.hpp
    I_TerrainGroupNode.hpp
    I_TerrainGroupView.hpp
    src/I_Folder.cpp
    src/I_Project.cpp
    src/I_ProjectListener.cpp
    src/I_ProjectService.cpp
    src/I_TerrainBuilderService.cpp
    src/I_TerrainChunkController.cpp
    src/I_TerrainChunkDocument.cpp
    src/I_TerrainChunkNode.cpp
    src/I_TerrainChunkView.cpp
    src/I_TerrainGroupController.cpp
    src/I_TerrainGroupDocument.cpp
    src/I_TerrainGroupNode.cpp
    src/I_TerrainGroupView.cpp

    )

add_definitions( -DTERRAINBUILDERCOMMON_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( TerrainBuilderCommon ${LIB_TYPE} ${TerrainBuilderCommon_SRC} )

add_dependencies(TerrainBuilderCommon ZenCore ZenEnterprise WorkbenchCommon ${Boost_LIBRARIES} )

target_link_libraries( TerrainBuilderCommon ZenCore ZenEnterprise WorkbenchCommon ${Boost_LIBRARIES})

set_target_properties ( TerrainBuilderCommon 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( TerrainBuilderCommon PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( TerrainBuilderCommon PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_Folder.hpp
    I_Project.hpp
    I_ProjectListener.hpp
    I_ProjectService.hpp
    I_TerrainBuilderService.hpp
    I_TerrainChunkController.hpp
    I_TerrainChunkDocument.hpp
    I_TerrainChunkNode.hpp
    I_TerrainChunkView.hpp
    I_TerrainGroupController.hpp
    I_TerrainGroupDocument.hpp
    I_TerrainGroupNode.hpp
    I_TerrainGroupView.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	TerrainBuilderCommon
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)