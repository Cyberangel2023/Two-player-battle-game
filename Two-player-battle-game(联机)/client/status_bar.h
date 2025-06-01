#ifndef _STATUS_BAR_H_
#define _STATUS_BAR_H_

#include "util.h"

class StatusBar
{
public:
	StatusBar() = default;
	~StatusBar() = default;

	void set_avatar(IMAGE* img)
	{
		img_avatar = img;
	}

	void set_position(int x, int y)
	{
		position.x = x, position.y = y;
	}

	void set_hp(int val)
	{
		hp = val;
	}

	void on_draw()
	{
		// 绘制头像
		putimage_alpha(position.x, position.y, img_avatar);
        
		// 绘制边框
		setfillcolor(RGB(5, 5, 5));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3 * 2, position.y + 36, 8, 8); 
        
		// 绘制底色
		setfillcolor(RGB(61, 18, 23));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3, position.y + 33, 8, 8); 
        
		// 绘制血条
		float hp_bar_width = width * std::max(0, hp) / 100.0f;
		setfillcolor(RGB(197, 61, 67));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + (int)hp_bar_width + 3, position.y + 33, 8, 8); 
	}

private:
	const int width = 275;        // 状态条宽度

	int hp = 0;                   // 需要显示的生命值
	POINT position = { 0 };       // 在窗口中显示的位置
	IMAGE* img_avatar = nullptr;  // 角色头像图片
};

#endif // !_STATUS_BAR_H_