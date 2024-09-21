set(CMAKE_CXX_STANDARD 17)

add_library(
        uedoxygen-config-LIB
        STATIC
        source/Public/UeDoxConfig.h
)

set_target_properties(uedoxygen-config-LIB PROPERTIES LINKER_LANGUAGE CXX)

#[[target_include_directories(
        uedoxygen-config-LIB
        PRIVATE
        source/Private
)]]

target_include_directories(
        uedoxygen-config-LIB
        PUBLIC
        Source/Public
)


#output lib
install(TARGETS uedoxygen-config-LIB DESTINATION lib)