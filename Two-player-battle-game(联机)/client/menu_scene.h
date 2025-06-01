#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "scene_manager.h"
#include "button.h"

extern bool is_selector;

extern SceneManager scene_manager;

class MenuScene : public Scene
{
public:
	MenuScene()
	{
		img_menu_background = ResourcesManager::instance()->find_image("menu");
	}
	~MenuScene() = default;

	void on_enter() { }

	void on_draw()
	{
		putimage(0, 0, img_menu_background);

		btn_start_game.on_draw();
		btn_quit_game.on_draw();
	}

	void on_input(const ExMessage& msg)
	{
		btn_start_game.on_input(msg);
		btn_quit_game.on_input(msg);		
		
		if (is_selector)
			scene_manager.switch_to(SceneManager::SceneType::Selector);
	}

private:
	IMAGE* img_menu_background;

	// 按钮位置设置
	RECT set_btn_start()
	{
		static IMAGE* img_ui_start_idle = ResourcesManager::instance()->find_image("ui_start_idle");

		RECT rect;
		rect.left = (getwidth() - img_ui_start_idle->getwidth()) / 2;
		rect.right = (getwidth() + img_ui_start_idle->getwidth()) / 2;
		rect.top = 500;
		rect.bottom = img_ui_start_idle->getheight() + 500;

		return rect;
	}

	RECT set_btn_quit()
	{
		static IMAGE* img_ui_quit_idle = ResourcesManager::instance()->find_image("ui_quit_idle");

		RECT rect;
		rect.left = (getwidth() - img_ui_quit_idle->getwidth()) / 2;
		rect.right = (getwidth() + img_ui_quit_idle->getwidth()) / 2;
		rect.top = 600;
		rect.bottom = img_ui_quit_idle->getheight() + 600;

		return rect;
	}

	// 按钮初始化
	RECT region_btn_start_game = set_btn_start();
	RECT region_btn_quit_game = set_btn_quit();

	StartGameButton btn_start_game = StartGameButton(region_btn_start_game, "ui_start_idle",
		"ui_start_hovered", "ui_start_pushed");
	QuitGameButton btn_quit_game = QuitGameButton(region_btn_quit_game, "ui_quit_idle",
		"ui_quit_hovered", "ui_quit_pushed");
};
#endif // !_MENU_SCENE_H_