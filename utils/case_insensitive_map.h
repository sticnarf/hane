#ifndef RACKCPP_CASE_INSENSITIVE_MAP_H
#define RACKCPP_CASE_INSENSITIVE_MAP_H

#include <string>
#include <map>
#include "string_utils.h"

template<typename V>
class CaseInsensitiveMap;

template<typename V>
class MapEntry
{
private:
    bool valid;
    std::string key;
    V value;

    explicit MapEntry<V>(bool valid)
            :valid(valid) { }

    friend class CaseInsensitiveMap<V>;

public:
    MapEntry<V>() = default;

    MapEntry<V>(const std::string& key, const V& value)
            :valid(true), key(key), value(value) { }

    bool isValid() const
    {
        return valid;
    }

    const std::string& getKey() const
    {
        if (!valid)
            throw std::logic_error("MapEntry is not valid");

        return key;
    }

    V getValue() const
    {
        if (!valid)
            throw std::logic_error("MapEntry is not valid");

        return value;
    }

};

template<typename V>
class CaseInsensitiveMap
{
    std::map<std::string, MapEntry<V> > innerMap;

public:
    void put(const std::string& key, const V& value)
    {
        innerMap[StringUtils::toLowercase(key)] = MapEntry<V>(key, value);
    }

    MapEntry<V> get(const std::string& key) const
    {
        auto entry = innerMap.find(StringUtils::toLowercase(key));

        if (entry == innerMap.end())
        {
            return MapEntry<V>(false);
        }

        return entry->second;
    }
};
#endif
