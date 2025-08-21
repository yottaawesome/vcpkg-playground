#include <toml++/toml.hpp>

import std;

template<typename T, typename X>
concept Assignable = requires(T t, X x)
{
	t = x;
};

struct A
{
	std::string S;
	toml::table T;
};

struct TomlFile
{
	template<typename TUnderlying>
	auto Get(std::convertible_to<std::string_view> auto first, std::convertible_to<std::string_view> auto...rest) const
	{
		auto value = GetTomlFile().at_path(first);
		((value = value.at_path(rest)), ...);
		return value.value<TUnderlying>();
	}

	auto GetTomlFile() const -> toml::parse_result&
	{
		static toml::parse_result result = toml::parse_file("settings.toml");
		return result;
	}

	auto Set(auto&& newValue, std::convertible_to<std::string_view> auto...rest) const
	{
		toml::table* node = GetTomlFile().as_table();
			
		int iter = 0;
		((node = 
			[](bool last, auto node, auto&& newValue, auto&& nodeName)
			{
				if (last)
				{
					node->insert_or_assign(nodeName, newValue);
					return node;
				}
				if (not node->contains(nodeName))
					node->insert_or_assign(nodeName, toml::table{});
				return node->at(nodeName).as_table();
			}(iter++ == sizeof...(rest)-1, node, newValue, rest)), ...);
		return;

		//auto value = GetTomlFile().at_path(first);
		//((value = value.at_path(rest)), ...);

		//if (value.type() == toml::node_type::none)
		//{
		//	std::vector<std::pair<std::string_view, toml::table>> tables;
		//	tables.reserve(15);
		//	tables.push_back({ first, toml::table{} });
		//	(tables.push_back({ rest, toml::table{} }), ...);

		//	for (size_t i = tables.size() - 1; i > 0; i--)
		//	{
		//		tables[i - 1].second.insert_or_assign(tables[i].first, tables[i].second);
		//	}

		//	GetTomlFile().insert_or_assign(tables[0].first, tables[0].second);
		//}
		//else
		//{
		//	value.visit(
		//		[newValue](auto&& x)
		//		{
		//			//if constexpr (toml::is_string<std::remove_cvref_t<decltype(x)>>)
		//			if constexpr (Assignable<decltype(x), decltype(newValue)>)
		//			{
		//				std::println("Set");
		//				x = newValue;
		//			}
		//		});
		//}
	}

	void Print() const
	{
		std::cout << GetTomlFile() << std::endl;
	}
};

constexpr TomlFile File;

template<typename TChar, size_t N>
struct FixedString
{
	TChar Buffer[N]{};

	using String = std::basic_string<TChar, std::char_traits<TChar>>;
	using StringView = std::basic_string<TChar, std::char_traits<TChar>>;

	constexpr FixedString(const TChar(&buffer)[N])
	{
		std::copy_n(buffer, N, Buffer);
	}

	constexpr String ToString() const noexcept
	{
		return { Buffer };
	}

	constexpr StringView ToView() const noexcept
	{
		return { Buffer };
	}

	constexpr operator StringView() const noexcept
	{
		return ToView();
	}
};
template<size_t N>
FixedString(const char(&)[N]) -> FixedString<char, N>;

struct OptionalT {} constexpr Optional;
struct RequiredT {} constexpr Required;

template<typename T>
struct False : std::false_type {};

template<auto VConfigSource, bool VWritable, typename TUnderlying, typename TTo, auto VDefault, FixedString...VPath>
struct Value
{
	operator TTo() const
	{
		return Get();
	}

	auto Set(auto&& value) const
	{
		VConfigSource.Set(value, VPath.ToView()...);
	}

	auto Get() const
	{
		using DefaultSpecificationType = std::remove_cvref_t<decltype(VDefault)>;

		auto value = VConfigSource.Get<TUnderlying>(VPath.ToView()...);
		if (value)
			return TTo{ *value };

		// A default has been specified
		if constexpr (not std::same_as<DefaultSpecificationType, OptionalT>
			and not std::same_as<DefaultSpecificationType, RequiredT>)
		{
			return TTo{ VDefault };
		}
		// Not found, but optional
		else if constexpr (std::same_as<DefaultSpecificationType, OptionalT>)
		{
			return TTo{};
		}
		// Required, not found, and no default.
		else if constexpr (std::same_as<DefaultSpecificationType, RequiredT>)
		{
			throw std::runtime_error("Not found.");
		}
		else
		{
			static_assert(False<TTo>::value, "Not specified.");
		}
	}
};

constexpr Value<File, true, long long, std::chrono::milliseconds, Optional, "server.a.timeout"> 
	ServerATimeout;
constexpr Value<File, true, std::string, std::string, Optional, "server.a.url">
	ServerAUrl;
constexpr Value<File, true, std::string, std::string, FixedString{"AAA"}, "server", "a", "blah", "noob">
	Blah;

void Manual()
{
	auto result = toml::parse_file("settings.toml");
	toml::table* xx = result["server"]["a"].as_table();
	toml::table* xx2 = result["server"]["noob"].as_table();
	toml::table a;
	toml::table b{};
	toml::table c{};
	a.insert("a", 1);
	b.insert("b", c);
	a.insert("b", b);

	b.insert("aaa", 1);

	result.insert("podye", a);

	xx->insert_or_assign("additional", "aa");
	std::cout << result << std::endl;
}

auto main() -> int
try
{
	//Manual();

	std::chrono::milliseconds v = ServerATimeout;
	std::string s = ServerAUrl;
	auto y = Blah.Get();
	Blah.Set(std::string_view{ "a" });
	
	/*result["server"]["a"]["url"].visit(
		[](auto&& x)
		{
			if constexpr (toml::is_string<decltype(x)>)
			{
				x = std::string_view{ "five" };
			}
		}
	);*/
	//ServerAUrl.Set("five");

	File.Print();


	//auto ms = std::chrono::milliseconds{ result["timeout"].value_or(3000)};

	return 0;
}
catch (const std::exception& ex)
{
	std::println("{}", ex.what());
	return 1;
}