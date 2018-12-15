//////////////////////////////////////////////////////////////////////////
/// file:    logger.h
///
/// summary: Declares the logging facilities for Lyngvi library
//////////////////////////////////////////////////////////////////////////

#ifndef LYNGVI_LOGGER_H_INCLUDED
#define LYNGVI_LOGGER_H_INCLUDED

#include <mutex>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>

#include "lyngvi.h"

namespace lyngvi
{

class Engine;

//////////////////////////////////////////////////////////////////////////
class Logger
{
    friend class Engine;
    //////////////////////////////////////////////////////////////////////////
    /// <summary>
    ///   Helper class to collect the whole message via '<<' operator
    ///   and put it into the logging streams. Instances of this class
    ///   could be obtained via Logger::Debug(), Logger::Info() and
    ///   Logger::Error() methods only.
    /// </summary>
    //////////////////////////////////////////////////////////////////////////
    class MessagePrinter
    {
        friend class Logger;
        Logger& m_parent;
        LogLevel m_level;
        std::unique_ptr<std::ostringstream> m_buf;

    protected:
        MessagePrinter(Logger& parent, LogLevel level)
            : m_parent(parent), m_level(level), m_buf(new std::ostringstream())
        {}

    public:
        //////////////////////////////////////////////////////////////////////////
        /// Default move constructor is enabled on purpose to allow proper work of
        /// the Logger's methods Debug(), Info() and Error()
        //////////////////////////////////////////////////////////////////////////
        MessagePrinter(MessagePrinter&&) = default;

        //////////////////////////////////////////////////////////////////////////
        /// Message is put to the logging stream on object destruction (at the
        /// end of the expression evaluation at the instantiation point)
        //////////////////////////////////////////////////////////////////////////
        ~MessagePrinter()
        {
            m_parent.PutMessage(m_level, m_buf->str());
        }

        //////////////////////////////////////////////////////////////////////////
        /// The only useful method here: put something into the stream
        //////////////////////////////////////////////////////////////////////////
        template<class T>
        MessagePrinter& operator << (const T& data)
        {
            *m_buf << data;
            return *this;
        }
    };

public:    
    //////////////////////////////////////////////////////////////////////////
    /// <summary>
    ///   Put the debug message into the log
    /// </summary>
    //////////////////////////////////////////////////////////////////////////
    MessagePrinter Debug()
    {
        return MessagePrinter(*this, LogLevel::LogAll);
    }

    //////////////////////////////////////////////////////////////////////////
    /// <summary>
    ///   Put the message about the untranslated string into the log
    /// </summary>
    //////////////////////////////////////////////////////////////////////////
    MessagePrinter Untranslated()
    {
        return MessagePrinter(*this, LogLevel::LogUntranslated);
    }

    //////////////////////////////////////////////////////////////////////////
    /// <summary>
    ///   Put the error message into the log
    /// </summary>
    //////////////////////////////////////////////////////////////////////////
    MessagePrinter Error()
    {
        return MessagePrinter(*this, LogLevel::LogErrors);
    }

    //////////////////////////////////////////////////////////////////////////
    /// <summary>
    ///   Set logger settings
    /// </summary>
    ///
    /// <param name="level"> The level of the logging messages </param>
    /// <param name="printer"> </param>
    //////////////////////////////////////////////////////////////////////////
    void SetSettings(LogLevel level, LogCallback printer);

protected:
    Logger();
    void PutMessage(LogLevel level, const std::string& message);

private:    
    LogLevel m_level;
    LogCallback m_printer;
    std::mutex m_lock;
};

} // namespace lyngvi

#endif // LYNGVI_LOGGER_H_INCLUDED
