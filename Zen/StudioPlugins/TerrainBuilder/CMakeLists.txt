cmake_minimum_required (VERSION 2.6) 

project (TerrainBuilder)
set ( PROJECT_PREFIX "StudioPlugins/TerrainBuilder" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED regex )
Find_Package(wxWidgets REQUIRED)
Find_Package(OGRE REQUIRED)

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of Zen Engine

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( TerrainBuilder_SRC
    src/TerrainGroupNodeContributor.cpp
    src/ContributorService.hpp
    src/ContributorService.cpp
    src/ContributorServiceFactory.hpp
    src/TerrainChunkNodeContributor.cpp
    src/TerrainBuilderPlugin.cpp
    src/TerrainChunkNodeContributor.hpp
    src/ContributorServiceFactory.cpp
    src/FolderContributor.hpp
    src/FolderContributor.cpp
    src/TerrainGroupNodeContributor.hpp
    Configuration.hpp

    )

add_definitions( -DTERRAINBUILDER_EXPORTS ${wxWidgets_DEFINITIONS} ${wxWidgets_CXX_FLAGS} ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${wxWidgets_INCLUDE_DIRS} ${OGRE_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} ${wxWidgets_LIB_DIR} ${Boost_LIBRARY_DIRS} )

add_library ( TerrainBuilder ${LIB_TYPE} ${TerrainBuilder_SRC} )

add_dependencies(TerrainBuilder ZenCore ZenEngine ZenEnterprise Workbench WorkbenchCommon TerrainBuilderCommon ZOgre )

target_link_libraries( TerrainBuilder ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES} ZenCore ZenEngine ZenEnterprise Workbench WorkbenchCommon TerrainBuilderCommon ZOgre ${OGRE_LIBRARIES})

create_zenstudio_vcproj_userfile(TerrainBuilder)

set_target_properties ( TerrainBuilder 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( TerrainBuilder PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( TerrainBuilder PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    plugin.xml
    Configuration.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	TerrainBuilder
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
