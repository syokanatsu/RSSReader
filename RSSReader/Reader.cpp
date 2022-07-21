#include "Reader.h"
#include <tinyxml2.h>

bool Channel::Parse(const char* str)
{
	tinyxml2::XMLDocument xml;
	auto err = xml.Parse(str);
	auto rss = xml.RootElement();
	auto channel = rss->FirstChildElement("channel");
	this->title_ = channel->FirstChildElement("title")->GetText();
	this->description_ = channel->FirstChildElement("description")->GetText();
	this->link_ = channel->FirstChildElement("link")->GetText();
	auto item = channel->FirstChildElement("item");
	while (item) {
		Item it;
		it.description_ = item->FirstChildElement("description")->GetText();
		it.title_ = item->FirstChildElement("title")->GetText();
		it.link_ = item->FirstChildElement("link")->GetText();
		this->items_.emplace_back(it);
		item = item->NextSiblingElement();
	}
	return true;
}
