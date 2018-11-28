#pragma once
#include <iostream>

#define _DEBUG

template<typename T>
std::ostream& operator<<(
	 typename std::enable_if<std::is_enum<T>::value, 
	 std::ostream>::type& stream, const T& e
)
{
	 return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

template <class T>
constexpr std::string_view type_name()
{
    using namespace std;
#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
    return string_view(p.data() + 36, p.size() - 36 - 1);
#  else
    return string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
    string_view p = __FUNCSIG__;
    return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}

#ifdef _DEBUG
#define LOG_DEBUG(x)	(std::cout << x << std::endl)
#else
#define LOG_DEBUG(x)
#endif