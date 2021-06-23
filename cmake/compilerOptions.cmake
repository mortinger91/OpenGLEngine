### Architecture setup

# Determine architecture (32/64 bit)
set(X64 OFF)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(X64 ON)
endif()

# MSVC compiler options
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
	set(DEFAULT_COMPILE_DEFINITIONS ${DEFAULT_COMPILE_DEFINITIONS}
			_SCL_SECURE_NO_WARNINGS  # Calling any one of the potentially unsafe methods in the Standard C++ Library
			_CRT_SECURE_NO_WARNINGS  # Calling any one of the potentially unsafe methods in the CRT Library
			)
endif ()

### Compile options
set(DEFAULT_COMPILE_OPTIONS)

target_compile_options(
	exec 
	PRIVATE
	# MSVC compiler options
    $<$<CXX_COMPILER_ID:MSVC>:
    	/MP
    	/Wall
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

    # GCC compiler options
    $<$<CXX_COMPILER_ID:GNU>:
    	-pthread
    	-Wall
		-Wextra
		-Wunused
		-g
		-Wreorder
		-Wignored-qualifiers
		-Wmissing-braces
		-Wreturn-type
		-Wswitch
		-Wswitch-default
		-Wuninitialized
		-Wmissing-field-initializers
    >
    )

# Linker options
set(DEFAULT_LINKER_OPTIONS)
