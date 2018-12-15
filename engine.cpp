//////////////////////////////////////////////////////////////////////////
/// file:    engine.cpp
///
/// summary: Implements the main entity of Lyngvi library
//////////////////////////////////////////////////////////////////////////

#include <stdexcept>

#include "engine.h"

namespace lyngvi
{

#ifdef _WIN32
# define PATH_SEPARATOR "\\"
#else
# define PATH_SEPARATOR "/"
#endif

//////////////////////////////////////////////////////////////////////////
Engine::Engine(cstr locDir)
	: m_locDir(locDir)
    , m_loader(*this)
{}

//////////////////////////////////////////////////////////////////////////
void Engine::LoadModule(cstr name)
{
	if (!name)
		throw std::invalid_argument("lyngvi: module name couldn't be empty");

    auto domain = m_modules.find(name);
    if (m_modules.end() != domain)
    {
        return;
    }

	const std::string path = m_locDir + PATH_SEPARATOR + std::string(name) + ".mo";
	m_modules.emplace(std::string(name), m_loader.Load(path));
}

//////////////////////////////////////////////////////////////////////////
void Engine::EnableLogging(LogCallback cb, LogLevel what)
{
    m_logger.SetSettings(what, cb);	
}

//////////////////////////////////////////////////////////////////////////
cstr Engine::GetTranslation(cstr module, cstr text)
try
{
    auto domain = m_modules.find(module);
    if (m_modules.end() != domain)
    {
        auto res = domain->second->FindTranslation(text);
        if (nullptr != res)
            return res;

        m_logger.Untranslated() << "untranslated: " << text;
        return text;
    }

    LoadModule(module);
    return GetTranslation(module, text);    
}
catch (const std::exception& e)
{
    m_logger.Error() << e.what();
	return text;
}

} // namespace lyngvi
