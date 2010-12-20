cmake_minimum_required (VERSION 2.6)

project (PermissionClient)
set (PROJECT_PREFIX "EnterprisePlugins/Permission/PermissionClient")

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

set (PermissionClient_SRC
        Configuration.hpp
    I_PermissionService.hpp
    Protocol/I_PermissionProtocolManager.hpp
    src/I_PermissionService.cpp
    src/PermissionService.cpp
    src/PermissionService.hpp
    src/PermissionServiceFactory.cpp
    src/PermissionServiceFactory.hpp
    src/Protocol/I_PermissionProtocolManager.cpp
    src/Protocol/Message.cpp
    src/Protocol/Message.hpp
    src/Protocol/MessageFactory.cpp
    src/Protocol/MessageFactory.hpp
    src/Protocol/Request.cpp
    src/Protocol/Request.hpp
    src/Protocol/Response.cpp
    src/Protocol/Response.hpp

    )
    
add_definitions (-DPERMISSION_CLIENT_EXPORTS ${Zen_DEFINITIONS})

include_directories (${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT})

link_directories (${DEV_LIB} ${Boost_LIBRARY_DIRS})

add_library (PermissionClient ${LIB_TYPE} ${PermissionClient_SRC})

add_dependencies (PermissionClient ZenCore ZenEnterprise)

target_link_libraries (PermissionClient ZenCore ZenEnterprise ${Boost_LIBRARIES})

set_target_properties (PermissionClient
    PROPERTIES
    DEBUG_POSTFIX "_d"
)

if (MSVC_IDE)
    set_target_properties (PermissionClient PROPERTIES PREFIX "../../bin/")
    set_target_properties (PermissionClient PROPERTIES IMPORT_PREFIX "../../lib/")
endif (MSVC_IDE)


install ( FILES 
    Protocol/I_PermissionProtocolManager.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Protocol/
)

install ( FILES 
    Configuration.hpp
    I_PermissionService.hpp
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	PermissionClient
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
