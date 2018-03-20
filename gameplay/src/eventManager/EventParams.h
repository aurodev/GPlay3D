#ifndef EVENTPARAMS_H
#define EVENTPARAMS_H

#include <cassert>

#include <map>
#include <string>

#include "../Variant.h"

namespace gameplay {

// Define a variant with next possible types used for events.
typedef nonstd::variant<
    int
    , unsigned int
    , double
    , std::string
    , const char*
> ArgVariant;


class EventParams
{
    typedef std::map<std::string, ArgVariant> MappedParams;
    typedef MappedParams::iterator MappedParamsIt;
    MappedParams _params;

public:
    // Check a specific parameter exists
    bool find(const std::string& szParamName) const;

    // Count the number of parameters that exist
    size_t count() const;

    // // Set a parameter value
    // template<class T>
    // void Set(const std::string& szParamName, T& ptrValue)
    // {
    //   m_params[szParamName] = reinterpret_cast<void*>(ptrValue);
    // }
    //
    // template<class T>
    // void Set(const std::string& szParamName, T* ptrValue)
    // {
    //     m_params[szParamName] = ptrValue;
    // }


    void set(const std::string& szParamName, ArgVariant value)
    {
        _params[szParamName] = value;
    }



    // Remove a parameter if it exists, returning the reference
    template<class T>
    bool remove(const std::string& szParamName, T& ptrValue)
    {
        MappedParamsIt it(_params.find(szParamName));

        if( it == _params.end() ) {
            return false;
        } else {
            ptrValue = reinterpret_cast<T&>(_params[szParamName]);
            _params.erase(it);
            return true;
        }
    }

    // Return the parameter value reference of known type
    template<class T>
    T& get(const std::string& szParamName)
    {
        return reinterpret_cast<T&>(_params[szParamName]);
    }

    // Return the parameter value as a pointer of known type
    template<class T>
    T* getPtr(const std::string& szParamName)
    {
        return reinterpret_cast<T*>(_params[szParamName]);
    }
};

}

#endif
