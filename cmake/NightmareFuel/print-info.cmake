#[[
fun

MESSAGE(${BASE_OUTPUT_DIR})
MESSAGE(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
MESSAGE(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
MESSAGE(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})]]

###############PRINT IMPORTANT VARS ETC ###############
function(print_info)
    MESSAGE(STATUS "BASE_OUTPUT_DIR: ${BASE_OUTPUT_DIR}")
    MESSAGE(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    MESSAGE(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    MESSAGE(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
endfunction()