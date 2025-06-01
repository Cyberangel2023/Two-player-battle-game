#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "vector2.h"
#include "animation.h"
#include "collision_box.h"
#include "state_machine.h"
#include "player_id.h"
#include "player_type.h"

#include <string>
#include <graphics.h>
#include <unordered_map>
#include <chrono>
#include <thread>

class Character
{
public:
	Character();
	~Character();

	void decrease_hp();

	PlayerID get_id() const
	{
		return this->id;
	}

	void set_id(PlayerID id)
	{
		this->id = id;
	}

	PlayerType get_type() const
	{
		return this->type;
	}

	void set_type(PlayerType type)
	{
		this->type = type;
	}

	void set_facing_left(bool flag)
	{
		is_facing_left = flag;
	}

	bool get_facing_left() const
	{
		return is_facing_left;
	}

	int get_hp() const
	{
		return hp;
	}

	void set_hp(int hp)
	{
		this->hp = hp;
	}

	void set_position(const Vector2& position)
	{
		this->position = position;
	}

	void set_attack_position(float val)
	{
		this->position.x += val;
	}

	const Vector2& get_position() const
	{
		return position;
	}

	void set_velocity(const Vector2& velocity)
	{
		this->velocity = velocity;
	}

	const Vector2& get_velocity()
	{
		return velocity;
	}

	Vector2 get_logic_center() const
	{
		return Vector2(position.x, position.y - logic_height / 2);
	}

	void set_gravity_enabled(bool flag)
	{
		enable_gravity = flag;
	}

	CollisionBox* get_hit_box()
	{
		return hit_box;
	}

	CollisionBox* get_hurt_box()
	{
		return hurt_box;
	}

	bool is_on_floor() const
	{
		return position.y >= FLOOR_Y;
	}

	float get_floor_y() const
	{
		return FLOOR_Y;
	}

	bool get_render_facing_left() const
	{
		return render_facing_left;
	}

	void set_render_facing_left(bool flag)
	{
		this->render_facing_left = flag;
	}

	void set_can_change(bool flag)
	{
		can_change = flag;
	}

	// 设置无敌
	void make_invulnerable()
	{
		is_invulnerable = true;
		hurt_box->set_enabled(false);
		timer_invulnerable_status.restart();
	}

	virtual void on_input(std::string ch, std::string ch_other);
	virtual void on_update(float delta);
	virtual void on_render();
	virtual void on_hurt();

	void switch_state(const std::string& id);
	void set_animation(const std::string& id);
	void set_slash_animation(const std::string& id);

protected:
	// 左右动画组
	struct AnimationGroup
	{
		Animation left;
		Animation right;
	};

protected:
	const float FLOOR_Y = 720;                                                          // 地板的竖直方向坐标
	const float GRAVITY = 980 * 2;                                                      // 重力大小

	std::chrono::steady_clock::time_point last_tick_run = std::chrono::steady_clock::now(); // 上次更新的时间点
	bool last_left_down = false;                                                          // 上次按键是否向左

protected:
	static Character* player;

	std::string J_is_down;                                          // 是否攻击
	int hurt_num = 0;                                               // 受击次数
	int hp = 100;                                                   // 角色生命值
	PlayerID id = PlayerID::P1;                                     // 玩家序号 ID
	PlayerType type = PlayerType::Yan;                              // 玩家角色 TYPE
	Vector2 position;                                               // 角色脚底位置
	Vector2 velocity;                                               // 角色速度
	float logic_height = 0;                                         // 角色的逻辑高度
	bool is_facing_left = true;                                     // 当前角色是否朝向左
	bool render_facing_left = true;                                 // 当前角色动画朝向
	bool can_change = true;                                         // 当前角色是否可以改变方向
	bool is_stopping = true;                                        // 当前角色是否停止
	bool is_hurting = false;                                        // 当前角色是否受到攻击
	StateMachine state_machine;                                     // 角色逻辑状态机
	bool enable_gravity = true;                                     // 启用重力模拟
	bool is_invulnerable = false;                                   // 当前是否无敌
	Timer timer_invulnerable_blink;                                 // 无敌闪烁状态定时器
	Timer timer_invulnerable_status;                                // 无敌状态定时器
	Timer timer_hurt_num;                                           // 受击重置定时器
	bool is_blink_invisible = false;                                // 当前是否处于闪烁的不可见帧
	CollisionBox* hit_box = nullptr;                                // 攻击碰撞箱
	CollisionBox* hurt_box = nullptr;                               // 受击碰撞箱
	AnimationGroup* current_animation = nullptr;                    // 当前角色动画
	AnimationGroup* current_slash_animation = nullptr;              // 当前角色特效动画
	std::unordered_map<std::string, AnimationGroup> animation_pool; // 角色动画池
};

#endif // !_CHARACTER_H_