//////////////////////////////////////////////////////////////////////////
/// file:    domain.h
///
/// summary: Declares the message domain interface
//////////////////////////////////////////////////////////////////////////

#ifndef LYNGVI_DOMAIN_H_INCLUDED
#define LYNGVI_DOMAIN_H_INCLUDED

#include <memory>
#include <vector>
#include <cstdint>

#include "lyngvi.h"

namespace lyngvi
{

//////////////////////////////////////////////////////////////////////////
struct StringDescr
{
	uint32_t length;
	uint32_t offset;
};

//////////////////////////////////////////////////////////////////////////
struct SysdepSegment
{
    uint32_t length;
    uint32_t offset;
};

//////////////////////////////////////////////////////////////////////////
struct SegmentPair
{
    uint32_t size;
    uint32_t sysdepPref;
};

//////////////////////////////////////////////////////////////////////////
struct SysdepString
{
    uint32_t offset;
    SegmentPair segments[1];
};

class ModuleLoader;

class Domain
{
	friend class ModuleLoader;

    cstr FindByHash(cstr original) const;
    cstr FindByValue(cstr original) const;
    uint32_t HashString(cstr original) const;

protected:
	/// \brief Pointer to the memory containing the .mo file
	std::unique_ptr<char> m_data;
	/// \brief Number of static string pairs
	uint32_t     m_stringPairs;
	/// \brief Descriptors of original strings in the file
	StringDescr* m_originalStrings;
	/// \brief Descriptors of translated strings in the file
	StringDescr* m_translatedStrings;	
    uint32_t     m_hashTableSize;
    uint32_t*    m_hashTable;
public:
	virtual cstr FindTranslation(cstr original) const;
    ~Domain();
};

class SysdepDomain : public Domain
{
    friend class ModuleLoader;
protected:
    SysdepString* m_origSysdepStrings;
    SysdepString* m_translatedSysdepStrings;
    
};

} // namespace lyngvi

#endif // LYNGVI_DOMAIN_H_INCLUDED