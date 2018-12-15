//////////////////////////////////////////////////////////////////////////
/// file:    test/main.cpp
///
/// summary: Entry point for unit tests
//////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <mutex>

#include <lyngvi.h>

//////////////////////////////////////////////////////////////////////////
void LyngviLogger(lyngvi::cstr str)
{
    static std::mutex outputLock;
    std::lock_guard<std::mutex> lock(outputLock);
    std::cout << "Lyngvi: " << str << std::endl;
}

//////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);    
    if (2 != argc)
    {
        std::cout << "invalid command line: path to the samples directory should be specified"
            << std::endl;
        return EXIT_FAILURE;
    }

    lyngvi::Initialize(argv[1]);
    lyngvi::EnableLogging(LyngviLogger, lyngvi::LogLevel::LogAll);
    return RUN_ALL_TESTS();
}