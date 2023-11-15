module;

#include <string>
#include "spdlog-helpers.hpp"

export module spdlogmodule2;
//import :part;

export namespace SpdlogTest2
{
	//using SpdlogHelpers::info;
	//using SpdlogHelpers::Info;
	//using SpdlogPart::Info;

	template<typename...Args>
	void Info(std::string_view fmt, Args&&...args)
	{
		SpdlogHelpers::Info(fmt, std::forward<Args>(args)...);
	}

	//using SpdlogHelpers::Info;
}
