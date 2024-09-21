#this combines all libs into a single lib
#this is useful for creating a single lib from multiple libs

function(combine_libs)
    set(py_combine_script ${PROJECT_SOURCE_DIR}/scripts/lib_combiner.py)

