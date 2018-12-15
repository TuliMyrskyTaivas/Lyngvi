add_subdirectory(googletest)

include_directories(${CMAKE_SOURCE_DIR})
add_executable (lyngvi_test
	enable_logging_tests.cpp
	get_text_tests.cpp
	initialize_tests.cpp
	load_module_tests.cpp
	main.cpp	
)
target_link_libraries(lyngvi_test gtest lyngvi)

add_test (NAME PublicApiTests
	COMMAND lyngvi_test ${CMAKE_CURRENT_SOURCE_DIR}/samples
)