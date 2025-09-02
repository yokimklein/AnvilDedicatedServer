#pragma once

template<typename t_member_function>
struct t_member_to_static_function;

template<typename t_result, typename t_class, typename...t_args>
struct t_member_to_static_function<t_result(t_class::*)(t_args...)>
{
    using t_member_function = t_result(t_class::*)(t_args...);
    using t_static_function = t_result(*)(t_class* _this, t_args...);

    t_member_to_static_function() = delete;
    t_member_to_static_function(t_member_to_static_function const&) = delete;
    t_member_to_static_function(t_member_function _member_function) :
        member_function(_member_function)
    {

    }

    t_member_function member_function;

    operator t_member_function()
    {
        return member_function;
    }

    operator t_static_function()
    {
        return *reinterpret_cast<t_static_function*>(&member_function);
    }
};

template<typename t_userdata, typename t_member_function>
struct t_member_to_static_function_userdata;

template<typename t_userdata, typename t_result, typename t_class, typename...t_args>
struct t_member_to_static_function_userdata<t_userdata, t_result(t_class::*)(t_args...)>
{
    using t_member_function = t_result(t_class::*)(t_args...);
    using t_static_function = t_result(*)(t_userdata* _this, t_args...);

    t_member_to_static_function_userdata() = delete;
    t_member_to_static_function_userdata(t_member_to_static_function_userdata const&) = delete;
    t_member_to_static_function_userdata(t_member_function _member_function) :
        member_function(_member_function)
    {

    }

    t_member_function member_function;

    operator t_member_function()
    {
        return member_function;
    }

    operator t_static_function()
    {
        return *reinterpret_cast<t_static_function*>(&member_function);
    }
};

template<typename t_member_function>
auto member_to_static_function(t_member_function member_function)
{
    using t_member_to_static = t_member_to_static_function<decltype(member_function)>;
    t_member_to_static method_conversion_helper(member_function);
    return static_cast<typename t_member_to_static::t_static_function>(method_conversion_helper);
}

template<typename t_userdata, typename t_member_function>
auto member_to_static_function(t_member_function member_function)
{
    using t_member_to_static = t_member_to_static_function_userdata<t_userdata, decltype(member_function)>;
    t_member_to_static method_conversion_helper(member_function);
    return static_cast<typename t_member_to_static::t_static_function>(method_conversion_helper);
}

template<typename function_t>
union static_to_member_t
{
    unsigned long address;
    void* pointer;
    function_t function;
};

union module_address
{
    size_t address;
    char* data;
    void* pointer;
};