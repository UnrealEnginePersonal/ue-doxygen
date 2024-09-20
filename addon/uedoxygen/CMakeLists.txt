find_package(Iconv REQUIRED)

add_library(unreal_doxygen STATIC
        Source/Private/uedoxygen.cpp
)

if (use_libclang)
    find_package(LLVM REQUIRED CONFIG)
    find_package(Clang REQUIRED CONFIG)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_features(unreal_doxygen PRIVATE cxx_alignof)
        if (use_libc++)
            target_compile_options(unreal_doxygen PRIVATE -stdlib=libc++)
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
    target_compile_definitions(unreal_doxygen PRIVATE ${LLVM_DEFINITIONS})
endif ()

target_include_directories(
        unreal_doxygen
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/Private
        ${GENERATED_SRC}
        ${Iconv_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/src
)

target_include_directories(
        unreal_doxygen
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/Public
)


target_link_libraries(unreal_doxygen
    PUBLIC
    kdsdocs-models-LIB
)

target_link_libraries(unreal_doxygen
        PRIVATE

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

install(TARGETS unreal_doxygen DESTINATION bin)










