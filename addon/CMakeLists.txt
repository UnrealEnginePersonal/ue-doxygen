add_subdirectory(doxmlparser)

if (build_app)
    add_subdirectory(doxyapp)
endif ()

if (build_parse)
    add_subdirectory(doxyparse)
endif ()

if (build_search)
    add_subdirectory(doxysearch)
endif ()

if (build_wizard)
    add_subdirectory(doxywizard)
endif ()

if(build_uedox)
    add_subdirectory(uedoxygen)
    include(${PROJECT_SOURCE_DIR}/cmake/IconV.cmake)
    add_custom_target(copyIconVLibs ALL)
    add_dependencies(copyIconVLibs uedoxygen)
    MESSAGE(STATUS "Copying IconV libraries to build directory  ${IconV_LIB_DIR}")
    add_custom_command(TARGET copyIconVLibs POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${IconV_LIB_DIR} ${PROJECT_BINARY_DIR}
        COMMENT "Copying IconV libraries to build directory"
    )
endif()
