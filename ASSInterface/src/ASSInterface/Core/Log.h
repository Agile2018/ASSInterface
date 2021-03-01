#pragma once

#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace ASSInterface {
	class ASS_API Log
	{
	public:
		static void Init();
		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

// Core Log macros
#define ASS_CORE_INFO(...)	::ASSInterface::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ASS_CORE_TRACE(...)	::ASSInterface::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ASS_CORE_WARN(...)	::ASSInterface::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ASS_CORE_ERROR(...)	::ASSInterface::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ASS_CORE_FATAL(...)	::ASSInterface::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define ASS_CORE_CRITICAL(...)	::ASSInterface::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client Log macros

#define ASS_INFO(...)		::ASSInterface::Log::GetClientLogger()->info(__VA_ARGS__)
#define ASS_TRACE(...)		::ASSInterface::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ASS_WARN(...)		::ASSInterface::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ASS_ERROR(...)		::ASSInterface::Log::GetClientLogger()->error(__VA_ARGS__)
#define ASS_FATAL(...)		::ASSInterface::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define ASS_CRITICAL(...)   ::ASSInterface::Log::GetClientLogger()->critical(__VA_ARGS__)
