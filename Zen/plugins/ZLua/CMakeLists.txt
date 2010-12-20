cmake_minimum_required (VERSION 2.6) 

project (ZLua)
set ( PROJECT_PREFIX "plugins/ZLua" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include ZLua source file

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)

source_group(
    src\\functions\\
	REGULAR_EXPRESSION 
	src/functions/*.*
	)


	
set ( ZLua_SRC
    I_LuaScriptEngine.hpp
    src/Config.cpp
    src/Config.hpp
    src/functions/BoolFunctionArgs.cpp
    src/functions/BoolFunctionNoArgs.cpp
    src/functions/GenericFunction.cpp
    src/functions/IntFunctionArgs.cpp
    src/functions/IntFunctionNoArgs.cpp
    src/functions/ObjFunctionArgs.cpp
    src/functions/ObjFunctionNoArgs.cpp
    src/functions/StringFunctionArgs.cpp
    src/functions/StringFunctionNoArgs.cpp
    src/functions/VoidFunctionArgs.cpp
    src/functions/VoidFunctionNoArgs.cpp
    src/I_LuaScriptEngine.cpp
    src/LuaEngine.cpp
    src/LuaEngine.hpp
    src/LuaEngineFactory.cpp
    src/LuaEngineFactory.hpp
    src/LuaHeap.cpp
    src/LuaHeap.hpp
    src/LuaMethod.cpp
    src/LuaMethod.hpp
    src/LuaModule.cpp
    src/LuaModule.hpp
    src/LuaObject.cpp
    src/LuaObject.hpp
    src/LuaType.cpp
    src/LuaType.hpp
    src/LuaTypeMap.cpp
    src/LuaTypeMap.hpp
    src/ZLuaModule.cpp
    src/ZLuaPlugin.cpp
    src/ZLuaPlugin.hpp

    )

add_definitions( -DZLUA_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${LUA_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( ZLua ${LIB_TYPE} ${ZLua_SRC} )

add_dependencies( ZLua ZenCore )

target_link_libraries( ZLua ZenCore ${LUA_LIBRARY} )

set_target_properties ( ZLua 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZLua PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZLua PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    I_LuaScriptEngine.hpp
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZLua
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)