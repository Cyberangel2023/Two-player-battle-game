#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "util.h"
#include "status_bar.h"
#include "scene_manager.h"

extern bool ending;

extern IMAGE* img_player_1_avatar;
extern IMAGE* img_player_2_avatar;

extern Character* player_1;
extern Character* player_2;

extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter()
	{
		if (player_1->get_type() == PlayerType::Yan)
			player_1 = (YanPlayer*)player_1;
		else if (player_1->get_type() == PlayerType::Kong)
			player_1 = (KongPlayer*)player_1;
		else
			player_1 = (YanPlayer*)player_1;

		if (player_2->get_type() == PlayerType::Yan)
			player_2 = (YanPlayer*)player_2;
		else if (player_2->get_type() == PlayerType::Kong)
			player_2 = (KongPlayer*)player_2;
		else
			player_2 = (KongPlayer*)player_2;

		img_background = ResourcesManager::instance()->find_image("background");
		rect_dst =
		{
			(getwidth() - img_background->getwidth()) / 2,
			(getheight() - img_background->getheight()) / 2,
			img_background->getwidth(),
			img_background->getheight(),
		};

		img_winner_bar = ResourcesManager::instance()->find_image("winner_bar");
		img_1P_winner = ResourcesManager::instance()->find_image("1P_winner");
		img_2P_winner = ResourcesManager::instance()->find_image("2P_winner");
		img_draw = ResourcesManager::instance()->find_image("draw");

		is_game_over = false;

		status_bar_1P.set_avatar(img_player_1_avatar);
		status_bar_2P.set_avatar(img_player_2_avatar);

		status_bar_1P.set_position(35, 20);
		status_bar_2P.set_position(1015, 20);
	}

	void on_update(float delta)
	{
		CharacterManager::instance()->on_update(delta);

		const Vector2& position_player_1 = player_1->get_position();
		const Vector2& position_player_2 = player_2->get_position();

		// 掉出屏幕外
		if (position_player_1.y >= getheight() && !is_game_over)
			player_1->set_hp(0);
		if (position_player_2.y >= getheight() && !is_game_over)
			player_2->set_hp(0);
		if (player_1->get_hp() <= 0 || player_2->get_hp() <= 0)
			is_game_over = true;

		// 更新血条
		status_bar_1P.set_hp(player_1->get_hp());
		status_bar_2P.set_hp(player_2->get_hp());

		// 游戏结束
		if (is_game_over && ending)
		{
			if (player_1->get_hp() <= 0 && player_2->get_hp() > 0)
				MessageBox(GetHWnd(), _T("玩家2获胜!"), _T("虫虫大作战"), MB_OK);
			else if (player_2->get_hp() <= 0 && player_1->get_hp() > 0)
				MessageBox(GetHWnd(), _T("玩家1获胜！"), _T("虫虫大作战"), MB_OK);
			else if (player_1->get_hp() <= 0 && player_2->get_hp() <= 0)
				MessageBox(GetHWnd(), _T("平局！"), _T("虫虫大作战"), MB_OK);
			ending = false;
			scene_manager.switch_to(SceneManager::SceneType::Menu);
		}
	}

	void on_draw()
	{
		putimage_ex(img_background, &rect_dst);

		CharacterManager::instance()->on_render();

		// 绘制血条
		status_bar_1P.on_draw();
		status_bar_2P.on_draw();
	}

	void on_input(const ExMessage& msg)
	{
		CharacterManager::instance()->on_input(msg);
	}

	void on_exit()
	{
		delete player_1; player_1 = nullptr;
		delete player_2; player_2 = nullptr;
	}

private:
	const float speed_winnner_bar = 3.0f;    // 结算动效背景移动速度
	const float speed_winnner_text = 1.5f;   // 获胜结算动效背景移动速度
	const float speed_draw_text = 1.7f;      // 平局结算动效背景移动速度

private:
	IMAGE* img_background;
	Rect rect_dst;

	IMAGE* img_winner_bar;
	IMAGE* img_1P_winner;
	IMAGE* img_2P_winner;
	IMAGE* img_draw;

	StatusBar status_bar_1P;            // 玩家 1 的状态条
	StatusBar status_bar_2P;            // 玩家 2 的状态条

	bool is_game_over = false;          // 游戏是否结束
};

#endif // !_GAME_SCENE_H_