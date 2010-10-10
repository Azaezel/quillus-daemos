cmake_minimum_required (VERSION 2.6) 

project (ArtLibraryServer)
set ( PROJECT_PREFIX "StudioPlugins/ArtLibraryServer" )

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


	
set ( ArtLibraryServer_SRC
    src/ExplorerNodeFactory.cpp
    src/ArtLibraryPlugin.cpp
    src/ArtAssetController.cpp
    src/ArtFolder.hpp
    src/ArtAssetExplorerNode.hpp
    src/ArtFolder.cpp
    src/ArtAssetDocument.hpp
    src/ArtAssetDocument.cpp
    src/ArtAssetExplorerNode.cpp
    src/ExplorerNodeFactory.hpp
    src/ArtAssetController.hpp

    )

add_definitions( -DARTLIBRARYSERVER_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${wxWidgets_LIB_DIR} ${Boost_LIBRARY_DIRS} )

add_library ( ArtLibraryServer ${LIB_TYPE} ${ArtLibraryServer_SRC} )

add_dependencies(ArtLibraryServer ZenCore ZenEngine ZenEnterprise ZenObjectModel WorkbenchCommon WorkbenchModel ArtLibraryCommon ArtLibraryModel )

target_link_libraries( ArtLibraryServer ZenCore ZenEngine ZenEnterprise ZenObjectModel WorkbenchCommon WorkbenchModel ArtLibraryCommon ArtLibraryModel ${Boost_LIBRARIES} )

set_target_properties ( ArtLibraryServer 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ArtLibraryServer PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ArtLibraryServer PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ArtLibraryServer
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)