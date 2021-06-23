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

# MSVC compiler options
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
	set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
			$<$<CXX_COMPILER_ID:MSVC>:
				PRIVATE
				/MP           # -> build with multiple processes
				/W4           # -> warning level 4
				# /WX         # -> treat warnings as errors
				
				$<$<CONFIG:Debug>:
					/RTCc         # -> value is assigned to a smaller data type and results in a data loss
					/Zi			  # -> generate debug symbols
				>

				$<$<CONFIG:Release>:
					/Gw           # -> whole program global optimization
					/GS-          # -> buffer security check: no
					/GL           # -> whole program optimization: enable link-time code generation (disables Zi)
					/GF           # -> enable string pooling
				>
			>
			)
endif ()

# GCC and Clang compiler options
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
			PRIVATE
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

			$<$<CXX_COMPILER_ID:GNU>:
				-Wmaybe-uninitialized
				$<$<VERSION_GREATER:$<CXX_COMPILER_VERSION>,4.8>:
					-Wpedantic
					-Wreturn-local-addr
				>
			>

			$<$<CXX_COMPILER_ID:Clang>:
				-Wpedantic
			>
			)
endif ()

# Linker options
set(DEFAULT_LINKER_OPTIONS)

# Use pthreads on mingw and linux
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	set(DEFAULT_LINKER_OPTIONS
			${DEFAULT_LINKER_OPTIONS}
			-pthread
			)
endif()