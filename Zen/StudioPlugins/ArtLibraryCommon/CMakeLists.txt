cmake_minimum_required (VERSION 2.6) 

project (ArtLibraryCommon)
set ( PROJECT_PREFIX "StudioPlugins/ArtLibraryCommon" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )
Find_Package(LibXml2 REQUIRED)

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include various modules of ArtLibraryCommon

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( ArtLibraryCommon_SRC
    Configuration.hpp
    I_ArtAssetController.hpp
    I_ArtAssetDocument.hpp
    I_ArtAssetExplorerNode.hpp
    I_ArtAssetView.hpp
    I_ArtLibraryService.hpp
    I_Folder.hpp
    I_Project.hpp
    I_ProjectListener.hpp
    I_ProjectService.hpp
    src/I_ArtAssetController.cpp
    src/I_ArtAssetDocument.cpp
    src/I_ArtAssetExplorerNode.cpp
    src/I_ArtAssetView.cpp
    src/I_ArtLibraryService.cpp
    src/I_Folder.cpp
    src/I_Project.cpp
    src/I_ProjectListener.cpp
    src/I_ProjectService.cpp

    )

add_definitions( -DARTLIBRARYCOMMON_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${LIBXML2_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} ${wxWidgets_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS} )

add_library ( ArtLibraryCommon ${LIB_TYPE} ${ArtLibraryCommon_SRC} )

add_dependencies(ArtLibraryCommon ZenCore ZenEnterprise WorkbenchCommon ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES} )

target_link_libraries( ArtLibraryCommon WorkbenchCommon )

set_target_properties ( ArtLibraryCommon 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ArtLibraryCommon PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ArtLibraryCommon PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_ArtAssetController.hpp
    I_ArtAssetDocument.hpp
    I_ArtAssetExplorerNode.hpp
    I_ArtAssetView.hpp
    I_ArtLibraryService.hpp
    I_Folder.hpp
    I_Project.hpp
    I_ProjectListener.hpp
    I_ProjectService.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ArtLibraryCommon
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)