#include <iostream>
#include <memory>
#include <conio.h>
#include "../RSSReader/Reader.h"

void OnUpdate(PtrItemList plist) {
	std::cout << std::endl;
	std::cout << "Update: " << plist->size() << std::endl;
	std::cout << std::endl;
	for (auto item : *plist.get()) {
		std::cout << "Title: " << item.title_ << std::endl;
		std::cout << "Link: " << item.link_ << std::endl;
		std::cout << "PubDate: " << item.pubDate_.value_or("") << std::endl;
		std::cout << "Description: " << item.description_ << std::endl;
		std::cout << std::endl;
	}
}

int main() {
	system("chcp 65001");
	SubScriber sub;
	sub.StartSubScribe("/rss.xml", OnUpdate);
	while (_getch() != 'q');
	return 0;
}
