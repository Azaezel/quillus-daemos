cmake_minimum_required (VERSION 2.6) 

project (ArtLibraryModel)
set ( PROJECT_PREFIX "StudioPlugins/ArtLibraryModel" )

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


	
set ( ArtLibraryModel_SRC
    Configuration.hpp
    I_ArtAssetDocumentDataCollection.hpp
    I_ArtAssetDocumentDataMap.hpp
    I_ArtAssetDocumentDomainObject.hpp
    I_ArtAssetExplorerNodeDataCollection.hpp
    I_ArtAssetExplorerNodeDataMap.hpp
    I_ArtAssetExplorerNodeDomainObject.hpp
    I_ArtFolderDataCollection.hpp
    I_ArtFolderDataMap.hpp
    I_ArtFolderDomainObject.hpp
    src/ArtAssetDocumentDataCollection.cpp
    src/ArtAssetDocumentDataCollection.hpp
    src/ArtAssetDocumentDataMap.cpp
    src/ArtAssetDocumentDataMap.hpp
    src/ArtAssetDocumentDomainObject.cpp
    src/ArtAssetDocumentDomainObject.hpp
    src/ArtAssetExplorerNodeDataCollection.cpp
    src/ArtAssetExplorerNodeDataCollection.hpp
    src/ArtAssetExplorerNodeDataMap.cpp
    src/ArtAssetExplorerNodeDataMap.hpp
    src/ArtAssetExplorerNodeDomainObject.cpp
    src/ArtAssetExplorerNodeDomainObject.hpp
    src/ArtFolderDataCollection.cpp
    src/ArtFolderDataCollection.hpp
    src/ArtFolderDataMap.cpp
    src/ArtFolderDataMap.hpp
    src/ArtFolderDomainObject.cpp
    src/ArtFolderDomainObject.hpp
    src/I_ArtAssetDocumentDataCollection.cpp
    src/I_ArtAssetDocumentDataMap.cpp
    src/I_ArtAssetDocumentDomainObject.cpp
    src/I_ArtAssetExplorerNodeDataCollection.cpp
    src/I_ArtAssetExplorerNodeDataMap.cpp
    src/I_ArtAssetExplorerNodeDomainObject.cpp
    src/I_ArtFolderDataCollection.cpp
    src/I_ArtFolderDataMap.cpp
    src/I_ArtFolderDomainObject.cpp

    )

add_definitions( -DARTLIBRARYMODEL_EXPORTS  ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ArtLibraryModel ${LIB_TYPE} ${ArtLibraryModel_SRC} )

add_dependencies(ArtLibraryModel ZenCore ZenEngine ZenEnterprise ArtLibraryCommon)

target_link_libraries( ArtLibraryModel ${Boost_LIBRARIES} )

set_target_properties ( ArtLibraryModel 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ArtLibraryModel PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ArtLibraryModel PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_ArtAssetDocumentDataCollection.hpp
    I_ArtAssetDocumentDataMap.hpp
    I_ArtAssetDocumentDomainObject.hpp
    I_ArtAssetExplorerNodeDataCollection.hpp
    I_ArtAssetExplorerNodeDataMap.hpp
    I_ArtAssetExplorerNodeDomainObject.hpp
    I_ArtFolderDataCollection.hpp
    I_ArtFolderDataMap.hpp
    I_ArtFolderDomainObject.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ArtLibraryModel
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)