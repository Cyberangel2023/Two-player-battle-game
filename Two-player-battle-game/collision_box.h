#ifndef _COLLISION_BOX_H_
#define _COLLISION_BOX_H_

#include "vector2.h"
#include "collision_layer.h"

#include <functional>

class CollisionManager;

class CollisionBox
{
	friend class CollisionManager;

public:
	void set_enabled(bool flag)
	{
		enabled = flag;
	}

	void set_layer_src(CollisionLayer layer)
	{
		layer_src = layer;
	}

	void set_layer_dst(CollisionLayer layer)
	{
		layer_dst = layer;
	}

	void set_on_collide(std::function<void()> on_collide)
	{
		this->on_collide = on_collide;
	}

	void set_size(const Vector2& size)
	{
		this->size = size;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	void set_position(const Vector2& position)
	{
		this->position = position;
	}

	void add_position_x(float val)
	{
		this->position.x += val;
	}

private:
	Vector2 size;                                     // 碰撞箱尺寸
	Vector2 position;                                 // 碰撞箱中心位置
	bool enabled = true;                              // 是否生效
	std::function<void()> on_collide;                 // 触发回调
	CollisionLayer layer_src = CollisionLayer::None;  // 源层
	CollisionLayer layer_dst = CollisionLayer::None;  // 目标层

private:
	CollisionBox() = default;
	~CollisionBox() = default;
};

#endif // !_COLLISION_BOX_H_