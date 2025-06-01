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

bool is_quit = false;         // 是否结束程序
bool is_selector = false;     // 是否选角

bool ending = false;          // 一场游戏是否结束

bool is_player_1 = true;      // 是否为角色1

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

int main(int argc, char** argv)
{
	using namespace std::chrono;

	HWND hwnd = initgraph(1420, 800/*, EW_SHOWCONSOLE*/);
	SetWindowText(hwnd, _T("虫虫大作战"));

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

	scene_manager.set_current_scene(menu_scene);  // 初始化为菜单界面

	BeginBatchDraw();

	play_audio(_T("bgm"), true);   // 背景音乐播放

	while (!is_quit)
	{
		while (peekmessage(&msg))
			scene_manager.on_input(msg);

		steady_clock::time_point frame_start = steady_clock::now();       // 获取当前时间
		duration<float> delta = duration<float>(frame_start - last_tick); // 计算上一次循环到本次循环的时间差

		scene_manager.on_update(delta.count());
		CollisionManager::instance()->process_collide();

		setbkcolor(RGB(0, 0, 0));
		cleardevice();

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