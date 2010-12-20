cmake_minimum_required (VERSION 2.6) 

project (TerrainBuilderServer)
set ( PROJECT_PREFIX "StudioPlugins/TerrainBuilderServer" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )
Find_Package(wxWidgets REQUIRED)

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of Zen Engine

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( TerrainBuilderServer_SRC
    src/ExplorerNodeFactory.cpp
    src/ExplorerNodeFactory.hpp
    src/Folder.cpp
    src/Folder.hpp
    src/Project.cpp
    src/Project.hpp
    src/ProjectService.cpp
    src/ProjectService.hpp
    src/ProjectServiceFactory.cpp
    src/ProjectServiceFactory.hpp
    src/Properties.cpp
    src/Properties.hpp
    src/Property.cpp
    src/Property.hpp
    src/TerrainBuilderPlugin.cpp
    src/TerrainChunkController.cpp
    src/TerrainChunkController.hpp
    src/TerrainChunkDocument.cpp
    src/TerrainChunkDocument.hpp
    src/TerrainChunkNode.cpp
    src/TerrainChunkNode.hpp
    src/TerrainGroupController.cpp
    src/TerrainGroupController.hpp
    src/TerrainGroupDocument.cpp
    src/TerrainGroupDocument.hpp
    src/TerrainGroupNode.cpp
    src/TerrainGroupNode.hpp

    )

add_definitions( -DTERRAINBUILDERSERVER_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT}  )

link_directories ( ${DEV_LIB} ${wxWidgets_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS} )

add_library ( TerrainBuilderServer ${LIB_TYPE} ${TerrainBuilderServer_SRC} )

add_dependencies(TerrainBuilderServer ZenCore ZenEngine ZenEnterprise ZenObjectModel WorkbenchCommon WorkbenchModel TerrainBuilderCommon TerrainBuilderModel  )

target_link_libraries( TerrainBuilderServer ZenCore ZenEngine ZenEnterprise ZenObjectModel WorkbenchCommon WorkbenchModel TerrainBuilderCommon TerrainBuilderModel ${Boost_LIBRARIES} )

set_target_properties ( TerrainBuilderServer 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( TerrainBuilderServer PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( TerrainBuilderServer PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	TerrainBuilderServer
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
