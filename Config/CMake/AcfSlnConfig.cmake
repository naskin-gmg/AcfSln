function(acfsln_get_root_dir identifier_to_use)
	set(${identifier_to_use} "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../../.." PARENT_SCOPE)
endfunction()

