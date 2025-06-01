#include "thirdparty/httplib.h"

#include <graphics.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>

#include "util.h"
#include "resources_manager.h"
#include "collision_manager.h"
#include "character_manager.h"
#include "scene_manager.h"
#include "start_game_button.h"
#include "quit_game_button.h"
#include "menu_scene.h"
#include "game_scene.h"
#include "selector_scene.h"

std::string cr = "mn";           // 按键信息
std::string cr_other = "mn";     // 按键信息

// 脚底位置 速度 朝向 动画朝向 hp 状态
std::string status_1;         // 角色信息序列
std::string status_2;         // 角色信息序列

int type_1 = 0;               // 角色类型
int type_2 = 0;               // 角色类型

bool game_start = false;      // 游戏开始

bool enter_1 = false;         // 确认开始1
bool enter_2 = false;         // 确认开始2

bool is_quit = false;         // 是否结束程序
bool is_selector = false;     // 是否选角

bool ending = false;          // 一场游戏是否结束

bool is_player_1 = true;      // 是否为角色1

int id = 0;                   // 角色ID

bool player_1_left = false;   // 角色1是否朝向左
bool player_2_left = true;    // 角色2是否朝向左

IMAGE* img_player_1_avatar = nullptr;    // 玩家1头像
IMAGE* img_player_2_avatar = nullptr;    // 玩家2头像

Scene* menu_scene = nullptr;       // 菜单
Scene* game_scene = nullptr;       // 游戏
Scene* selector_scene = nullptr;   // 选角

Character* player_1 = nullptr;     // 创建玩家1角色
Character* player_2 = nullptr;     // 创建玩家2角色

SceneManager scene_manager;        // 场景管理器

std::string str_address;             // 服务器地址
httplib::Client* client = nullptr;   // HTTP客户端对象

SOCKET gServer;

enum class Stage
{
	Waiting,    // 等待玩家加入
	Racing      // 开始游戏
};

Stage stage = Stage::Waiting;    // 当前游戏阶段

std::vector<std::string> split_string_by_comma(const std::string& str) {
	std::vector<std::string> tokens;
	std::istringstream iss(str);
	std::string token;

	while (std::getline(iss, token, ',')) {
		tokens.push_back(token);
	}

	return tokens;
}

void load_address(HWND hwnd)
{
	std::ifstream file("config.cfg");

	if (!file.good())
	{
		MessageBox(hwnd, L"无法打开配置 config.cfg", L"启动失败", MB_OK | MB_ICONERROR);
		exit(-1);
	}

	std::stringstream str_stream;
	str_stream << file.rdbuf();
	str_address = str_stream.str();

	file.close();
}

void login_to_server(HWND hwnd)
{
	cr = "Re";

	client = new httplib::Client("129.211.30.252", 3389);
	client->set_keep_alive(true);

	httplib::Result result = client->Post("/login"); 
	if (!result || result->status != 200)
	{
		MessageBox(hwnd, _T("无法连接到服务器！"), _T("启动失败"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	id = std::stoi(result->body);

	if (id <= 0)
	{
		MessageBox(hwnd, _T("比赛已经开始啦！"), _T("拒绝加入"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	std::thread([&]()
		{
			while (true)
			{
				if (ending)
					break;

				if (!game_start)
					continue;

				using namespace std::chrono;

				status_1 = std::to_string((float)1420 - player_1->get_position().x) + "," + std::to_string(player_1->get_position().y)
					+ "," + std::to_string(-player_1->get_velocity().x) + "," + std::to_string(player_1->get_velocity().y) + "," + (player_1->get_facing_left()
						? "f" : "t") + "," + (player_1->get_render_facing_left() ? "f" : "t") + "," + std::to_string(player_1->get_hp());
				std::string route = (id == 1) ? "/update_status_1" : "/update_status_2";
				std::string body = status_1;
				httplib::Result result_statu = client->Post(route, body, "text/plain");

				if (result_statu && result_statu->status == 200)
				{
					status_2 = result_statu->body;
					std::vector<std::string> status_2_list = split_string_by_comma(status_2);
					player_2->set_position({ std::stof(status_2_list[0]), std::stof(status_2_list[1]) });
					player_2->set_velocity({ std::stof(status_2_list[2]), std::stof(status_2_list[3]) });
					(status_2_list[4] == "t") ? player_2->set_facing_left(true) : player_2->set_facing_left(false);
					(status_2_list[5] == "t") ? player_2->set_render_facing_left(true) : player_2->set_render_facing_left(false);
					player_2->set_hp(std::stoi(status_2_list[6]));
				}
			}
		}).detach();

	std::thread([&]()
		{
			while (true)
			{
				if (ending)
					break;

				if (cr == "")
					cr = "mn";

				std::string route = (id == 1) ? "/update_1" : "/update_2";
				std::string body = cr;
				httplib::Result result_key = client->Post(route, body, "text/plain");

				if (result_key && result_key->status == 200)
				{
					cr_other = result_key->body;

					if (cr_other == "OK")
						enter_2 = true;
				}
			}
		}).detach();
}

int main(int argc, char** argv)
{
	using namespace std::chrono;

	HWND hwnd = initgraph(1420, 800);
	SetWindowText(hwnd, _T("虫虫大作战"));

	AddFontResourceEx(_T("resources/IPix.ttf"), FR_PRIVATE, NULL);
	settextstyle(28, 0, _T("IPix"));

	setbkmode(TRANSPARENT);

	//load_address(hwnd);
	login_to_server(hwnd);

	try
	{
		ResourcesManager::instance()->load();
	}
	catch (const LPCTSTR id)
	{
		TCHAR err_msg[512];
		_stprintf_s(err_msg, _T("无法加载: %s"), id);
		MessageBox(hwnd, err_msg, _T("资源加载失败"), MB_OK | MB_ICONERROR);
		return -1;
	}

	const nanoseconds frame_duration(1000000000 / 144);        // 帧数
	steady_clock::time_point last_tick = steady_clock::now();  // 计时

	ExMessage msg;

	menu_scene = new MenuScene();
	game_scene = new GameScene();
	selector_scene = new SelectorScene();

	scene_manager.set_current_scene(selector_scene);  // 初始化为菜单界面

	BeginBatchDraw();

	play_audio(_T("bgm"), true);   // 背景音乐播放

	while (!is_quit)
	{
		while (peekmessage(&msg))
		{
			if (stage == Stage::Waiting)
				continue;

			if (scene_manager.get_type() != SceneManager::SceneType::Selector)
			{
				if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
				{
					cr = "";
					cr_other = "";
					switch (msg.message)
					{
					case WM_KEYDOWN:
						switch (msg.vkcode)
						{
						case 0x41:  // 'A'
							cr += "Ad";
							break;
						case 0x44:  // 'D'
							cr += "Dd";
							break;
						case 0x57:  // 'W'
							cr += "Wd";
							break;
						case 0x4A:  // 'J'
							cr += "Jd";
							break;
						case 0x4B:  // 'K'
							cr += "Kd";
							break;
						}
						break;
					case WM_KEYUP:
						switch (msg.vkcode)
						{
						case 0x41:  // 'A'
							cr += "Au";
							break;
						case 0x44:  // 'D'
							cr += "Du";
							break;
						case 0x57:  // 'W'
							cr += "Wu";
							break;
						case 0x4A:  // 'J'
							cr += "Ju";
							break;
						case 0x4B:  // 'K'
							cr += "Ku";
							break;
						}
						if (msg.vkcode != VK_RETURN)
							cr += "mn";
						break;
					default:
						cr = "mn";
						break;
					}
				}
			}
			else
			{
				if ((cr[1] != 'x' || cr[1] != 'y') && !enter_1)
					cr = "mn";
				if (msg.message == WM_KEYUP)
				{
					switch (msg.vkcode)
					{
					case 0x41:  // 'A'
						type_1 = 1;
						break;
					case 0x44:  // 'D'
						type_1 = 2;
						break;
					case VK_RETURN:
						enter_1 = true;
						cr = "OK";
						break;
					}
				}
			}	
			if (cr == "")
				cr = "mn";
		}

		std::string route = (id == 1) ? "/update_1" : "/update_2";
		std::string body = cr;
		httplib::Result result = client->Post(route, body, "text/plain");

		if (result && result->status == 200)
		{
			cr_other = result->body;

			if (cr_other == "OK")
				enter_2 = true;
		}

		if (stage != Stage::Waiting)
		{
			int cr_length = cr.length();
			int cr_other_length = cr_other.length();
			int i = 0, j = 0;
			while (true)
			{
				if (i >= cr_length && j >= cr_other_length)
					break;
				if (i >= cr_length)
					i -= 2;	
				if (j >= cr_other_length)
					j -= 2;
				std::string cr_0 = { cr[i], cr[i + 1] };
				std::string cr_other_0 = { cr_other[j], cr_other[j + 1] };
				scene_manager.on_input(cr_0, cr_other_0);
				i += 2;
				j += 2;
			}
		}

		steady_clock::time_point frame_start = steady_clock::now();       // 获取当前时间
		duration<float> delta = duration<float>(frame_start - last_tick); // 计算上一次循环到本次循环的时间差

		if (stage == Stage::Waiting)
			if (cr == "Re" && cr_other == "Re")
				stage = Stage::Racing;

		if (enter_1 && enter_2)
		{
			scene_manager.on_update(delta.count());
			CollisionManager::instance()->process_collide();
		}

		setbkcolor(RGB(0, 0, 0));
		cleardevice();

		if (stage == Stage::Waiting)
		{
			settextcolor(RGB(195, 195, 195));
			outtextxy(15, 675, _T("比赛即将开始，等待其他玩家加入..."));
		}
		else
			scene_manager.on_draw();
		//CollisionManager::instance()->on_debug_render();   // 碰撞箱调试

		FlushBatchDraw();

		last_tick = frame_start;    // 更新上一次循环时间
		nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);  // 本次循环是否运行够对应时间
		if (sleep_duration > nanoseconds(0))
			std::this_thread::sleep_for(sleep_duration);  // 休眠以补足不足的时间
	}

	EndBatchDraw();

	return 0;
}