#pragma once
#include <any>
#include <unordered_map>
#include <string>
#include <optional>
#include "ParameterKey.h"

template<class Key>
using ParameterMap_t = std::unordered_map<Key, std::any>;
using ParameterMap   = ParameterMap_t<ParamKey::Type>;

class EditorParameter {
private:

    ParameterMap _map;

    EditorParameter(void) = default;

public:
    
    ~EditorParameter(void) = default;

    static EditorParameter& GetInstance(void) {
        static EditorParameter instance;
        return instance;
    }

    ParameterMap& GetMap(void) {
        return _map;
    }

    EditorParameter& Register(ParamKey::Type key, std::any data) {
        _map[key] = data;
        return *this;
    }

    template<typename T>
    std::optional<T*> GetData(const ParamKey::Type& key) {
        if (_map.find(key) != _map.end()) {
            if (T* data = std::any_cast<T*>(_map[key])) {
                return data;
            }
        }
        return std::nullopt;
    }

    template<typename T>
    T* GetDataPointer(const ParamKey::Type& key) {
        if (std::optional<T*> c = GetData<T>(key)) {
            return c.value();
        }
        return nullptr;
    }
};

#define theParam EditorParameter::GetInstance()