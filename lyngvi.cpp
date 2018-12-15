//////////////////////////////////////////////////////////////////////////
/// file:    lyngvi.cpp
///
/// summary: Declares the interface of Lyngvi library
//////////////////////////////////////////////////////////////////////////

#include <stdexcept>
#include <memory>

#include "lyngvi.h"
#include "engine.h"

namespace lyngvi
{

static std::unique_ptr<Engine> engine;

//////////////////////////////////////////////////////////////////////////
void Initialize(cstr locDir)
{
	if (engine)
		throw std::logic_error("lyngvi was already initialized");	

	engine.reset(new Engine(locDir));
}

//////////////////////////////////////////////////////////////////////////
void LoadModule(cstr name)
{
	if (!engine)
		throw std::logic_error("lyngvi is not initialized");	

	engine->LoadModule(name);
}

//////////////////////////////////////////////////////////////////////////
void EnableLogging(LogCallback cb, LogLevel what)
{
	if (!engine)
		throw std::logic_error("lyngvi is not initialized");

	engine->EnableLogging(cb, what);
}

//////////////////////////////////////////////////////////////////////////
cstr GetText(cstr const module, cstr text)
{
	if (!engine)
		throw std::logic_error("lyngvi is not initialized");

	return engine->GetTranslation(module, text);
}

} // namespace lyngvi