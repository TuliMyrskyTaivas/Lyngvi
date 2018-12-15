//////////////////////////////////////////////////////////////////////////
/// file:    lyngvi.h
///
/// summary: Declares the interface of Lyngvi library
//////////////////////////////////////////////////////////////////////////

#ifndef LYNGVI_LYNGVI_H_INCLUDED
#define LYNGVI_LYNGVI_H_INCLUDED

namespace lyngvi
{

using cstr = const char*;

enum class LogLevel : unsigned int
{
	LogNone         = 0x0000,
	LogUntranslated = 0x0001,
	LogErrors       = 0x0002,
	LogAll          = 0xF000
};
using LogCallback = void(*)(cstr);

void Initialize(cstr locDir);
void LoadModule(cstr name);
void EnableLogging(LogCallback cb, LogLevel what);
cstr GetText(cstr const module, cstr text);

} // namespace lyngvi

#endif // LYNGVI_LYNGVI_H_INCLUDED