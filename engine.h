//////////////////////////////////////////////////////////////////////////
/// file:    engine.h
///
/// summary: Declares the main entity of Lyngvi library
//////////////////////////////////////////////////////////////////////////

#ifndef LYNGVI_ENGINE_H_INCLUDED
#define LYNGVI_ENGINE_H_INCLUDED

#include "lyngvi.h"
#include "domain.h"
#include "logger.h"
#include "module_loader.h"

#include <string>
#include <map>

namespace lyngvi
{

class Engine
{
	using DomainHandle = ModuleLoader::DomainHandle;
public:
	explicit Engine(cstr locDir);

	void LoadModule(cstr name);
	void EnableLogging(LogCallback cb, LogLevel what);
	cstr GetTranslation(cstr module, cstr text);
    Logger& GetLog()
    {
        return m_logger;
    }

private:	
	const std::string m_locDir;
	ModuleLoader m_loader;
    Logger m_logger;
	
	std::map<std::string, DomainHandle> m_modules;
};

} // namespace Lyngvi

#endif // LYNGVI_ENGINE_H_INCLUDED