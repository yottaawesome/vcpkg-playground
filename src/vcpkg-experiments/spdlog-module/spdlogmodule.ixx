module;

#include <string>
#include <spdlog/spdlog.h>

export module spdlogmodule;

// Still doesn't work, no workarounds available.
// https://github.com/gabime/spdlog/issues/2701
// https://developercommunity.visualstudio.com/t/Visual-Studio-cant-find-time-function/1126857
export namespace SpdlogTest
{
	//using spdlog::info;

	void Info(std::string_view format, auto&&...args)
	{
		//spdlog::info(format.data(), std::forward<decltype(args)>(args)...);
	}
}
