### Architecture setup

# Determine architecture (32/64 bit)
set(X64 OFF)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(X64 ON)
endif()

# Add custom preprocessor directives here
target_compile_definitions(
	${LIBRARY_TARGET_NAME}
	PRIVATE
)

target_compile_options(
	${LIBRARY_TARGET_NAME}
	PRIVATE
	# MSVC compiler options
    $<$<CXX_COMPILER_ID:MSVC>:
    	/MP
    	/W4
    	$<$<CONFIG:Debug>:
			/ZI			  # -> generate debug symbols and continue
			/Gy
    	>
    	$<$<CONFIG:Release>:
    		/Gw           # -> whole program global optimization
			/GS-          # -> buffer security check: no
			/GL           # -> whole program optimization: enable link-time code generation (disables Zi)
			/GF           # -> enable string pooling
    	>
    >

    # GCC C/C++ compiler options
    $<$<CXX_COMPILER_ID:GNU>:
		-D DEBUG
    	-pthread
		-g

		# -Wall
		# -Wextra
		-Wunused
		-Wreorder
		-Wignored-qualifiers
		-Wmissing-braces
		-Wreturn-type
		-Wswitch
		-Wswitch-default
		-Wuninitialized
		-Wmissing-field-initializers
	>
	
	# Specific options for C++
	$<$<COMPILE_LANG_AND_ID:C,GNU>:
	
	>
    )

# Linker options
set(DEFAULT_LINKER_OPTIONS)
