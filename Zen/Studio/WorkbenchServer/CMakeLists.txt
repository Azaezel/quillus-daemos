cmake_minimum_required (VERSION 2.6) 

project (WorkbenchServer)
set ( PROJECT_PREFIX "Studio/WorkbenchServer" )

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


	
set ( WorkbenchServer_SRC
    src/WorkbenchService.hpp
    src/ProjectExplorerNode.hpp
    src/RowHeader.hpp
    src/WorkbenchServiceFactory.cpp
    src/ProjectExplorerNode.cpp
    src/SpreadSheetCell.cpp
    src/SceneController.hpp
    src/SpreadSheetCell.hpp
    src/SpreadSheetController.cpp
    src/WorkbenchServiceFactory.hpp
    src/WorkbenchService.cpp
    src/ColumnHeader.cpp
    src/ProjectExplorerModel.hpp
    src/SpreadSheetDocument.hpp
    src/ProjectExplorerModel.cpp
    src/ProjectExplorerController.hpp
    src/SpreadSheetDocument.cpp
    src/RowHeader.cpp
    src/SceneController.cpp
    src/WorkbenchServerPlugin.cpp
    src/ColumnHeader.hpp
    src/SceneModel.hpp
    src/ProjectExplorerController.cpp
    src/SceneModel.cpp
    src/SpreadSheetController.hpp

    )

add_definitions( -DWORKBENCHSERVER_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${LIBXML2_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( WorkbenchServer ${LIB_TYPE} ${WorkbenchServer_SRC} )

add_dependencies(WorkbenchServer ZenCore ZenEnterprise ZenObjectModel WorkbenchCommon WorkbenchModel WorkbenchProtocol )

target_link_libraries( WorkbenchServer ZenCore ZenEnterprise ZenObjectModel  WorkbenchCommon WorkbenchModel WorkbenchProtocol )

set_target_properties ( WorkbenchServer 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( WorkbenchServer PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( WorkbenchServer PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	WorkbenchServer
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
