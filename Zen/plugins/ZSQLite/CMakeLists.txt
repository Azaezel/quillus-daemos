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
    src/DatabaseColumn.cpp
    src/DatabaseColumn.hpp
    src/DatabaseCommand.cpp
    src/DatabaseCommand.hpp
    src/DatabaseConnection.cpp
    src/DatabaseConnection.hpp
    src/DatabaseDynamicQuery.cpp
    src/DatabaseDynamicQuery.hpp
    src/DatabaseQuery.cpp
    src/DatabaseQuery.hpp
    src/DatabaseResult.cpp
    src/DatabaseResult.hpp
    src/DatabaseRow.cpp
    src/DatabaseRow.hpp
    src/DatabaseService.cpp
    src/DatabaseService.hpp
    src/DatabaseServiceFactory.cpp
    src/DatabaseServiceFactory.hpp
    src/DatabaseStaticQuery.cpp
    src/DatabaseStaticQuery.hpp
    src/DatabaseTransaction.cpp
    src/DatabaseTransaction.hpp
    src/sqlite.hpp
    src/sqlite3.c
    src/sqlite3.h
    src/sqlite3ext.h
    src/SQLiteTypes.cpp
    src/SQLiteTypes.hpp
    src/ZMySQLPlugin.cpp

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