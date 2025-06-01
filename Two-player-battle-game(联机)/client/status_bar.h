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
		// ����ͷ��
		putimage_alpha(position.x, position.y, img_avatar);
        
		// ���Ʊ߿�
		setfillcolor(RGB(5, 5, 5));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3 * 2, position.y + 36, 8, 8); 
        
		// ���Ƶ�ɫ
		setfillcolor(RGB(61, 18, 23));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3, position.y + 33, 8, 8); 
        
		// ����Ѫ��
		float hp_bar_width = width * std::max(0, hp) / 100.0f;
		setfillcolor(RGB(197, 61, 67));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + (int)hp_bar_width + 3, position.y + 33, 8, 8); 
	}

private:
	const int width = 275;        // ״̬�����

	int hp = 0;                   // ��Ҫ��ʾ������ֵ
	POINT position = { 0 };       // �ڴ�������ʾ��λ��
	IMAGE* img_avatar = nullptr;  // ��ɫͷ��ͼƬ
};

#endif // !_STATUS_BAR_H_