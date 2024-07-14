#pragma once

using HashType = size_t;
constexpr HashType InvalidHash = {};

template <typename T>
HashType HashFun(const T& value)
{
    return std::hash<T>{}(value);
}