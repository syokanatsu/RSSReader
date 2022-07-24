#include "Reader.h"

bool Item::Parse(const tinyxml2::XMLElement* item)
{
	this->description_ = item->FirstChildElement("description")->GetText();
	this->title_ = item->FirstChildElement("title")->GetText();
	this->link_ = item->FirstChildElement("link")->GetText();
	auto pubDate = item->FirstChildElement("pubDate");
	if (pubDate) {
		this->pubDate_ = pubDate->GetText();
	}
	return true;
}

bool Channel::Parse(const char* str, size_t nBytes)
{
	if (xml_) xml_.reset();
	xml_ = std::make_unique<tinyxml2::XMLDocument>();
	auto err = xml_->Parse(str, nBytes);
	auto rss = xml_->RootElement();
	auto channel = rss->FirstChildElement("channel");
	return Parse(channel);
}

bool Channel::Parse(const std::string& str)
{
	return Parse(str.c_str(), str.length());
}

bool Channel::Parse(const tinyxml2::XMLElement* chan)
{
	this->title_ = chan->FirstChildElement("title")->GetText();
	this->description_ = chan->FirstChildElement("description")->GetText();
	this->link_ = chan->FirstChildElement("link")->GetText();
	auto lastBuildDate = chan->FirstChildElement("lastBuildDate");
	if (lastBuildDate) {
		this->lastBuildDate_ = lastBuildDate->GetText();
	}
	return true;
}

bool Channel::ParseItem()
{
	_ASSERT(xml_);
	auto rss = xml_->RootElement();
	auto chan = rss->FirstChildElement("channel");
	auto item = chan->FirstChildElement("item");
	while (item) {
		Item it;
		it.Parse(item);
		this->items_.emplace_back(it);
		item = item->NextSiblingElement();
	}
	return true;
}

SubScriber::SubScriber(const int64_t interval)
	: interval_(interval)
	, alive_({ true })
	, client_("www.kisssub.org")
{
	lastChannel_ = update();
	lastChannel_->ParseItem();
}

SubScriber::~SubScriber()
{
	alive_.exchange(false);
	cv_.notify_all();
	if (run_thread_) {
		run_thread_->wait();
	}
}

void SubScriber::StartSubScribe(const std::string& rss, OnUpdateFunc func)
{
	onUpdate_ = func;
	rssLink_ = rss;
	run_thread_ = std::async(std::launch::async, &SubScriber::run, this);
}

void SubScriber::run()
{
	int count = 0;
	while (alive_)
	{
		std::unique_lock lck(mtx_);
		cv_.wait_for(lck, std::chrono::milliseconds(interval_));
		if (!alive_) break;
		std::cout << "Current Time: " << ++count <<std::endl;
		auto chan = update();
		if (chan && chan->lastBuildDate_ != lastChannel_->lastBuildDate_) {
			// ÓÐ¸üÐÂ
			chan->ParseItem();
			auto last_data = lastChannel_->items_.front().pubDate_;
			auto plist = new std::list<Item>();
			for(auto item : chan->items_) {
				if (item.pubDate_ == last_data) {
					break;
				}
				plist->push_back(item);
			}
			if (!plist->empty() && onUpdate_) {
				onUpdate_(PtrItemList(plist));
			}
			lastChannel_.swap(chan);
		}
	}
}

std::shared_ptr<Channel> SubScriber::update()
{
	std::string body;
	int count = 0;
Retry:
	{
		count++;
		auto res = client_.Get(rssLink_.c_str(),
			[&body](const char* data, size_t data_length) -> bool {
				body.append(data, data_length);
				return true;
			});
		if (res.error() != httplib::Error::Success && count < 10) {
			body.clear();
			goto Retry;
		}
		else if (count > 9) {
			return nullptr;
		}
	}
	auto chan = std::make_shared<Channel>();
	chan->Parse(body);
	return chan;
}
