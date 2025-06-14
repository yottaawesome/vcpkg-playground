module;

#define FMT_UNICODE 0
#include <spdlog/spdlog.h>

export module spdlogmodule;
import std;

// Still doesn't work, no workarounds available.
// https://github.com/gabime/spdlog/issues/2701
// https://developercommunity.visualstudio.com/t/Visual-Studio-cant-find-time-function/1126857

namespace Internal
{
	constexpr auto Format = "{}\n    ['{}' in {}:{}].";
	void Info(std::string&& s)
	{
		spdlog::info(std::move(s));
	}
	void Warn(std::string&& s)
	{
		spdlog::warn(std::move(s));
	}
	void Error(std::string&& s)
	{
		spdlog::error(std::move(s));
	}
}

export struct LogLocationCapture
{
	std::source_location location = std::source_location::current();
};

export namespace Log
{
	template<typename...TArgs>
	constexpr void Info(std::format_string<TArgs...> format, TArgs&&...args)
	{
		Internal::Info(std::format(format, std::forward<TArgs>(args)...));
	}

	template<typename...TArgs>
	constexpr void Warn(LogLocationCapture m, std::format_string<TArgs...> format, TArgs&&...args)
	{
		Internal::Warn(std::format(Internal::Format, std::format(format, std::forward<TArgs>(args)...), m.location.function_name(), m.location.file_name(), m.location.line()));
	}

	template<typename...TArgs>
	void Error(LogLocationCapture m, std::format_string<TArgs...> format, TArgs&&...args)
	{
		Internal::Error(std::format(Internal::Format, std::format(format, std::forward<TArgs>(args)...), m.location.function_name(), m.location.file_name(), m.location.line()));
	}
}

export namespace Random2
{
	template<typename...TArgs>
	struct MessageAndLocation
	{
		std::format_string<TArgs...> Fmt;
		std::source_location Location;

		consteval MessageAndLocation(auto&& fmt, std::source_location loc = std::source_location::current()) : Fmt(fmt), Location(loc) { };
	};

	template<typename...TArgs>
	constexpr auto FMT(MessageAndLocation<std::type_identity_t<TArgs>...> m, TArgs&&...args)
	{
		//m.location;
		//std::format(m.message, std::forward<TArgs>(args)...);
		return Internal::Error(
			std::format(
				Internal::Format, 
				std::format(m.Fmt, std::forward<TArgs>(args)...),
				m.Location.function_name(),
				m.Location.file_name(),
				m.Location.line()));
	}
}
