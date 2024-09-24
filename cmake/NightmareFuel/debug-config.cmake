if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    MESSAGE(STATUS "Debug build")
    # Set _CL_ flags for debug builds
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /NOLOGO /MD /DEBUG")
    # Set _LINK_ flags for debug builds
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /nologo /Z")
endif()