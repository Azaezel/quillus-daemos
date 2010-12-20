cmake_minimum_required (VERSION 2.6)

project (PermissionServer)
set (PROJECT_PREFIX "EnterprisePlugins/Permission/PermissionServer")

if (NOT INDIEZEN_PARENT)
    find_path (INDIEZEN_PARENT Zen .. ../.. ../../.. ../../../.. ENV)
endif (NOT INDIEZEN_PARENT)

set (CMAKE_MODULE_PATH
    "{INDIEZEN_PARENT}/Zen/build/CMake/Modules"
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_MODULE_PATH}
    )
    
Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED)

SET (LIB_TYPE SHARED)

set (PermissionServer_SRC
        Configuration.hpp
    I_PermissionService.hpp
    src/I_PermissionService.cpp
    src/PermissionService.cpp
    src/PermissionService.hpp
    src/PermissionServiceFactory.cpp
    src/PermissionServiceFactory.hpp

    )
    
add_definitions (-DPERMISSION_SERVER_EXPORTS ${Zen_DEFINITIONS})

include_directories (${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT})

link_directories (${DEV_LIB} ${Boost_LIBRARY_DIRS})

add_library (PermissionServer ${LIB_TYPE} ${PermissionServer_SRC})

add_dependencies (PermissionServer ZenCore ZenEnterprise PermissionClient)

target_link_libraries (PermissionServer ZenCore ZenEnterprise PermissionClient ${Boost_LIBRARIES})

set_target_properties (PermissionServer
    PROPERTIES
    DEBUG_POSTFIX "_d"
)

if (MSVC_IDE)
    set_target_properties (PermissionServer PROPERTIES PREFIX "../../bin/")
    set_target_properties (PermissionServer PROPERTIES IMPORT_PREFIX "../../lib/")
endif (MSVC_IDE)


install ( FILES 
    Configuration.hpp
    I_PermissionService.hpp
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	PermissionServer
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
