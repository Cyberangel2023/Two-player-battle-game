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

std::string cr = "mn";           // ������Ϣ
std::string cr_other = "mn";     // ������Ϣ

// �ŵ�λ�� �ٶ� ���� �������� hp ״̬
std::string status_1;         // ��ɫ��Ϣ����
std::string status_2;         // ��ɫ��Ϣ����

int type_1 = 0;               // ��ɫ����
int type_2 = 0;               // ��ɫ����

bool game_start = false;      // ��Ϸ��ʼ

bool enter_1 = false;         // ȷ�Ͽ�ʼ1
bool enter_2 = false;         // ȷ�Ͽ�ʼ2

bool is_quit = false;         // �Ƿ��������
bool is_selector = false;     // �Ƿ�ѡ��

bool ending = false;          // һ����Ϸ�Ƿ����

bool is_player_1 = true;      // �Ƿ�Ϊ��ɫ1

int id = 0;                   // ��ɫID

bool player_1_left = false;   // ��ɫ1�Ƿ�����
bool player_2_left = true;    // ��ɫ2�Ƿ�����

IMAGE* img_player_1_avatar = nullptr;    // ���1ͷ��
IMAGE* img_player_2_avatar = nullptr;    // ���2ͷ��

Scene* menu_scene = nullptr;       // �˵�
Scene* game_scene = nullptr;       // ��Ϸ
Scene* selector_scene = nullptr;   // ѡ��

Character* player_1 = nullptr;     // �������1��ɫ
Character* player_2 = nullptr;     // �������2��ɫ

SceneManager scene_manager;        // ����������

std::string str_address;             // ��������ַ
httplib::Client* client = nullptr;   // HTTP�ͻ��˶���

SOCKET gServer;

enum class Stage
{
	Waiting,    // �ȴ���Ҽ���
	Racing      // ��ʼ��Ϸ
};

Stage stage = Stage::Waiting;    // ��ǰ��Ϸ�׶�

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
		MessageBox(hwnd, L"�޷������� config.cfg", L"����ʧ��", MB_OK | MB_ICONERROR);
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
		MessageBox(hwnd, _T("�޷����ӵ���������"), _T("����ʧ��"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	id = std::stoi(result->body);

	if (id <= 0)
	{
		MessageBox(hwnd, _T("�����Ѿ���ʼ����"), _T("�ܾ�����"), MB_OK | MB_ICONERROR);
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
	SetWindowText(hwnd, _T("������ս"));

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
		_stprintf_s(err_msg, _T("�޷�����: %s"), id);
		MessageBox(hwnd, err_msg, _T("��Դ����ʧ��"), MB_OK | MB_ICONERROR);
		return -1;
	}

	const nanoseconds frame_duration(1000000000 / 144);        // ֡��
	steady_clock::time_point last_tick = steady_clock::now();  // ��ʱ

	ExMessage msg;

	menu_scene = new MenuScene();
	game_scene = new GameScene();
	selector_scene = new SelectorScene();

	scene_manager.set_current_scene(selector_scene);  // ��ʼ��Ϊ�˵�����

	BeginBatchDraw();

	play_audio(_T("bgm"), true);   // �������ֲ���

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

		steady_clock::time_point frame_start = steady_clock::now();       // ��ȡ��ǰʱ��
		duration<float> delta = duration<float>(frame_start - last_tick); // ������һ��ѭ��������ѭ����ʱ���

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
			outtextxy(15, 675, _T("����������ʼ���ȴ�������Ҽ���..."));
		}
		else
			scene_manager.on_draw();
		//CollisionManager::instance()->on_debug_render();   // ��ײ�����

		FlushBatchDraw();

		last_tick = frame_start;    // ������һ��ѭ��ʱ��
		nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);  // ����ѭ���Ƿ����й���Ӧʱ��
		if (sleep_duration > nanoseconds(0))
			std::this_thread::sleep_for(sleep_duration);  // �����Բ��㲻���ʱ��
	}

	EndBatchDraw();

	return 0;
}