cmake_minimum_required (VERSION 2.6) 

project (TerrainBuilderProtocol)
set ( PROJECT_PREFIX "StudioPlugins/TerrainBuilderProtocol" )

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


	
set ( TerrainBuilderProtocol_SRC

    )

add_definitions( -DTERRAINBUILDERPROTOCOL_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( TerrainBuilderProtocol ${LIB_TYPE} ${TerrainBuilderProtocol_SRC} )

add_dependencies(TerrainBuilderProtocol ZenCore ZenEngine ZenEnterprise WorkbenchCommon )

target_link_libraries( TerrainBuilderProtocol WorkbenchCommon ${Boost_LIBRARIES} )

set_target_properties ( TerrainBuilderProtocol 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( TerrainBuilderProtocol PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( TerrainBuilderProtocol PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)



install ( TARGETS
	TerrainBuilderProtocol
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)