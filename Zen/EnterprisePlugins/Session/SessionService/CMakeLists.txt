cmake_minimum_required (VERSION 2.6)

project (SessionService)
set (PROJECT_PREFIX "EnterprisePlugins/Session/SessionService")

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

set (SessionService_SRC
        Configuration.hpp
    Model/I_SessionDataCollection.hpp
    Model/I_SessionDataMap.hpp
    Model/I_SessionDomainObject.hpp
    src/Model/I_SessionDataCollection.cpp
    src/Model/I_SessionDataMap.cpp
    src/Model/I_SessionDomainObject.cpp
    src/Model/SessionDataCollection.cpp
    src/Model/SessionDataCollection.hpp
    src/Model/SessionDataMap.cpp
    src/Model/SessionDataMap.hpp
    src/Model/SessionDomainObject.cpp
    src/Model/SessionDomainObject.hpp
    src/Session.cpp
    src/Session.hpp
    src/SessionScopator.cpp
    src/SessionScopator.hpp
    src/SessionService.cpp
    src/SessionService.hpp
    src/SessionServiceFactory.cpp
    src/SessionServiceFactory.hpp
    src/SessionServicePlugin.cpp
    src/SessionService_gen.cpp

    )
    
add_definitions (-DSESSION_SERVICE_EXPORTS ${Zen_DEFINITIONS} -DBOOST_ALL_DYN_LINK)

include_directories (${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT})

link_directories (${DEV_LIB} ${Boost_LIBRARY_DIRS})

add_library (SessionService ${LIB_TYPE} ${SessionService_SRC})

add_dependencies (SessionService ZenCore ZenEnterprise SessionClient SessionServer AccountClient)

target_link_libraries (SessionService ZenCore ZenEnterprise SessionClient SessionServer AccountClient ${Boost_LIBRARIES})

set_target_properties (SessionService
    PROPERTIES
    DEBUG_POSTFIX "_d"
)

if (MSVC_IDE)
    set_target_properties (SessionService PROPERTIES PREFIX "../../bin/")
    set_target_properties (SessionService PROPERTIES IMPORT_PREFIX "../../lib/")
endif (MSVC_IDE)


install ( FILES 
    Model/I_SessionDataCollection.hpp
    Model/I_SessionDataMap.hpp
    Model/I_SessionDomainObject.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/Model/
)

install ( FILES 
    Configuration.hpp
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	SessionService
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
