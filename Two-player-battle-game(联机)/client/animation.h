#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "util.h"
#include "timer.h"
#include "atlas.h"
#include "vector2.h"

#include <vector>
#include <functional>

class Animation
{
public:
	enum class AnchorMode
	{
		Centered,
		BottomCentered
	};
public:
	Animation()
	{
		timer.set_one_shot(false);
		timer.set_on_timeout(
			[&]()
			{
				idx_frame++;
				if (idx_frame >= frame_list.size())
				{
					idx_frame = is_loop ? 0 : frame_list.size() - 1;
					if (!is_loop && on_finished)
						on_finished();
				}
			}
		);
	}
	~Animation() = default;

	// 重置定时器
	void reset()
	{
		timer.restart();

		idx_frame = 0;
	}

	// 设置锚点
	void set_anchor_mode(AnchorMode mode)
	{
		anchor_mode = mode;
	}

	// 设置位置
	void set_position(const Vector2& position)
	{
		this->position = position;
	}

	// 设置循环
	void set_loop(bool is_loop)
	{
		this->is_loop = is_loop;
	}

	// 设置帧间隔
	void set_interval(float interval)
	{
		timer.set_wait_time(interval);
	}

	// 设置回调
	void set_on_finished(std::function<void()> on_finished)
	{
		this->on_finished = on_finished;
	}

	// 添加帧
	void add_frame(IMAGE* image, int num_h)
	{
		int width = image->getwidth();
		int height = image->getheight();
		int width_frame = width / num_h;

		for (int i = 0; i < num_h; i++)
		{
			Rect rect_src;
			rect_src.x = i * width_frame, rect_src.y = 0;
			rect_src.w = width_frame, rect_src.h = height;

			frame_list.emplace_back(image, rect_src);
		}
	}

	// 动画更新
	void on_update(float delta)
	{
		timer.on_update(delta);
	}

	// 动画渲染
	void on_render()
	{
		const Frame& frame = frame_list[idx_frame];

		Rect rect_dst;
		rect_dst.x = (int)position.x - frame.rect_src.w / 2;
		rect_dst.y = (anchor_mode == AnchorMode::Centered)
			? (int)position.y - frame.rect_src.h / 2 : (int)position.y - frame.rect_src.h + 76;
		rect_dst.w = frame.rect_src.w, rect_dst.h = frame.rect_src.h;

		putimage_ex(frame.image, &rect_dst, &frame.rect_src);
	}

private:
	struct Frame
	{
		Rect rect_src;
		IMAGE* image = nullptr;

		Frame() = default;
		Frame(IMAGE* image, const Rect& rect_src)
			: image(image), rect_src(rect_src) { }

		~Frame() = default;
	};

private:
	Timer timer;                                      // 计时器
	Vector2 position;                                 // 当前位置
	bool is_loop = true;                              // 是否循环
	size_t idx_frame = 0;                             // 帧索引
	std::vector<Frame> frame_list;                    // 图片资源
	std::function<void()> on_finished;                // 触发回调
	AnchorMode anchor_mode = AnchorMode::Centered;    // 锚点模式
};

#endif // !_ANIMATION_H_