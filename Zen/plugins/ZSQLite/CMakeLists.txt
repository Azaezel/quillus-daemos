cmake_minimum_required (VERSION 2.6) 

project (ZSQLite)
set ( PROJECT_PREFIX "plugins/ZSQLite" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include ZSQLite source file

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( ZSQLite_SRC
    src/DatabaseServiceFactory.cpp
    src/DatabaseColumn.hpp
    src/DatabaseCommand.cpp
    src/DatabaseStaticQuery.hpp
    src/sqlite.hpp
    src/DatabaseConnection.hpp
    src/sqlite3ext.h
    src/DatabaseResult.cpp
    src/DatabaseDynamicQuery.hpp
    src/DatabaseStaticQuery.cpp
    src/DatabaseCommand.hpp
    src/DatabaseDynamicQuery.cpp
    src/DatabaseTransaction.cpp
    src/DatabaseService.cpp
    src/DatabaseRow.cpp
    src/DatabaseQuery.cpp
    src/DatabaseResult.hpp
    src/sqlite3.h
    src/DatabaseRow.hpp
    src/ZMySQLPlugin.cpp
    src/sqlite3.c
    src/DatabaseService.hpp
    src/DatabaseColumn.cpp
    src/DatabaseConnection.cpp
    src/DatabaseTransaction.hpp
    src/SQLiteTypes.hpp
    src/DatabaseQuery.hpp
    src/SQLiteTypes.cpp
    src/DatabaseServiceFactory.hpp

    )

add_definitions( -DZSQLITE_EXPORTS -DSQLITE_ENABLE_COLUMN_METADATA ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT}  )

link_directories ( ${DEV_LIB} )

add_library ( ZSQLite ${LIB_TYPE} ${ZSQLite_SRC} )

add_dependencies(ZSQLite ZenCore ZenEnterprise )

target_link_libraries( ZSQLite ZenEnterprise )

set_target_properties ( ZSQLite 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZSQLite PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZSQLite PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZSQLite
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)