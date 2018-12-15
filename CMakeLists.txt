cmake_minimum_required(VERSION 3.6)
project(lyngvi CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compilation modes
if (UNIX)
    set (CMAKE_CXX_FLAGS         "-ggdb -Wall -Wpedantic -Wextra -Werror -static-libgcc -static-libstdc++")
    set (CMAKE_CXX_FLAGS_DEBUG   "-O0")
    set (CMAKE_CXX_FLAGS_RELEASE "-Os")
elseif (MSVC)
    set (CMAKE_CXX_FLAGS         "/MP /WX /W4 /EHsc /sdl /guard:cf /GS")
	set (CMAKE_CXX_FLAGS_DEBUG   "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")	
	set (CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
endif ()

option(ENABLE_TESTING "Enable tests" ON)

# Sources
add_library (lyngvi STATIC
	domain.h
	domain.cpp
	engine.h
	engine.cpp
	logger.h
	logger.cpp
	lyngvi.h
    lyngvi.cpp	
	module_loader.h
    module_loader.cpp
)

if (${ENABLE_TESTING})
	enable_testing()
	add_subdirectory(test)
endif()	