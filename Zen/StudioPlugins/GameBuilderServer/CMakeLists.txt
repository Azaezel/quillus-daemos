cmake_minimum_required (VERSION 2.6) 

project (GameBuilderServer)
set ( PROJECT_PREFIX "StudioPlugins/GameBuilderServer" )

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


	
set ( GameBuilderServer_SRC
    src/ElementProperties.hpp
    src/PersistenceProperty.hpp
    src/GameObjectTypeDocument.hpp
    src/GameObjectTypeController.hpp
    src/DocumentProperties.cpp
    src/GameObjectType.cpp
    src/TextCellData.cpp
    src/ExplorerNodeFactory.cpp
    src/PropertyGroup.cpp
    src/ReplicationPropertyGroup.cpp
    src/Properties.hpp
    src/GameObjectTypeDocument.cpp
    src/Project.hpp
    src/ExtensionPointProperty.hpp
    src/CreatorCategoryProperty.cpp
    src/PersistenceProperty.cpp
    src/ExtensionPointProperty.cpp
    src/GameBuilderPlugin.cpp
    src/Folder.hpp
    src/GameObjectType.hpp
    src/ProjectServiceFactory.hpp
    src/TextCell.hpp
    src/Project.cpp
    src/Property.hpp
    src/CreatorCategoryProperty.hpp
    src/CreatorIconProperty.cpp
    src/GameObjectElement.hpp
    src/ProjectService.cpp
    src/GameObjectElement.cpp
    src/ReplicationDirectionProperty.hpp
    src/CreatorPropertyGroup.hpp
    src/CreatorIconProperty.hpp
    src/PropertyGroup.hpp
    src/TextCell.cpp
    src/Folder.cpp
    src/ElementProperties.cpp
    src/GameObjectTypeController.cpp
    src/ReplicationTypeProperty.hpp
    src/TextCellData.hpp
    src/ProjectServiceFactory.cpp
    src/Property.cpp
    src/Properties.cpp
    src/ReplicationDirectionProperty.cpp
    src/ProjectService.hpp
    src/DocumentProperties.hpp
    src/CreatorPropertyGroup.cpp
    src/ReplicationTypeProperty.cpp
    src/ExplorerNodeFactory.hpp
    src/ReplicationPropertyGroup.hpp

    )

add_definitions( -DGAMEBUILDERSERVER_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${wxWidgets_LIB_DIR} ${Boost_LIBRARY_DIRS} )

add_library ( GameBuilderServer ${LIB_TYPE} ${GameBuilderServer_SRC} )

add_dependencies(GameBuilderServer ZenCore ZenEngine ZenEnterprise ZenObjectModel WorkbenchCommon WorkbenchModel GameBuilderCommon GameBuilderModel )

target_link_libraries( GameBuilderServer ZenCore ZenEngine ZenEnterprise ZenObjectModel WorkbenchCommon WorkbenchModel GameBuilderCommon GameBuilderModel ${Boost_LIBRARIES} )

create_zenstudio_vcproj_userfile(GameBuilderServer)

set_target_properties ( GameBuilderServer 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( GameBuilderServer PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( GameBuilderServer PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	GameBuilderServer
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)