cmake_minimum_required (VERSION 2.6)

project (PermissionService)
set (PROJECT_PREFIX "EnterprisePlugins/Permission/PermissionService")

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

set (PermissionService_SRC
        Configuration.hpp
    src/PermissionService.cpp
    src/PermissionService.hpp
    src/PermissionServiceFactory.cpp
    src/PermissionServiceFactory.hpp
    src/PermissionService_gen.cpp

    )
    
add_definitions (-DPERMISSION_SERVICE_EXPORTS ${Zen_DEFINITIONS})

include_directories (${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT})

link_directories (${DEV_LIB} ${Boost_LIBRARY_DIRS})

add_library (PermissionService ${LIB_TYPE} ${PermissionService_SRC})

add_dependencies (PermissionService ZenCore ZenEnterprise PermissionClient PermissionServer)

target_link_libraries (PermissionService ZenCore ZenEnterprise PermissionClient PermissionServer ${Boost_LIBRARIES})

set_target_properties (PermissionService
    PROPERTIES
    DEBUG_POSTFIX "_d"
)

if (MSVC_IDE)
    set_target_properties (PermissionService PROPERTIES PREFIX "../../bin/")
    set_target_properties (PermissionService PROPERTIES IMPORT_PREFIX "../../lib/")
endif (MSVC_IDE)


install ( FILES 
    Configuration.hpp
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	PermissionService
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
