#pragma once
#include <ranges>


/*
template <typename T>
requires std::ranges::range<T>
constexpr bool containsDuplicate(const T& container)
{
    /*for (auto it = container.begin(); it != container.end()-1; ++it)
    {
        for (auto jt = it+1; jt != container.end(); ++jt)
        {
            if (*it == *jt)
            {
                return true;
            }
        }
    }#1#
    return false;
}
*/
