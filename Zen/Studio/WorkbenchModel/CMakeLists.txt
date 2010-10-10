cmake_minimum_required (VERSION 2.6) 

project (WorkbenchModel)
set ( PROJECT_PREFIX "Studio/WorkbenchModel" )

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


	
set ( WorkbenchModel_SRC
    I_PropertiesDataMap.hpp
    I_PropertyDataMap.hpp
    I_SpreadSheetRowDomainObject.hpp
    I_SceneModelDataCollection.hpp
    I_ProjectDomainObject.hpp
    I_WorkbenchAttributeDomainObject.hpp
    I_SpreadSheetDocumentDomainObject.hpp
    I_SceneModelDomainObject.hpp
    I_SceneModelDataMap.hpp
    I_SpreadSheetRowDataCollection.hpp
    I_SpreadSheetColumnDomainObject.hpp
    I_PropertiesDomainObject.hpp
    I_ProjectDataCollection.hpp
    I_WorkbenchAttributeDataCollection.hpp
    I_SpreadSheetDocumentDataMap.hpp
    src/WorkbenchAttributeDataMap.hpp
    src/SpreadSheetColumnDataCollection.cpp
    src/SpreadSheetColumnDomainObject.cpp
    src/I_SpreadSheetDocumentDomainObject.cpp
    src/SpreadSheetRowDomainObject.cpp
    src/ProjectDataMap.cpp
    src/I_SpreadSheetColumnDataMap.cpp
    src/I_SpreadSheetCellDomainObject.cpp
    src/I_PropertyDomainObject.cpp
    src/SpreadSheetRowDomainObject.hpp
    src/I_SpreadSheetCellDataCollection.cpp
    src/PropertiesDataMap.hpp
    src/ExplorerNodeDataMap.cpp
    src/SpreadSheetRowDataMap.cpp
    src/I_SceneModelDataMap.cpp
    src/SpreadSheetColumnDomainObject.hpp
    src/ExplorerNodeDataCollection.hpp
    src/I_ExplorerNodeDomainObject.cpp
    src/I_SpreadSheetDocumentDataMap.cpp
    src/SceneModelDataMap.hpp
    src/WorkbenchAttributeDataMap.cpp
    src/SpreadSheetCellDataCollection.cpp
    src/ProjectDomainObject.cpp
    src/PropertiesDomainObject.hpp
    src/ProjectDataCollection.hpp
    src/SpreadSheetDocumentDataCollection.hpp
    src/SpreadSheetDocumentDataCollection.cpp
    src/I_SpreadSheetDocumentDataCollection.cpp
    src/SceneModelDataMap.cpp
    src/WorkbenchAttributeDataCollection.cpp
    src/I_ProjectDomainObject.cpp
    src/SpreadSheetDocumentDomainObject.hpp
    src/SpreadSheetColumnDataMap.cpp
    src/PropertyDomainObject.cpp
    src/I_SpreadSheetRowDataCollection.cpp
    src/PropertyDomainObject.hpp
    src/ProjectDataCollection.cpp
    src/I_ProjectDataMap.cpp
    src/SpreadSheetRowDataCollection.hpp
    src/WorkbenchAttributeDomainObject.cpp
    src/SpreadSheetCellDataCollection.hpp
    src/I_SceneModelDataCollection.cpp
    src/PropertyDataCollection.cpp
    src/I_PropertiesDomainObject.cpp
    src/I_SceneModelDomainObject.cpp
    src/I_PropertyDataCollection.cpp
    src/SpreadSheetColumnDataCollection.hpp
    src/ProjectDataMap.hpp
    src/ProjectDomainObject.hpp
    src/PropertiesDomainObject.cpp
    src/I_ExplorerNodeDataCollection.cpp
    src/I_SpreadSheetRowDataMap.cpp
    src/ExplorerNodeDomainObject.cpp
    src/PropertiesDataMap.cpp
    src/I_SpreadSheetCellDataMap.cpp
    src/SceneModelDomainObject.cpp
    src/PropertyDataCollection.hpp
    src/SceneModelDomainObject.hpp
    src/SpreadSheetDocumentDataMap.hpp
    src/SpreadSheetCellDomainObject.cpp
    src/ExplorerNodeDataMap.hpp
    src/I_SpreadSheetColumnDataCollection.cpp
    src/WorkbenchAttributeDataCollection.hpp
    src/ExplorerNodeDomainObject.hpp
    src/SpreadSheetRowDataCollection.cpp
    src/PropertyDataMap.cpp
    src/I_SpreadSheetRowDomainObject.cpp
    src/ExplorerNodeDataCollection.cpp
    src/I_SpreadSheetColumnDomainObject.cpp
    src/SpreadSheetDocumentDomainObject.cpp
    src/I_PropertiesDataCollection.cpp
    src/SpreadSheetRowDataMap.hpp
    src/PropertiesDataCollection.hpp
    src/WorkbenchAttributeDomainObject.hpp
    src/SpreadSheetColumnDataMap.hpp
    src/SceneModelDataCollection.hpp
    src/I_PropertiesDataMap.cpp
    src/PropertiesDataCollection.cpp
    src/I_PropertyDataMap.cpp
    src/I_WorkBenchAttributeDataCollection.cpp
    src/I_ProjectDataCollection.cpp
    src/PropertyDataMap.hpp
    src/SceneModelDataCollection.cpp
    src/SpreadSheetCellDataMap.cpp
    src/I_WorkBenchAttributeDataMap.cpp
    src/SpreadSheetDocumentDataMap.cpp
    src/SpreadSheetCellDomainObject.hpp
    src/I_ExplorerNodeDataMap.cpp
    src/I_WorkBenchAttributeDomainObject.cpp
    src/SpreadSheetCellDataMap.hpp
    I_SpreadSheetRowDataMap.hpp
    I_PropertyDomainObject.hpp
    I_SpreadSheetDocumentDataCollection.hpp
    I_ExplorerNodeDataMap.hpp
    I_WorkbenchAttributeDataMap.hpp
    I_SpreadSheetColumnDataCollection.hpp
    I_SpreadSheetCellDataCollection.hpp
    I_SpreadSheetColumnDataMap.hpp
    I_ProjectDataMap.hpp
    I_ExplorerNodeDataCollection.hpp
    I_PropertiesDataCollection.hpp
    I_SpreadSheetCellDomainObject.hpp
    I_ExplorerNodeDomainObject.hpp
    Configuration.hpp
    I_PropertyDataCollection.hpp
    I_SpreadSheetCellDataMap.hpp

    )

add_definitions( -DWORKBENCHMODEL_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${LIBXML2_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} )

add_library ( WorkbenchModel ${LIB_TYPE} ${WorkbenchModel_SRC} )

add_dependencies(WorkbenchModel ZenCore ZenEnterprise)

target_link_libraries( WorkbenchModel ZenCore ZenEnterprise )

set_target_properties ( WorkbenchModel 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( WorkbenchModel PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( WorkbenchModel PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    I_PropertiesDataMap.hpp
    I_PropertyDataMap.hpp
    I_SpreadSheetRowDomainObject.hpp
    I_SceneModelDataCollection.hpp
    I_ProjectDomainObject.hpp
    I_WorkbenchAttributeDomainObject.hpp
    I_SpreadSheetDocumentDomainObject.hpp
    I_SceneModelDomainObject.hpp
    I_SceneModelDataMap.hpp
    I_SpreadSheetRowDataCollection.hpp
    I_SpreadSheetColumnDomainObject.hpp
    I_PropertiesDomainObject.hpp
    I_ProjectDataCollection.hpp
    I_WorkbenchAttributeDataCollection.hpp
    I_SpreadSheetDocumentDataMap.hpp
    I_SpreadSheetRowDataMap.hpp
    I_PropertyDomainObject.hpp
    I_SpreadSheetDocumentDataCollection.hpp
    I_ExplorerNodeDataMap.hpp
    I_WorkbenchAttributeDataMap.hpp
    I_SpreadSheetColumnDataCollection.hpp
    I_SpreadSheetCellDataCollection.hpp
    I_SpreadSheetColumnDataMap.hpp
    I_ProjectDataMap.hpp
    I_ExplorerNodeDataCollection.hpp
    I_PropertiesDataCollection.hpp
    I_SpreadSheetCellDomainObject.hpp
    I_ExplorerNodeDomainObject.hpp
    Configuration.hpp
    I_PropertyDataCollection.hpp
    I_SpreadSheetCellDataMap.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	WorkbenchModel
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)