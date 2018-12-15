//////////////////////////////////////////////////////////////////////////
/// file:    logger.cpp
///
/// summary: Implements the logging facilities for Lyngvi library
//////////////////////////////////////////////////////////////////////////

#include "logger.h"

#include <iostream>
#include <iomanip>
#include <cassert>
#include <thread>
#include <mutex>

namespace lyngvi
{

//////////////////////////////////////////////////////////////////////////
Logger::Logger()
    : m_level(LogLevel::LogNone)    
    , m_printer(nullptr)
{}

//////////////////////////////////////////////////////////////////////////
void Logger::SetSettings(LogLevel level, LogCallback printer)
{
    std::lock_guard<std::mutex> lock(m_lock);
    m_level = level;
    m_printer = printer;
}

//////////////////////////////////////////////////////////////////////////
void Logger::PutMessage(LogLevel level, const std::string& message)
{
    // Check if the requested log level is allowed
    if (!m_printer || level > m_level)
        return;    

    std::ostringstream buf;
    buf << "lyngvi: " << message;

    {
        std::lock_guard<std::mutex> lock(m_lock);
        m_printer(buf.str().c_str());
    }
}

} // namespace lyngvi
