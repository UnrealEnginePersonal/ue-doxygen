MESSAGE(STATUS "Current Project Source Directory: ${PROJECT_SOURCE_DIR}")


add_library(unreal_doxygen STATIC
    Source/Private/uedoxygen.cpp
)

target_include_directories(
    unreal_doxygen
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Private
)

target_include_directories(
    unreal_doxygen
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Public
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/libversion
    ${GENERATED_SRC}
    ${CLANG_INCLUDEDIR}
    ${Iconv_INCLUDE_DIRS}
)

add_sanitizers(unreal_doxygen)

if (use_libclang)
    find_package(LLVM REQUIRED CONFIG)
    find_package(Clang REQUIRED CONFIG)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_features(unreal_doxygen PRIVATE cxx_alignof)
        if (use_libc++)
            target_compile_options(unreal_doxygen PRIVATE -stdlib=libc++)
        endif()
    endif()
    include_directories(${LLVM_INCLUDE_DIRS})
    add_definitions(${LLVM_DEFINITIONS})
    if (static_libclang)
        set(CLANG_LIBS libclang clangTooling)
    else() # dynamically linked version of clang
        llvm_config(doxymain USE_SHARED support)
        set(CLANG_LIBS libclang clang-cpp)
    endif()
    target_compile_definitions(unreal_doxygen PRIVATE ${LLVM_DEFINITIONS})
endif()

target_link_libraries(unreal_doxygen
        PUBLIC
        doxymain
        doxygen_version
        doxycfg
        md5
        sqlite3
        xml
        lodepng
        mscgen
        vhdlparser
)

target_link_libraries(unreal_doxygen
    PUBLIC
    ${CMAKE_THREAD_LIBS_INIT}
    ${SQLITE3_LIBRARIES}
    ${EXTRA_LIBS}
    ${CLANG_LIBS}
    ${COVERAGE_LINKER_FLAGS}
)

install(TARGETS unreal_doxygen DESTINATION lib)
