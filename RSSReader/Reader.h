#pragma once
#include <string>
#include <map>
#include <optional>
#include <list>

class Item {
public:

public:
	std::optional<std::string> author_;
	std::optional<std::string> category_;
	std::optional<std::string> comments_;
	std::string description_;
	std::string title_;
	std::string link_;
	std::optional<std::string> pubDate_;
};

class Channel {
public:

	bool Parse(const char* str);
public:
	std::optional<std::string> category_;
	std::optional<std::string> copyright_;
	struct Image
	{
		std::optional<std::string> description_;
		std::optional<int32_t> height_;
		std::string link_;
		std::string title_;
		std::string url_;
		std::optional<int32_t> width_;
	};
	std::optional<Image> image_;
	std::optional<std::string> language_;
	std::string link_;
	std::optional<std::string> generator_;
	std::string description_;
	std::string title_;
	std::list<Item> items_;
};

class Reader
{
};

