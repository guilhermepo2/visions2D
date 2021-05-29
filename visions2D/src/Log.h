#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class Log {
public:
	static void Initialize();
	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

private:
	static std::shared_ptr<spdlog::logger> s_Logger;
};

#define LOG_INFO(...)		Log::GetLogger()->info(__VA_ARGS__);
#define LOG_WARNING(...)	Log::GetLogger()->warn(__VA_ARGS__);
#define LOG_ERROR(...)		Log::GetLogger()->error(__VA_ARGS__);

#ifdef assert
#undef assert
#endif

#define assert(x, ...) { if(!(x)) { LOG_ERROR(__VA_ARGS__); __debugbreak; } }