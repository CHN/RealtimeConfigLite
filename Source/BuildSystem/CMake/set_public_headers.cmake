function(SetPublicHeaders CURRENT_DIR OUT_PUBLIC_HEADER_PATHS)

	file(GLOB subdirs RELATIVE ${CURRENT_DIR} ${CURRENT_DIR}/*)

	foreach(subdir ${subdirs})

		if(subdir STREQUAL "Private")
			continue()
		endif()

		if(subdir STREQUAL "Public")
			list(APPEND ${OUT_PUBLIC_HEADER_PATHS} ${CURRENT_DIR}/${subdir})
			continue()
		endif()

		if(IS_DIRECTORY ${CURRENT_DIR}/${subdir})
			SetPublicHeaders(${CURRENT_DIR}/${subdir} ${OUT_PUBLIC_HEADER_PATHS})
		endif()

	endforeach()

	set(${OUT_PUBLIC_HEADER_PATHS} ${${OUT_PUBLIC_HEADER_PATHS}} PARENT_SCOPE)
	
endfunction()