#pragma once
#include <functional>

template<typename T>
using Reference = std::shared_ptr<T>;

template <typename T>
using Function = std::function<T>;
using ApplicationEvent = Function<void()>;
using UpdateEvent = Function<void(float)>;


#define unreferenced(...) (__VA_ARGS__)