#include <graphics.h>
#include <chrono>
#include <thread>

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

bool is_quit = false;         // �Ƿ��������
bool is_selector = false;     // �Ƿ�ѡ��

bool ending = false;          // һ����Ϸ�Ƿ����

bool is_player_1 = true;      // �Ƿ�Ϊ��ɫ1

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

int main(int argc, char** argv)
{
	using namespace std::chrono;

	HWND hwnd = initgraph(1420, 800/*, EW_SHOWCONSOLE*/);
	SetWindowText(hwnd, _T("������ս"));

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

	scene_manager.set_current_scene(menu_scene);  // ��ʼ��Ϊ�˵�����

	BeginBatchDraw();

	play_audio(_T("bgm"), true);   // �������ֲ���

	while (!is_quit)
	{
		while (peekmessage(&msg))
			scene_manager.on_input(msg);

		steady_clock::time_point frame_start = steady_clock::now();       // ��ȡ��ǰʱ��
		duration<float> delta = duration<float>(frame_start - last_tick); // ������һ��ѭ��������ѭ����ʱ���

		scene_manager.on_update(delta.count());
		CollisionManager::instance()->process_collide();

		setbkcolor(RGB(0, 0, 0));
		cleardevice();

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