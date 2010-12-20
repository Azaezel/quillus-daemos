cmake_minimum_required (VERSION 2.6)

project (AccountService)
set (PROJECT_PREFIX "EnterprisePlugins/Account/AccountService")

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

set (AccountService_SRC
        Configuration.hpp
    Model/I_AccountDataCollection.hpp
    Model/I_AccountDataMap.hpp
    Model/I_AccountDomainObject.hpp
    Model/I_AccountGroupDataCollection.hpp
    Model/I_AccountGroupDataMap.hpp
    Model/I_AccountGroupDomainObject.hpp
    Model/I_GroupDataCollection.hpp
    Model/I_GroupDataMap.hpp
    Model/I_GroupDomainObject.hpp
    src/Account.cpp
    src/Account.hpp
    src/AccountScopator.cpp
    src/AccountScopator.hpp
    src/AccountService.cpp
    src/AccountService.hpp
    src/AccountServiceFactory.cpp
    src/AccountServiceFactory.hpp
    src/AccountServicePlugin.cpp
    src/AccountService_gen.cpp
    src/Model/AccountDataCollection.cpp
    src/Model/AccountDataCollection.hpp
    src/Model/AccountDataMap.cpp
    src/Model/AccountDataMap.hpp
    src/Model/AccountDomainObject.cpp
    src/Model/AccountDomainObject.hpp
    src/Model/AccountGroupDataCollection.cpp
    src/Model/AccountGroupDataCollection.hpp
    src/Model/AccountGroupDataMap.cpp
    src/Model/AccountGroupDataMap.hpp
    src/Model/AccountGroupDomainObject.cpp
    src/Model/AccountGroupDomainObject.hpp
    src/Model/GroupDataCollection.cpp
    src/Model/GroupDataCollection.hpp
    src/Model/GroupDataMap.cpp
    src/Model/GroupDataMap.hpp
    src/Model/GroupDomainObject.cpp
    src/Model/GroupDomainObject.hpp
    src/Model/I_AccountDataCollection.cpp
    src/Model/I_AccountDataMap.cpp
    src/Model/I_AccountDomainObject.cpp
    src/Model/I_AccountGroupDataCollection.cpp
    src/Model/I_AccountGroupDataMap.cpp
    src/Model/I_AccountGroupDomainObject.cpp
    src/Model/I_GroupDataCollection.cpp
    src/Model/I_GroupDataMap.cpp
    src/Model/I_GroupDomainObject.cpp

    )
    
add_definitions (-DACCOUNT_SERVICE_EXPORTS ${Zen_DEFINITIONS})

include_directories (${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT})

link_directories (${DEV_LIB} ${Boost_LIBRARY_DIRS})

add_library (AccountService ${LIB_TYPE} ${AccountService_SRC})

add_dependencies (AccountService ZenCore ZenEnterprise AccountClient AccountServer)

target_link_libraries (AccountService ZenCore ZenEnterprise AccountClient AccountServer ${Boost_LIBRARIES})

set_target_properties (AccountService
    PROPERTIES
    DEBUG_POSTFIX "_d"
)

if (MSVC_IDE)
    set_target_properties (AccountService PROPERTIES PREFIX "../../bin/")
    set_target_properties (AccountService PROPERTIES IMPORT_PREFIX "../../lib/")
endif (MSVC_IDE)


install ( FILES 
    Model/I_AccountDataCollection.hpp
    Model/I_AccountDataMap.hpp
    Model/I_AccountDomainObject.hpp
    Model/I_AccountGroupDataCollection.hpp
    Model/I_AccountGroupDataMap.hpp
    Model/I_AccountGroupDomainObject.hpp
    Model/I_GroupDataCollection.hpp
    Model/I_GroupDataMap.hpp
    Model/I_GroupDomainObject.hpp
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
	AccountService
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
