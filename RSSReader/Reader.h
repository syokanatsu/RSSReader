#pragma once
#include <string>
#include <map>
#include <optional>
#include <list>
#include <memory>
#include <condition_variable>
#include <tinyxml2.h>
#include <future>
#include <functional>
#include <httplib.h>

#ifdef RSSREADER_EXPORTS
#define RSSREADERAPI __declspec(dllexport)
#else
#define RSSREADERAPI __declspec(dllimport)
#endif

class Item;
using PtrItemList = std::shared_ptr<std::list<Item>>;
using OnUpdateFunc = std::function<void(PtrItemList)>;

class RSSREADERAPI Item {
public:
	bool Parse(const tinyxml2::XMLElement* item);
public:
	std::optional<std::string> author_;
	std::optional<std::string> category_;
	std::optional<std::string> comments_;
	std::string description_;
	std::string title_;
	std::string link_;
	std::optional<std::string> pubDate_;
};

class RSSREADERAPI Channel {
public:

	bool Parse(const char* str, size_t nBytes = static_cast<size_t>(-1));
	bool Parse(const std::string& str);
	bool Parse(const tinyxml2::XMLElement* chan);
	bool ParseItem();
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
	std::optional<std::string> lastBuildDate_;
	std::unique_ptr<tinyxml2::XMLDocument> xml_;
};

class RSSREADERAPI SubScriber {
public:

	SubScriber(const int64_t interval = 10000);
	~SubScriber();

	void StartSubScribe(const std::string& rss, OnUpdateFunc func);

private:
	void run();
	std::shared_ptr<Channel> update();

	std::optional<std::future<void>> run_thread_;

	OnUpdateFunc onUpdate_;

	httplib::Client client_;

	std::atomic_bool alive_;

	std::mutex mtx_;

	std::condition_variable cv_;

	std::shared_ptr<Channel> lastChannel_;

	int64_t interval_;

	std::string rssLink_;
};
