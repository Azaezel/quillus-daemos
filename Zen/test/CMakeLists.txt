Find_Package(CppUnit)
Find_Package(Boost REQUIRED serialization system filesystem)

# Configure Test build
if (ZEN_BUILD_UNIT_TESTS)

    if (CppUnit_FOUND)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/Core/include ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${CppUnit_INCLUDE_DIR})
    
        set (ZENCORE_HEADER_FILES
            Core/include/MathTests.hpp
            include/Suite.hpp
        )
    
        set (ZENCORE_SOURCE_FILES
            Core/src/MathTests.cpp
            src/Suite.cpp
            src/main.cpp
        )
        
        set (ZENCORE_RESOURCE_FILES
        )
        
        add_definitions(${Zen_DEFINITIONS})
        
        add_executable(Test_ZenCore ${ZENCORE_HEADER_FILES} ${ZENCORE_SOURCE_FILES} ${ZENCORE_RESOURCE_FILES})
        #zen_config_unit_test_exe(Test_ZenCore)
        target_link_libraries(Test_ZenCore ZenCore ${CppUnit_LIBRARIES} ${Boost_LIBRARIES})
        
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/Engine/include ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${CppUnit_INCLUDE_DIR})
    
        set (ZENENGINE_HEADER_FILES
			Engine/include/SoundTests.hpp
            include/Suite.hpp
        )
        
        set (ZENENGINE_SOURCE_FILES
			Engine/src/SoundTests.cpp
            src/Suite.cpp
            src/main.cpp
        )
        
        set (ZENENGINE_RESOURCE_FILES
        )
        
        add_definitions(${Zen_DEFINITIONS})
        
        add_executable(Test_ZenEngine ${ZENENGINE_HEADER_FILES} ${ZENENGINE_SOURCE_FILES} ${ZENENGINE_RESOURCE_FILES})
        #zen_config_unit_test_exe(Test_ZenEngine)
        target_link_libraries(Test_ZenEngine ZenCore ZenEngine ${CppUnit_LIBRARIES} ${Boost_LIBRARIES})
        
        include_directories(
            ${CMAKE_CURRENT_SOURCE_DIR}/include 
            ${CMAKE_CURRENT_SOURCE_DIR}/Enterprise/Account/include
            ${CMAKE_CURRENT_SOURCE_DIR}/Enterprise/Session/include 
            ${Boost_INCLUDE_DIR} 
            ${INDIEZEN_PARENT} 
            ${CppUnit_INCLUDE_DIR}
            )
    
        set (ZENENTERPRISE_HEADER_FILES
            Enterprise/Account/include/AccountClientTests.hpp
            Enterprise/Session/include/SessionClientTests.hpp
            include/Suite.hpp
        )
        
        set (ZENENTERPRISE_SOURCE_FILES
            Enterprise/Account/src/AccountClientTests.cpp
            Enterprise/Session/src/SessionClientTests.cpp
            src/Suite.cpp
            src/main.cpp
        )
        
        set (ZENENTERPRISE_RESOURCE_FILES
            Enterprise/Account/AccountClientTests.xml
            Enterprise/Account/AccountClientTests.lua
            Enterprise/Session/SessionClient.xml
        )
        
        add_definitions(${Zen_DEFINITIONS})
        
        add_executable(Test_ZenEnterprise ${ZENENTERPRISE_HEADER_FILES} ${ZENENTERPRISE_SOURCE_FILES} ${ZENENTERPRISE_RESOURCE_FILES})
        #zen_config_unit_test_exe(Test_ZenEngine)
        target_link_libraries(Test_ZenEnterprise ZenCore ZenEnterprise AccountClient AccountServer SessionClient SessionServer ${CppUnit_LIBRARIES} ${Boost_LIBRARIES})

    else(CppUnit_FOUND)
        message("-- CppUnit NOT FOUND")
    endif (CppUnit_FOUND)
endif (ZEN_BUILD_UNIT_TESTS)