//////////////////////////////////////////////////////////////////////////
/// file:    domain.cpp
///
/// summary: Implements the message domain interface
//////////////////////////////////////////////////////////////////////////

#include <cstring>
#include <iostream>

#include "domain.h"

namespace lyngvi
{

Domain::~Domain()
{
    std::cout << "~DOMAIN" << std::endl;
}

//////////////////////////////////////////////////////////////////////////
cstr Domain::FindByHash(cstr original) const
{
    const uint32_t len = static_cast<uint32_t>(strlen(original));
    const uint32_t hashValue = HashString(original);    
    const uint32_t incr = 1 + (hashValue % (m_hashTableSize - 2));
    uint32_t idx = hashValue % m_hashTableSize;

    while (true)
    {
        uint32_t nstr = m_hashTable[idx];
        if (0 == nstr)
            return nullptr;

        --nstr;
        if (nstr < m_stringPairs)
        {
            if (m_originalStrings[nstr].length >= len &&
                0 == strcmp(original, m_data.get() + m_originalStrings[nstr].offset))
            {
                return m_data.get() + m_translatedStrings[nstr].offset;
            }
        }

        if (idx >= m_hashTableSize - incr)
            idx -= m_hashTableSize - incr;
        else
            idx += incr;
    }
}

//////////////////////////////////////////////////////////////////////////
cstr Domain::FindByValue(cstr original) const
{
    // Try the default method: binary search in the sorted array of messages
    uint32_t top = m_stringPairs, bottom = 0;
    while (bottom < top)
    {
        const uint32_t actual = (bottom + top) / 2;
        const auto originalString = m_data.get() + m_originalStrings[actual].offset;
        const auto cmpResult = strcmp(original, originalString);
        if (cmpResult < 0)
            top = actual;
        else if (cmpResult > 0)
            bottom = actual + 1;
        else
            return m_data.get() + m_translatedStrings[actual].offset;
    }

    return nullptr;
}

//////////////////////////////////////////////////////////////////////////
uint32_t Domain::HashString(cstr original) const
{
    uint32_t hval = 0;
    const char *str = original;
    const uint32_t HashWordBits = 32;

    /* Compute the hash value for the given string.  */
    hval = 0;
    while (*str != '\0')
    {
        hval <<= 4;
        hval += (unsigned char)*str++;
        const uint32_t g = hval & ((unsigned long int) 0xf << (HashWordBits - 4));
        if (g != 0)
        {
            hval ^= g >> (HashWordBits - 8);
            hval ^= g;
        }
    }
    return hval;
}

//////////////////////////////////////////////////////////////////////////
cstr Domain::FindTranslation(cstr original) const
{    
    return m_hashTable ? FindByHash(original) : FindByValue(original);
}

} // namespace lyngvi
