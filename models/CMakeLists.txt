set(CMAKE_CXX_STANDARD 17)

add_library(
        kdsdocs-models-LIB
        STATIC
        source/Private/Models/DoxygenArguments.cpp
)
set_target_properties(kdsdocs-models-LIB PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(
        kdsdocs-models-LIB
        PRIVATE
        source/Private
)

target_include_directories(
        kdsdocs-models-LIB
        PUBLIC
        source/Public
)

#output lib
install(TARGETS kdsdocs-models-LIB DESTINATION lib)