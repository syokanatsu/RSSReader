#include "Reader.h"
#include <httplib.h>
#include <memory>

int main() {
	httplib::Client cli("www.kisssub.org");

	auto res = cli.Get("/rss.xml");
	if (res->status == 200) {
		auto chan = std::make_shared<Channel>();
		chan->Parse(res->body.c_str());
		system("pause");
	}
	return 0;
}