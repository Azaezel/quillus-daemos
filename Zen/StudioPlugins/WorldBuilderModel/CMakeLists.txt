cmake_minimum_required (VERSION 2.6) 

project (WorldBuilderModel)
set ( PROJECT_PREFIX "StudioPlugins/WorldBuilderModel" )

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


	
set ( WorldBuilderModel_SRC
    Configuration.hpp
    I_ZoneDataCollection.hpp
    I_ZoneDataMap.hpp
    I_ZoneDomainObject.hpp
    I_ZoneFolderDataCollection.hpp
    I_ZoneFolderDataMap.hpp
    I_ZoneFolderDomainObject.hpp
    src/I_ZoneDataCollection.cpp
    src/I_ZoneDataMap.cpp
    src/I_ZoneDomainObject.cpp
    src/I_ZoneFolderDataCollection.cpp
    src/I_ZoneFolderDataMap.cpp
    src/I_ZoneFolderDomainObject.cpp
    src/ZoneDataCollection.cpp
    src/ZoneDataCollection.hpp
    src/ZoneDataMap.cpp
    src/ZoneDataMap.hpp
    src/ZoneDomainObject.cpp
    src/ZoneDomainObject.hpp
    src/ZoneFolderDataCollection.cpp
    src/ZoneFolderDataCollection.hpp
    src/ZoneFolderDataMap.cpp
    src/ZoneFolderDataMap.hpp
    src/ZoneFolderDomainObject.cpp
    src/ZoneFolderDomainObject.hpp

    )

add_definitions( -DWORLDBUILDERMODEL_EXPORTS  ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT}  )

link_directories ( ${DEV_LIB} ${wxWidgets_LIBRARY_DIRS} ${Boost_LIBRARY_DIRS} )

add_library ( WorldBuilderModel ${LIB_TYPE} ${WorldBuilderModel_SRC} )

add_dependencies(WorldBuilderModel ZenCore ZenEngine ZenEnterprise )

target_link_libraries( WorldBuilderModel WorkbenchCommon ${Boost_LIBRARIES} )

set_target_properties ( WorldBuilderModel 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( WorldBuilderModel PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( WorldBuilderModel PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_ZoneDataCollection.hpp
    I_ZoneDataMap.hpp
    I_ZoneDomainObject.hpp
    I_ZoneFolderDataCollection.hpp
    I_ZoneFolderDataMap.hpp
    I_ZoneFolderDomainObject.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	WorldBuilderModel
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)