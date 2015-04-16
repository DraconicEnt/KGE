/**
 */

#include <easylua.hpp>

namespace EasyLua
{
    template <>
    void Table::get(const char *key, Table &out)
    {
        static std::hash<const char *> hasher;

        const size_t hash = hasher(key);

        if (mTypes.count(hash) == 0)
            throw std::runtime_error("No such key!");

        out.copy(mTables[hash]);
    }
} // End NameSpace EasyLua
