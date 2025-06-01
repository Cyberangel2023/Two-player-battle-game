#ifndef _UTIL_H_
#define _UTIL_H_

#include <graphics.h>

#pragma comment(lib, "WINMM.lib")
#pragma comment(lib, "MSIMG32.lib")

struct Rect
{
	int x, y;  // Õº∆¨◊Û…œΩ«◊Û±ﬂ
	int w, h;  // Õº∆¨øÌ∏ﬂ
};

// ªÊÕº
inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

inline void putimage_ex(IMAGE* img, const Rect* rect_dst, const Rect* rect_src = nullptr)
{
	static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(GetImageHDC(GetWorkingImage()), rect_dst->x, rect_dst->y, rect_dst->w, rect_dst->h,
		GetImageHDC(img), rect_src ? rect_src->x : 0, rect_src ? rect_src->y : 0,
		rect_src ? rect_src->w : img->getwidth(), rect_src ? rect_src->h : img->getheight(), blend_func);
}

// º”‘ÿ“Ù¿÷
inline void load_audio(LPCTSTR path, LPCTSTR id)
{
	static TCHAR str_cmd[512];
	_stprintf_s(str_cmd, _T("open %s alias %s"), path, id);
	mciSendString(str_cmd, NULL, 0, NULL);
}

// ≤•∑≈“Ù¿÷
inline void play_audio(LPCTSTR id, bool is_loop = false)
{
	static TCHAR str_cmd[512];
	_stprintf_s(str_cmd, _T("play %s %s from 0"), id, is_loop ? _T("repeat") : _T(""));
	mciSendString(str_cmd, NULL, 0, NULL);
}

// ‘›Õ£“Ù¿÷
inline void stop_audio(LPCTSTR id)
{
	static TCHAR str_cmd[512];
	_stprintf_s(str_cmd, _T("stop %s"), id);
	mciSendString(str_cmd, NULL, 0, NULL);
}

// ªÒ»°ÀÊª˙ ˝
inline int range_random(int min_num, int max_num)
{
	return min_num + rand() % (max_num - min_num + 1);
}

#endif // !_UTIL_H_