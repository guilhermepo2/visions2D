#pragma once
#include <functional>

template <typename T>
using Function = std::function<T>;
using ApplicationEvent = Function<void()>;
using UpdateEvent = Function<void(float)>;

#define unreferenced(...) (__VA_ARGS__)