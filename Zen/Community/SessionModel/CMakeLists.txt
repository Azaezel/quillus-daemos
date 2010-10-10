cmake_minimum_required (VERSION 2.6) 

project (SessionModel)
set ( PROJECT_PREFIX "Community/SessionModel" )

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


	
set ( SessionModel_SRC
    I_AccountDomainObject.hpp
    src/AccountDomainObject.cpp
    src/AccountDataMap.hpp
    src/AccountDataCollection.hpp
    src/I_AccountDomainObject.cpp
    src/AccountDomainObject.hpp
    src/I_AccountDataCollection.cpp
    src/AccountDataMap.cpp
    src/I_AccountDataMap.cpp
    src/AccountDataCollection.cpp
    I_AccountDataCollection.hpp
    I_AccountDataMap.hpp
    Configuration.hpp

    )

add_definitions( -DSESSIONMODEL_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${LIBXML2_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} )

add_library ( SessionModel ${LIB_TYPE} ${SessionModel_SRC} )

add_dependencies(SessionModel ZenCore ZenEnterprise)

target_link_libraries( SessionModel ZenCore ZenEnterprise )

set_target_properties ( SessionModel 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( SessionModel PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( SessionModel PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    I_AccountDomainObject.hpp
    I_AccountDataCollection.hpp
    I_AccountDataMap.hpp
    Configuration.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	SessionModel
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)