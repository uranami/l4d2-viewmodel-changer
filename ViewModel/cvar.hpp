#pragma once 

template < typename T > static T VirtualCall(void* base, const std::uint16_t index)
{
    return (*reinterpret_cast<T**>(base))[index];
}

struct Color {
    uint8_t r, g, b, a;
    Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 255) : r(_r), g(_g), b(_b), a(_a) {}
};

struct IConVar
{
private:
    using FnChangeCallback_t = void (*)(void* var, const char* pOldValue, float flOldValue);
    char pad_0x0000[0x4] {};
    IConVar* _next {};
    bool _registered {};
    const char* _name {};
    const char* _helpString {};
    uintptr_t _flags {};
    char pad_0x0018[0x4] {};
    IConVar* _parent {};
    const char* _defaultValue {};
    const char* _string {};
    int _stringLength {};
    float _valueFloat {};
    int _valueInt {};
    bool _hasMin {};
    float _minValue {};
    bool _hasMax {};
    float _maxValue {};
    FnChangeCallback_t _change_callback {};

    auto InternalSetValue(const char* v)
    {
        using Fn = void(__thiscall*)(void*, const char*);
        return VirtualCall<Fn>(this, 12)(this, v);
    }

    auto InternalSetFloatValue(float v)
    {
        using Fn = void(__thiscall*)(void*, float);
        return VirtualCall<Fn>(this, 13)(this, v);
    }

    auto InternalSetIntValue(int v)
    {
        using Fn = void(__thiscall*)(void*, int);
        return VirtualCall<Fn>(this, 14)(this, v);
    }

public:

    ////55 8B EC 8B 45 ? 83 EC ? 56 int __thiscall ConVar::Create(
    //int __thiscall ConVar::Create(
    //    _DWORD* this,
    //    int a2,
    //    const char* a3,
    //    int a4,
    //    int* a5,
    //    float a6,
    //    int a7,
    //    char a8,
    //    int a9,
    //    int a10)
    auto SetValue(const char* values)
    {
        IConVar* var = (IConVar*)_parent;
        var->InternalSetValue(values);
    }

    auto SetValue(float values)
    {
        IConVar* var = (IConVar*)_parent;
        var->InternalSetFloatValue(values);
    }

    auto SetValue(int values)
    {
        IConVar* var = (IConVar*)_parent;
        var->InternalSetIntValue(values);
    }

    [[nodiscard]] int GetInt() const
    {
        return this->_valueInt;
    }

    [[nodiscard]] bool GetBool() const
    {
        return this->_valueInt > 0;
    }

    [[nodiscard]] float GetFloat() const
    {
        return this->_valueFloat;
    }

    [[nodiscard]] bool HasMin() const
    {
        return this->_hasMin;
    }

    [[nodiscard]] bool HasMax() const
    {
        return this->_hasMax;
    }

    [[nodiscard]] float GetMax() const
    {
        return this->_maxValue;
    }

    [[nodiscard]] float GetMin() const
    {
        return this->_minValue;
    }

    [[nodiscard]] const char* GetString() const
    {
        return this->_string;
    }

    [[nodiscard]] const char* GetDefault() const
    {
        return this->_defaultValue;
    }

    const char* GetName() const
    {
        return _name;
    }
};

class ICVarIteratorInternal 
{
public:
    virtual ~ICVarIteratorInternal()
    {
    }
    virtual void SetFirst(void)       = 0;
    virtual void Next(void)           = 0;
    virtual bool IsValid(void)        = 0;
    virtual IConVar* Get(void) = 0;
};

struct ICvar
{
    IConVar* FindVar(const char* name)
    {
        using Fn = IConVar*(__thiscall*)(void*, const char*);
        return VirtualCall<Fn>(this, 12)(this, name);
    }
    template<typename... Args>
    void const PrintToConsole(const Color& clr, const char* format, Args... args)
    {
        VirtualCall<void(__cdecl*)(void*, const Color&, const char*, ...)>(this, 21)(this, clr, format, args...);
    }

    ICVarIteratorInternal* GetCvarIterator()
    {
        using Fn = ICVarIteratorInternal*(__thiscall*)(void*);
        return VirtualCall<Fn>(this, 38)(this);
    }
};
