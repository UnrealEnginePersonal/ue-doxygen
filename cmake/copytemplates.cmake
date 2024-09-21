#This copies the templates files to the build directory
function(copy_templates)
  # Copy the templates to the build directory
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/templates DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endfunction()