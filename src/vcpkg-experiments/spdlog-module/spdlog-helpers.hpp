#include <string>
#include "spdlog/spdlog.h"

namespace SpdlogHelpers
{
	void Info(std::string_view fmt, auto&&...args)
	{
		spdlog::info(fmt.data());
	}
}