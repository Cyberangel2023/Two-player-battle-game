#include "thirdparty/httplib.h"

#include <mutex>
#include <string>
#include <fstream>

std::mutex g_mutex;      // 全局互斥锁

int player_1 = -1;     // 玩家1
int player_2 = -1;     // 玩家2

std::string ch_1 = "mn";
std::string ch_2 = "mn";

std::string status_1 = "300,300,0,0,f,f,100";
std::string status_2 = "300,300,0,0,f,f,100";

int main(int argc, char** argv)
{
	httplib::Server server;

	server.Post("/login", [&](const httplib::Request& req, httplib::Response& res)
		{
			std::lock_guard<std::mutex> lock(g_mutex);

			if (player_1 >= 0 && player_2 >= 0)
			{
				res.set_content("-1", "text/plain");
				return;
			}

			res.set_content(player_1 >= 0 ? "2" : "1", "text/plain");

			(player_1 >= 0) ? (player_2 = 0) : (player_1 = 0);
			if (player_1 >= 0 && player_2 >= 0)
			{
				ch_1 = "Re";
				ch_2 = "Re";
			}
		});

	server.Post("/update_1", [&](const httplib::Request& req, httplib::Response& res)
		{
			std::lock_guard<std::mutex> lock(g_mutex);

			ch_1 = req.body;

			res.set_content(ch_2, "text/plain");
		});

	server.Post("/update_2", [&](const httplib::Request& req, httplib::Response& res)
		{
			std::lock_guard<std::mutex> lock(g_mutex);

			ch_2 = req.body;

			res.set_content(ch_1, "text/plain");
		});

	server.Post("/update_status_1", [&](const httplib::Request& req, httplib::Response& res)
		{
			std::lock_guard<std::mutex> lock(g_mutex);

			status_1 = req.body;

			res.set_content(status_2, "text/plain");
		});

	server.Post("/update_status_2", [&](const httplib::Request& req, httplib::Response& res)
		{
			std::lock_guard<std::mutex> lock(g_mutex);

			status_2 = req.body;

			res.set_content(status_1, "text/plain");
		});

	server.listen("129.211.30.252", 3389);

	return 0;
}