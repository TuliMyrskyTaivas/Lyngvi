//////////////////////////////////////////////////////////////////////////
/// file:    module_loader.h
///
/// summary: Declares the loader of message domains
//////////////////////////////////////////////////////////////////////////

#ifndef LYNGVI_MODULE_LOADER_H_INCLUDED
#define LYNGVI_MODULE_LOADER_H_INCLUDED

#include <string>
#include <memory>
#include "domain.h"

namespace lyngvi
{

class Engine;

class ModuleLoader
{
    friend class Engine;
    ModuleLoader(Engine& engine);

public:
	using DomainHandle = std::unique_ptr<Domain>;
	DomainHandle Load(const std::string& path);	

private:
    Engine& m_engine;
};

} // namespace lyngvi

#endif // LYNGVI_MODULE_LOADER_H_INCLUDED