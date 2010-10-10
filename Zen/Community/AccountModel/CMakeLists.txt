cmake_minimum_required (VERSION 2.6) 

project (AccountModel)
set ( PROJECT_PREFIX "Community/AccountModel" )

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


	
set ( AccountModel_SRC
    I_GroupMemberDomainObject.hpp
    I_GroupMemberDataMap.hpp
    I_GroupDataCollection.hpp
    I_PermissionDataMap.hpp
    I_AccountDomainObject.hpp
    src/GroupMemberDataMap.cpp
    src/PermissionDataCollection.hpp
    src/GroupDomainObject.cpp
    src/GroupMemberDomainObject.cpp
    src/PermissionDomainObject.hpp
    src/GroupMemberDataCollection.cpp
    src/AccountDomainObject.cpp
    src/PermissionDataCollection.cpp
    src/AccountDataMap.hpp
    src/AccountDataCollection.hpp
    src/I_AccountDomainObject.cpp
    src/GroupMemberDomainObject.hpp
    src/GroupDataCollection.cpp
    src/I_GroupDomainObject.cpp
    src/GroupDataMap.hpp
    src/I_GroupMemberDomainObject.cpp
    src/I_PermissionDataMap.cpp
    src/AccountDomainObject.hpp
    src/I_AccountDataCollection.cpp
    src/GroupDataMap.cpp
    src/I_PermissionDomainObject.cpp
    src/I_GroupDataMap.cpp
    src/AccountDataMap.cpp
    src/I_GroupDataCollection.cpp
    src/PermissionDomainObject.cpp
    src/I_AccountDataMap.cpp
    src/GroupMemberDataMap.hpp
    src/I_PermissionDataCollection.cpp
    src/I_GroupMemberDataMap.cpp
    src/GroupMemberDataCollection.hpp
    src/GroupDomainObject.hpp
    src/AccountDataCollection.cpp
    src/GroupDataCollection.hpp
    src/PermissionDataMap.hpp
    src/PermissionDataMap.cpp
    src/I_GroupMemberDataCollection.cpp
    I_AccountDataCollection.hpp
    I_AccountDataMap.hpp
    I_GroupDomainObject.hpp
    I_PermissionDataCollection.hpp
    Configuration.hpp
    I_GroupDataMap.hpp
    I_PermissionDomainObject.hpp
    I_GroupMemberDataCollection.hpp

    )

add_definitions( -DACCOUNTMODEL_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${LIBXML2_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} )

add_library ( AccountModel ${LIB_TYPE} ${AccountModel_SRC} )

add_dependencies(AccountModel ZenCore ZenEnterprise)

target_link_libraries( AccountModel ZenCore ZenEnterprise )

set_target_properties ( AccountModel 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( AccountModel PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( AccountModel PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    I_GroupMemberDomainObject.hpp
    I_GroupMemberDataMap.hpp
    I_GroupDataCollection.hpp
    I_PermissionDataMap.hpp
    I_AccountDomainObject.hpp
    I_AccountDataCollection.hpp
    I_AccountDataMap.hpp
    I_GroupDomainObject.hpp
    I_PermissionDataCollection.hpp
    Configuration.hpp
    I_GroupDataMap.hpp
    I_PermissionDomainObject.hpp
    I_GroupMemberDataCollection.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	AccountModel
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)