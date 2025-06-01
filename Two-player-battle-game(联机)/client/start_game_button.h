#ifndef _START_GAME_BUTTON_H_
#define _START_GAME_BUTTON_H_

#include "button.h"

extern bool is_selector;

//开始游戏按钮
class StartGameButton : public Button
{
public:
	StartGameButton(RECT rect, std::string path_img_idle, std::string path_img_hovered, std::string path_img_pushed)
		: Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}

	~StartGameButton() = default;

protected:
	void OnClick()
	{
		is_selector = true;
	}
};

#endif // !_START_GAME_BUTTON_H_
