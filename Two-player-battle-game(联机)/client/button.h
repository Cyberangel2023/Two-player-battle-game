#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "util.h"
#include "resources_manager.h"

#include <string>

class Button
{
public:
	Button(RECT rect, std::string path_img_idle, std::string path_img_hovered, std::string path_img_pushed)
	{
		// 初始化
		reset();
		region = rect;

		img_ui_idle = ResourcesManager::instance()->find_image(path_img_idle);
		img_ui_hovered = ResourcesManager::instance()->find_image(path_img_hovered);
		img_ui_pushed = ResourcesManager::instance()->find_image(path_img_pushed);
	}

	~Button() = default;

	// 重置状态
	void reset() 
	{
		status = Status::Idgle;
	}

	void on_draw()
	{
		switch (status)
		{
		case Status::Idgle:
			putimage_alpha(region.left, region.top, img_ui_idle);
			break;
		case Status::Hovered:
			putimage_alpha(region.left, region.top, img_ui_hovered);
			break;
		case Status::Pushed:
			putimage_alpha(region.left, region.top, img_ui_pushed);
			break;
		}
	}

	void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_MOUSEMOVE:
			// 检查鼠标位置是否超出窗口
			if (msg.x <= 0 || msg.x >= getwidth() || msg.y <= 0 || msg.y >= getheight())
			{	
				reset();
				return;
			}	
			if (status == Status::Idgle && CheckCursorHit(msg.x, msg.y))
				status = Status::Hovered;
			else if (status == Status::Hovered && !CheckCursorHit(msg.x, msg.y))
				status = Status::Idgle;
			break;
		case WM_LBUTTONDOWN:
			// 检查鼠标位置是否超出窗口
			if (msg.x <= 0 || msg.x >= getwidth() || msg.y <= 0 || msg.y >= getheight())
			{
				reset();
				return;
			}
			if (CheckCursorHit(msg.x, msg.y))
				status = Status::Pushed;
			break;
		case WM_LBUTTONUP:
			if (status == Status::Pushed && CheckCursorHit(msg.x, msg.y))
				OnClick();
			reset();
			break;
		default:
			break;
		}
	}

protected:
	virtual void OnClick() = 0;

public:
	enum class Status
	{
		Idgle = 0,
		Hovered,
		Pushed
	};

private:
	RECT region;

	IMAGE* img_ui_idle;
	IMAGE* img_ui_hovered;
	IMAGE* img_ui_pushed;

private:
	// 检查鼠标位置是否在按钮上
	bool CheckCursorHit(int x, int y)
	{
		return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;
	}

public:
	Status status = Status::Idgle;
};

#endif // !_BUTTON_H_