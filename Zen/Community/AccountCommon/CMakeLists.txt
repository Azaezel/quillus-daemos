cmake_minimum_required (VERSION 2.6) 

project (AccountCommon)
set ( PROJECT_PREFIX "Community/AccountCommon" )

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


	
set ( AccountCommon_SRC
    I_Account.hpp
    I_GroupMemberView.hpp
    I_AccountController.hpp
    I_GroupController.hpp
    I_AccountModel.hpp
    I_GroupMemberModel.hpp
    I_GroupModel.hpp
    subscription.hpp
    src/I_AccountView.cpp
    src/I_GroupMemberController.cpp
    src/I_GroupView.cpp
    src/I_GroupModel.cpp
    src/I_Group.cpp
    src/I_PermissibleResource.cpp
    src/I_GroupMemberModel.cpp
    src/I_GroupMemberView.cpp
    src/I_AccountService.cpp
    src/I_AccountModel.cpp
    src/I_AccountController.cpp
    src/I_Account.cpp
    src/I_GroupController.cpp
    I_AccountService.hpp
    I_AccountView.hpp
    I_GroupView.hpp
    Configuration.hpp
    I_Group.hpp
    I_GroupMemberController.hpp
    I_PermissibleResource.hpp

    )

add_definitions( -DACCOUNTCOMMON_EXPORTS ${Zen_DEFINITIONS} )

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS} )

add_library ( AccountCommon ${LIB_TYPE} ${AccountCommon_SRC} )

add_dependencies(AccountCommon ZenCore ZenEngine ZenEnterprise)

target_link_libraries(AccountCommon ZenCore ZenEngine ZenEnterprise ${Boost_LIBRARIES} )

set_target_properties ( AccountCommon
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( AccountCommon PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( AccountCommon PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    I_Account.hpp
    I_GroupMemberView.hpp
    I_AccountController.hpp
    I_GroupController.hpp
    I_AccountModel.hpp
    I_GroupMemberModel.hpp
    I_GroupModel.hpp
    subscription.hpp
    I_AccountService.hpp
    I_AccountView.hpp
    I_GroupView.hpp
    Configuration.hpp
    I_Group.hpp
    I_GroupMemberController.hpp
    I_PermissibleResource.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	AccountCommon
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)