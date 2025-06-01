#ifndef _QUIT_GAME_BUTTON_H_
#define _QUIT_GAME_BUTTON_H_

#include "button.h"

extern bool is_quit;

//ÍË³öÓÎÏ·°´Å¥
class QuitGameButton : public Button
{
public:
	QuitGameButton(RECT rect, std::string path_img_idle, std::string path_img_hovered, std::string path_img_pushed)
		: Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}

	~QuitGameButton() = default;

protected:
	void OnClick()
	{
		is_quit = true;
	}
};

#endif // !_QUIT_GAME_BUTTON_H_
