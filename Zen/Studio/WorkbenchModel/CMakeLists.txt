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
    Configuration.hpp
    I_ExplorerNodeDataCollection.hpp
    I_ExplorerNodeDataMap.hpp
    I_ExplorerNodeDomainObject.hpp
    I_ProjectDataCollection.hpp
    I_ProjectDataMap.hpp
    I_ProjectDomainObject.hpp
    I_PropertiesDataCollection.hpp
    I_PropertiesDataMap.hpp
    I_PropertiesDomainObject.hpp
    I_PropertyDataCollection.hpp
    I_PropertyDataMap.hpp
    I_PropertyDomainObject.hpp
    I_SceneModelDataCollection.hpp
    I_SceneModelDataMap.hpp
    I_SceneModelDomainObject.hpp
    I_SpreadSheetCellDataCollection.hpp
    I_SpreadSheetCellDataMap.hpp
    I_SpreadSheetCellDomainObject.hpp
    I_SpreadSheetColumnDataCollection.hpp
    I_SpreadSheetColumnDataMap.hpp
    I_SpreadSheetColumnDomainObject.hpp
    I_SpreadSheetDocumentDataCollection.hpp
    I_SpreadSheetDocumentDataMap.hpp
    I_SpreadSheetDocumentDomainObject.hpp
    I_SpreadSheetRowDataCollection.hpp
    I_SpreadSheetRowDataMap.hpp
    I_SpreadSheetRowDomainObject.hpp
    I_WorkbenchAttributeDataCollection.hpp
    I_WorkbenchAttributeDataMap.hpp
    I_WorkbenchAttributeDomainObject.hpp
    src/ExplorerNodeDataCollection.cpp
    src/ExplorerNodeDataCollection.hpp
    src/ExplorerNodeDataMap.cpp
    src/ExplorerNodeDataMap.hpp
    src/ExplorerNodeDomainObject.cpp
    src/ExplorerNodeDomainObject.hpp
    src/I_ExplorerNodeDataCollection.cpp
    src/I_ExplorerNodeDataMap.cpp
    src/I_ExplorerNodeDomainObject.cpp
    src/I_ProjectDataCollection.cpp
    src/I_ProjectDataMap.cpp
    src/I_ProjectDomainObject.cpp
    src/I_PropertiesDataCollection.cpp
    src/I_PropertiesDataMap.cpp
    src/I_PropertiesDomainObject.cpp
    src/I_PropertyDataCollection.cpp
    src/I_PropertyDataMap.cpp
    src/I_PropertyDomainObject.cpp
    src/I_SceneModelDataCollection.cpp
    src/I_SceneModelDataMap.cpp
    src/I_SceneModelDomainObject.cpp
    src/I_SpreadSheetCellDataCollection.cpp
    src/I_SpreadSheetCellDataMap.cpp
    src/I_SpreadSheetCellDomainObject.cpp
    src/I_SpreadSheetColumnDataCollection.cpp
    src/I_SpreadSheetColumnDataMap.cpp
    src/I_SpreadSheetColumnDomainObject.cpp
    src/I_SpreadSheetDocumentDataCollection.cpp
    src/I_SpreadSheetDocumentDataMap.cpp
    src/I_SpreadSheetDocumentDomainObject.cpp
    src/I_SpreadSheetRowDataCollection.cpp
    src/I_SpreadSheetRowDataMap.cpp
    src/I_SpreadSheetRowDomainObject.cpp
    src/I_WorkBenchAttributeDataCollection.cpp
    src/I_WorkBenchAttributeDataMap.cpp
    src/I_WorkBenchAttributeDomainObject.cpp
    src/ProjectDataCollection.cpp
    src/ProjectDataCollection.hpp
    src/ProjectDataMap.cpp
    src/ProjectDataMap.hpp
    src/ProjectDomainObject.cpp
    src/ProjectDomainObject.hpp
    src/PropertiesDataCollection.cpp
    src/PropertiesDataCollection.hpp
    src/PropertiesDataMap.cpp
    src/PropertiesDataMap.hpp
    src/PropertiesDomainObject.cpp
    src/PropertiesDomainObject.hpp
    src/PropertyDataCollection.cpp
    src/PropertyDataCollection.hpp
    src/PropertyDataMap.cpp
    src/PropertyDataMap.hpp
    src/PropertyDomainObject.cpp
    src/PropertyDomainObject.hpp
    src/SceneModelDataCollection.cpp
    src/SceneModelDataCollection.hpp
    src/SceneModelDataMap.cpp
    src/SceneModelDataMap.hpp
    src/SceneModelDomainObject.cpp
    src/SceneModelDomainObject.hpp
    src/SpreadSheetCellDataCollection.cpp
    src/SpreadSheetCellDataCollection.hpp
    src/SpreadSheetCellDataMap.cpp
    src/SpreadSheetCellDataMap.hpp
    src/SpreadSheetCellDomainObject.cpp
    src/SpreadSheetCellDomainObject.hpp
    src/SpreadSheetColumnDataCollection.cpp
    src/SpreadSheetColumnDataCollection.hpp
    src/SpreadSheetColumnDataMap.cpp
    src/SpreadSheetColumnDataMap.hpp
    src/SpreadSheetColumnDomainObject.cpp
    src/SpreadSheetColumnDomainObject.hpp
    src/SpreadSheetDocumentDataCollection.cpp
    src/SpreadSheetDocumentDataCollection.hpp
    src/SpreadSheetDocumentDataMap.cpp
    src/SpreadSheetDocumentDataMap.hpp
    src/SpreadSheetDocumentDomainObject.cpp
    src/SpreadSheetDocumentDomainObject.hpp
    src/SpreadSheetRowDataCollection.cpp
    src/SpreadSheetRowDataCollection.hpp
    src/SpreadSheetRowDataMap.cpp
    src/SpreadSheetRowDataMap.hpp
    src/SpreadSheetRowDomainObject.cpp
    src/SpreadSheetRowDomainObject.hpp
    src/WorkbenchAttributeDataCollection.cpp
    src/WorkbenchAttributeDataCollection.hpp
    src/WorkbenchAttributeDataMap.cpp
    src/WorkbenchAttributeDataMap.hpp
    src/WorkbenchAttributeDomainObject.cpp
    src/WorkbenchAttributeDomainObject.hpp

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
    Configuration.hpp
    I_ExplorerNodeDataCollection.hpp
    I_ExplorerNodeDataMap.hpp
    I_ExplorerNodeDomainObject.hpp
    I_ProjectDataCollection.hpp
    I_ProjectDataMap.hpp
    I_ProjectDomainObject.hpp
    I_PropertiesDataCollection.hpp
    I_PropertiesDataMap.hpp
    I_PropertiesDomainObject.hpp
    I_PropertyDataCollection.hpp
    I_PropertyDataMap.hpp
    I_PropertyDomainObject.hpp
    I_SceneModelDataCollection.hpp
    I_SceneModelDataMap.hpp
    I_SceneModelDomainObject.hpp
    I_SpreadSheetCellDataCollection.hpp
    I_SpreadSheetCellDataMap.hpp
    I_SpreadSheetCellDomainObject.hpp
    I_SpreadSheetColumnDataCollection.hpp
    I_SpreadSheetColumnDataMap.hpp
    I_SpreadSheetColumnDomainObject.hpp
    I_SpreadSheetDocumentDataCollection.hpp
    I_SpreadSheetDocumentDataMap.hpp
    I_SpreadSheetDocumentDomainObject.hpp
    I_SpreadSheetRowDataCollection.hpp
    I_SpreadSheetRowDataMap.hpp
    I_SpreadSheetRowDomainObject.hpp
    I_WorkbenchAttributeDataCollection.hpp
    I_WorkbenchAttributeDataMap.hpp
    I_WorkbenchAttributeDomainObject.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	WorkbenchModel
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)