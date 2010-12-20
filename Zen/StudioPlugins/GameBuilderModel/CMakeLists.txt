cmake_minimum_required (VERSION 2.6) 

project (GameBuilderModel)
set ( PROJECT_PREFIX "StudioPlugins/GameBuilderModel" )

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


	
set ( GameBuilderModel_SRC
    Configuration.hpp
    I_GameObjectElementDataCollection.hpp
    I_GameObjectElementDataMap.hpp
    I_GameObjectElementDomainObject.hpp
    I_GameObjectFolderDataCollection.hpp
    I_GameObjectFolderDataMap.hpp
    I_GameObjectFolderDomainObject.hpp
    I_GameObjectTypeDataCollection.hpp
    I_GameObjectTypeDataMap.hpp
    I_GameObjectTypeDomainObject.hpp
    src/GameObjectElementDataCollection.cpp
    src/GameObjectElementDataCollection.hpp
    src/GameObjectElementDataMap.cpp
    src/GameObjectElementDataMap.hpp
    src/GameObjectElementDomainObject.cpp
    src/GameObjectElementDomainObject.hpp
    src/GameObjectFolderDataCollection.cpp
    src/GameObjectFolderDataCollection.hpp
    src/GameObjectFolderDataMap.cpp
    src/GameObjectFolderDataMap.hpp
    src/GameObjectFolderDomainObject.cpp
    src/GameObjectFolderDomainObject.hpp
    src/GameObjectTypeDataCollection.cpp
    src/GameObjectTypeDataCollection.hpp
    src/GameObjectTypeDataMap.cpp
    src/GameObjectTypeDataMap.hpp
    src/GameObjectTypeDomainObject.cpp
    src/GameObjectTypeDomainObject.hpp
    src/I_GameObjectElementDataCollection.cpp
    src/I_GameObjectElementDataMap.cpp
    src/I_GameObjectElementDomainObject.cpp
    src/I_GameObjectFolderDataCollection.cpp
    src/I_GameObjectFolderDataMap.cpp
    src/I_GameObjectFolderDomainObject.cpp
    src/I_GameObjectTypeDataCollection.cpp
    src/I_GameObjectTypeDataMap.cpp
    src/I_GameObjectTypeDomainObject.cpp

    )

add_definitions( -DGAMEBUILDERMODEL_EXPORTS  ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( GameBuilderModel ${LIB_TYPE} ${GameBuilderModel_SRC} )

add_dependencies(GameBuilderModel ZenCore ZenEngine ZenEnterprise GameBuilderCommon)

target_link_libraries( GameBuilderModel ZenCore ZenEngine ZenEnterprise GameBuilderCommon ${Boost_LIBRARIES} )

set_target_properties ( GameBuilderModel 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( GameBuilderModel PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( GameBuilderModel PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_GameObjectElementDataCollection.hpp
    I_GameObjectElementDataMap.hpp
    I_GameObjectElementDomainObject.hpp
    I_GameObjectFolderDataCollection.hpp
    I_GameObjectFolderDataMap.hpp
    I_GameObjectFolderDomainObject.hpp
    I_GameObjectTypeDataCollection.hpp
    I_GameObjectTypeDataMap.hpp
    I_GameObjectTypeDomainObject.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	GameBuilderModel
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)