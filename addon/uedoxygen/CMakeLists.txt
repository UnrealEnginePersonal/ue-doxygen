find_package(Iconv REQUIRED)

MESSAGE(STATUS "CmakeSourceDir: ${CMAKE_CURRENT_SOURCE_DIR}")
include(${CMAKE_SOURCE_DIR}/cmake/NightmareFuel/NFLogger.cmake)

#config project
add_subdirectory(uedoxygen-config)
#add library
add_library(uedoxygen STATIC
        Source/Private/uedoxygen.cpp
)

target_link_libraries(uedoxygen
    PUBLIC
    NFLogging
)

if (use_libclang)
    find_package(LLVM REQUIRED CONFIG)
    find_package(Clang REQUIRED CONFIG)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_features(uedoxygen PRIVATE cxx_alignof)
        if (use_libc++)
            target_compile_options(uedoxygen PRIVATE -stdlib=libc++)
        endif ()
    endif ()
    include_directories(${LLVM_INCLUDE_DIRS})
    add_definitions(${LLVM_DEFINITIONS})
    if (static_libclang)
        set(CLANG_LIBS libclang clangTooling)
    else () # dynamically linked version of clang
        llvm_config(doxymain USE_SHARED support)
        set(CLANG_LIBS libclang clang-cpp)
    endif ()
    target_compile_definitions(uedoxygen PRIVATE ${LLVM_DEFINITIONS})
endif ()

target_include_directories(
        uedoxygen
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/Private
        ${GENERATED_SRC}
        ${Iconv_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/src
)

target_include_directories(
        uedoxygen
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/Public
)


target_link_libraries(uedoxygen
    PUBLIC
    kdsdocs-models-LIB
    uedoxygen-config-LIB
)

target_link_libraries(uedoxygen
        PRIVATE
        #[[uelogger]]
        doxymain
        md5
        sqlite3
        xml
        lodepng
        mscgen
        doxygen_version
        doxycfg
        vhdlparser
        ${Iconv_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${SQLITE3_LIBRARIES}
        ${EXTRA_LIBS}
        ${CLANG_LIBS}
        ${COVERAGE_LINKER_FLAGS}
)










