cmake_minimum_required (VERSION 2.6) 

project (ZenObjectModel)
set ( PROJECT_PREFIX "Spaces/ObjectModel" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED serialization )

message("Libraries: ${Boost_LIBRARIES}")

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include ZenObjectModel source file

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( ZenObjectModel_SRC
    I_ElementTypeCollection.hpp
    I_MethodType.hpp
    I_View.hpp
    I_ObjectModelManager.hpp
    I_ElementCollection.hpp
    I_ObjectCollection.hpp
    I_ObjectType.hpp
    I_Filter.hpp
    src/I_Element.cpp
    src/I_Object.cpp
    src/I_MethodType.cpp
    src/I_ObjectType.cpp
    src/I_ElementType.cpp
    src/I_Filter.cpp
    src/I_Subscription.cpp
    src/I_ObjectModelManager.cpp
    src/I_ElementCollection.cpp
    src/I_ObjectCollection.cpp
    src/I_Method.cpp
    I_Element.hpp
    I_Subscription.hpp
    I_Method.hpp
    I_ElementType.hpp
    Configuration.hpp
    I_Object.hpp

    )

add_definitions( -DOBJECTMODEL_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} )

link_directories ( ${DEV_LIB} ${Boost_LIBRARY_DIRS}  )

add_library ( ZenObjectModel ${LIB_TYPE} ${ZenObjectModel_SRC} )

add_dependencies(ZenObjectModel ZenCore ZenEnterprise )

target_link_libraries( ZenObjectModel ZenCore ZenEnterprise ${Boost_LIBRARIES} )

set_target_properties ( ZenObjectModel 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZenObjectModel PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZenObjectModel PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    I_ElementTypeCollection.hpp
    I_MethodType.hpp
    I_View.hpp
    I_ObjectModelManager.hpp
    I_ElementCollection.hpp
    I_ObjectCollection.hpp
    I_ObjectType.hpp
    I_Filter.hpp
    I_Element.hpp
    I_Subscription.hpp
    I_Method.hpp
    I_ElementType.hpp
    Configuration.hpp
    I_Object.hpp
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZenObjectModel
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)