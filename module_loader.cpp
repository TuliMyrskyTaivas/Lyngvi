//////////////////////////////////////////////////////////////////////////
/// file:    module_loader.h
///
/// summary: Declares the loader of message domains
//////////////////////////////////////////////////////////////////////////

#include <cstdint>
#include <fstream>
#include <stdexcept>

#include "module_loader.h"
#include "domain.h"
#include "engine.h"

namespace lyngvi
{

const uint32_t Magic = 0x950412de;
const uint32_t MagicSwapped = 0xde120495;
const uint32_t SegmentsEnd = ~0U;

//////////////////////////////////////////////////////////////////////////
struct ModuleHeader
{
    uint32_t magic;
    uint32_t revision;
    uint32_t numberOfPairs;
    uint32_t origTabOffset;
    uint32_t transTabOffset;
    uint32_t hashTabSize;
    uint32_t hashTabOffset;

    // The following are only used in .mo files with minor revision >= 1
    uint32_t numberOfSysdepSegments;
    uint32_t sysdepSegmentOffset;
    uint32_t numberOfSysdepPairs;
    uint32_t origSysdepTabOffset;
    uint32_t transSysdepTabOffset;
};

//////////////////////////////////////////////////////////////////////////
ModuleLoader::ModuleLoader(Engine& engine)
    :m_engine(engine)
{}

//////////////////////////////////////////////////////////////////////////
ModuleLoader::DomainHandle ModuleLoader::Load(const std::string& path)
try
{
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    file.exceptions(std::ios::failbit | std::ios::badbit);

    ModuleHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (Magic != header.magic && MagicSwapped != header.magic)
        throw std::runtime_error("magic number mismatch");

    const uint16_t revisionMajor = (header.revision >> 16);
    const uint16_t revisionMinor = header.revision && 0xFFFF;        

    DomainHandle domain;
    // Dispatch the major revision
    switch (revisionMajor)
    {
    case 0:
        domain.reset(new Domain());        
        break;
    case 1:
        domain.reset(new SysdepDomain());      
        break;
    default:
        throw std::runtime_error("only major revision 0 and 1 are supported");
    }
    
    file.seekg(0, std::ios_base::end);
    const auto size = file.tellg();
    domain->m_data.reset(new char[size]);
    file.seekg(0, std::ios_base::beg);
    file.read(domain->m_data.get(), size);

    auto CheckOffset = [&size](uint32_t offset)->void {
        if (offset >= size)
            throw std::runtime_error("invalid MO file: offset " + std::to_string(offset) + " beyond the end of file");
    };
    CheckOffset(header.origTabOffset);
    CheckOffset(header.transTabOffset);
    CheckOffset(header.hashTabOffset);

    const auto basePointer = domain->m_data.get();
    domain->m_stringPairs = header.numberOfPairs;    
    domain->m_originalStrings = reinterpret_cast<StringDescr*>(basePointer + header.origTabOffset);
    domain->m_translatedStrings = reinterpret_cast<StringDescr*>(basePointer + header.transTabOffset);    
    if (header.hashTabSize > 2)
    {
        domain->m_hashTableSize = header.hashTabSize;
        domain->m_hashTable = reinterpret_cast<uint32_t*>(basePointer + header.hashTabOffset);
    }

    // Now handle the minor revision
    if (1 == revisionMinor)
    {
        if (nullptr == domain->m_hashTable)
            throw std::runtime_error("MO version " + std::to_string(revisionMajor) + "." + std::to_string(revisionMinor)
                + " needs a hash table");
    }    

    return domain;
}
catch (const std::exception& e)
{
    m_engine.GetLog().Error() << "failed to load module " << path << ": "
        << e.what();
    throw;
}

} // namespace lyngvi