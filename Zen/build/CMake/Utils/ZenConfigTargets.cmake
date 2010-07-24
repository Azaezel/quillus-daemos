# Configure settings and install targets

# Default build output paths
if (NOT ZEN_ARCHIVE_OUTPUT)
    set(ZEN_ARCHIVE_OUTPUT ${INDIEZEN_PARENT}/lib)
endif ()
if (NOT ZEN_LIBRARY_OUTPUT)
    set(ZEN_LIBRARY_OUTPUT ${INDIEZEN_PARENT}/lib)
endif ()
if (NOT ZEN_RUNTIME_OUTPUT)
    set(ZEN_RUNTIME_OUTPUT ${INDIEZEN_PARENT}/bin)
endif ()

if (WIN32)
  set(ZEN_RELEASE_PATH "")
  set(ZEN_RELWDBG_PATH "")
  set(ZEN_MINSIZE_PATH "")
  set(ZEN_DEBUG_PATH "")
  set(ZEN_LIB_RELEASE_PATH "")
  set(ZEN_LIB_RELWDBG_PATH "")
  set(ZEN_LIB_MINSIZE_PATH "")
  set(ZEN_LIB_DEBUG_PATH "")
  set(ZEN_PLUGIN_PATH "")
elseif (UNIX)
  set(ZEN_RELEASE_PATH "")
  set(ZEN_RELWDBG_PATH "")
  set(ZEN_MINSIZE_PATH "")
  set(ZEN_DEBUG_PATH "")
  if (NOT APPLE)
	set(ZEN_DEBUG_PATH "")
  endif ()
  set(ZEN_LIB_RELEASE_PATH "")
  set(ZEN_LIB_RELWDBG_PATH "")
  set(ZEN_LIB_MINSIZE_PATH "")
  set(ZEN_LIB_DEBUG_PATH "")
  if (APPLE)
    set(ZEN_PLUGIN_PATH "")
  else()
    set(ZEN_PLUGIN_PATH "")
  endif(APPLE)
endif ()

# create vcproj.user file for Visual Studio to set debug working directory
function(zen_create_vcproj_userfile TARGETNAME)
    if (MSVC)
        configure_file(
            ${ZEN_TEMPLATES_DIR}/VisualStudioUserFile.vcproj.user.in
            ${CMAKE_CURRENT_BINARY_DIR}/${TARGETNAME}.vcproj.user
            @ONLY
        )
    endif()
endfunction(zen_create_vcproj_userfile)

# install targets according to current build type
function(zen_install_target TARGETNAME SUFFIX EXPORT)
endfunction(zen_install_target)

# setup common target settings
function(zen_config_common TARGETNAME)
    set_target_properties(${TARGETNAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${ZEN_ARCHIVE_OUTPUT}
        LIBRARY_OUTPUT_DIRECTORY ${ZEN_LIBRARY_OUTPUT}
        RUNTIME_OUTPUT_DIRECTORY ${ZEN_RUNTIME_OUTPUT}
    )
    
    zen_create_vcproj_userfile(${TARGETNAME})
endfunction(zen_config_common)

# setup library build
function(zen_config_lib LIBNAME EXPORT)
    zen_config_common(${LIBNAME})
    if (ZEN_STATIC)
        # add static prefix, if compiling static version
        set_target_properties(${LIBNAME} PROPERTIES OUTPUT_NAME ${LIBNAME}Static)
    else (ZEN_STATIC)
        if (CMAKE_COMPILER_IS_GNUCXX)
            # add GCC visibility flags to shared library build
            set_target_properties(${LIBNAME} PROPERTIES COMPILE_FLAGS "${ZEN_GCC_VISIBILITY_FLAGS}")
        endif (CMAKE_COMPILER_IS_GNUCXX)
        if (MINGW)
            # remove lib prefix from DLL outputs
            set_target_properties(${LIBNAME} PROPERTIES PREFIX "")
        endif (MINGW)
    endif (ZEN_STATIC)
    zen_install_target(${LIBNAME} "" ${EXPORT})
    
    if (ZEN_INSTALL_PDB)
        # install debug pdb files
        if (ZEN_STATIC)
            install(FILES ${ZEN_BINARY_DIR}/lib${ZEN_LIB_DEBUG_PATH}/${LIBNAME}Static_d.pdb
                DESTINATION ${ZEN_LIB_DIRECTORY}${ZEN_LIB_DEBUG_PATH}
                CONFIGURATIONS Debug
            )
            install(FILES ${ZEN_BINARY_DIR}/lib${ZEN_LIB_RELWDBG_PATH}/${LIBNAME}Static.pdb
                DESTINATION ${ZEN_LIB_DIRECTORY}${ZEN_LIB_RELWDBG_PATH}
                CONFIGURATIONS RelWithDebInfo
            )
        else (ZEN_STATIC)
            install(FILES ${ZEN_BINARY_DIR}/bin${ZEN_DEBUG_PATH}/${LIBNAME}_d.pdb
                DESTINATION ${ZEN_RUNTIME_DIR}${ZEN_DEBUG_PATH}
                CONFIGURATIONS Debug
            )
            install(FILES ${ZEN_BINARY_DIR}/bin${ZEN_RELWDBG_PATH}/${LIBNAME}.pdb
                DESTINATION ${ZEN_RUNTIME_DIR}${ZEN_RELWDBG_PATH}
                CONFIGURATIONS RelWithDebInfo
            )
        endif (ZEN_STATIC)
    endif (ZEN_INSTALL_PDB)
endfunction(zen_config_lib)

function(zen_config_component LIBNAME)
    zen_config_lib(${LIBNAME} FALSE)
endfunction(zen_config_component)

# setup Zen unit test build
function(zen_config_test_common TESTNAME)
    zen_config_common(${TESTNAME})
    
    # set install RPATH for Unix systems
    if (UNIX AND ZEN_FULL_RPATH)
        set_property(TARGET ${TESTNAME} APPEND PROPERTY
            INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${ZEN_LIB_DIRECTORY})
        )
        set_property(TARGET ${TESTNAME} PROPERTY 
            INSTALL_RPATH_USE_LINK_PATH TRUE
        )
    endif ()
    
    if (CMAKE_COMPILER_IS_GNUCXX)
        # add GCC visibility flags to shared library build
        set_target_properties(${TESTNAME} PROPERTIES COMPILE_FLAGS "${ZEN_GCC_VISIBILITY_FLAGS}")
        # disable "lib" prefix on Unix
        set_target_properties(${TESTNAME} PROPERTIES PREFIX "")
    endif (CMAKE_COMPILER_IS_GNUCXX)
    
endfunction(zen_config_test_common)

function(zen_config_test_exe TESTNAME)
    zen_config_sample_common(${TESTNAME})
    if (ZEN_INSTALL_PDB AND ZEN_INSTALL_SAMPLES)
        # install debug pdb files - no _d on exe
        install(FILES ${ZEN_BINARY_DIR}/bin${ZEN_DEBUG_PATH}/${TESTNAME}.pdb
            DESTINATION bin${ZEN_DEBUG_PATH}
            CONFIGURATIONS Debug
        )
        install(FILES ${ZEN_BINARY_DIR}/bin${ZEN_RELWDBG_PATH}/${TESTNAME}.pdb
            DESTINATION bin${ZEN_RELWDBG_PATH}
            CONFIGURATIONS RelWithDebInfo
        )
    endif ()
endfunction(zen_config_test_exe)