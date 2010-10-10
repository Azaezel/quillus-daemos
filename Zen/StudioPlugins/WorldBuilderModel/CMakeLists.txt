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
    I_ZoneFolderDomainObject.hpp
    I_ZoneDataCollection.hpp
    I_ZoneDomainObject.hpp
    src/ZoneFolderDataMap.cpp
    src/ZoneDataMap.hpp
    src/I_ZoneDataMap.cpp
    src/ZoneDomainObject.hpp
    src/ZoneFolderDataMap.hpp
    src/I_ZoneFolderDataCollection.cpp
    src/ZoneFolderDomainObject.hpp
    src/ZoneDomainObject.cpp
    src/ZoneFolderDataCollection.hpp
    src/ZoneDataCollection.hpp
    src/ZoneFolderDomainObject.cpp
    src/I_ZoneDomainObject.cpp
    src/I_ZoneFolderDomainObject.cpp
    src/I_ZoneFolderDataMap.cpp
    src/ZoneDataMap.cpp
    src/ZoneFolderDataCollection.cpp
    src/ZoneDataCollection.cpp
    src/I_ZoneDataCollection.cpp
    I_ZoneDataMap.hpp
    Configuration.hpp
    I_ZoneFolderDataMap.hpp
    I_ZoneFolderDataCollection.hpp

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
    I_ZoneFolderDomainObject.hpp
    I_ZoneDataCollection.hpp
    I_ZoneDomainObject.hpp
    I_ZoneDataMap.hpp
    Configuration.hpp
    I_ZoneFolderDataMap.hpp
    I_ZoneFolderDataCollection.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	WorldBuilderModel
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)