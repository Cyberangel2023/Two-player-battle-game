#ifndef _YAN_PLAYER_H_
#define _YAN_PLAYER_H_

#include "character.h"

class YanPlayer : public Character
{
public:
	YanPlayer();
	~YanPlayer();

	void on_input(std::string ch, std::string ch_other) override;
	void on_update(float delta) override;
	void on_render() override;

	void on_hurt() override;

	// 角色信息
	void set_hp(int val)
	{
		hp = val;
	}

	int get_hp() const
	{
		return hp;
	}

	float get_add_hurt_box_x()
	{
		return add_hurt_box_x;
	}

	void reset_add_hurt_box_x()
	{
		add_hurt_box_x = 0;
	}

	// 受击
	bool get_hurting() const
	{
		return is_hurting;
	}
	
	void set_hurting(bool flag)
	{
		is_hurting = flag;
	}

	// 闪避
	void set_evading(bool flag)
	{
		is_evading = flag;
	}

	bool get_evading() const
	{
		return is_evading;
	}

	bool can_evade() const
	{
		return is_evade_cd_comp && !is_evading && is_evade_key_down;
	}

	// 攻击
	void set_attacking(bool flag)
	{
		is_attacking = flag;
	}

	bool get_attacking() const
	{
		return is_attacking;
	}

	void set_next_attack(bool flag)
	{
		can_next_attack = flag;
	}

	bool get_next_attack()
	{
		return can_next_attack;
	}

	bool can_attack() const
	{
		return !is_attacking && is_attack_key_down && is_on_floor();
	}

	bool can_attack_next() const
	{
		return is_attack_key_down;
	}

	void enter_attack_next(bool flag)
	{
		enter_next_attack = flag;
	}

	bool get_enter_attack_next() const
	{
		return enter_next_attack;
	}

	// 走动
	bool can_walk() const
	{
		return get_move_axis() != 0;
	}

	bool get_walking() const
	{
		return is_walking;
	}

	void set_walking(bool flag)
	{
		is_walking = flag;
	}

	// 奔跑
	bool can_running() const
	{
		return can_run;
	}

	bool get_running() const
	{
		return is_running;
	}

	void set_running(bool flag)
	{
		is_running = flag;
	}

	void set_enter_run(bool flag)
	{
		enter_run = flag;
	}

	void set_accidental_touch(bool flag)
	{
		accidental_touch = flag;
	}

	// 跳跃
	bool can_jump() const
	{
		return once_jump && is_jump_key_down;
	}

	bool get_once_jump() const
	{
		return once_jump;
	}

	void set_once_jump(bool flag)
	{
		once_jump = flag;
	}

	bool get_twice_jump() const
	{
		return twice_jump;
	}

	void set_twice_jump(bool flag)
	{
		twice_jump = flag;
	}

	bool can_jump_again() const
	{
		return twice_jump && !is_jumping_again && !is_on_floor() && is_jump_key_down;
	}

	bool correct_key_jump_again() const
	{
		return !is_jump_key_down;
	}

	void set_jump_again(bool flag)
	{
		is_jumping_again = flag;
	}

	// 跳跃攻击
	bool can_jump_attack() const
	{
		return !is_jump_attacking && !is_on_floor() && is_attack_key_down;
	}

	bool get_jump_attacking() const
	{
		return is_jump_attacking;
	}

	void set_jump_attacking(bool flag)
	{
		is_jump_attacking = flag;
	}

	// 移动方向
	int get_move_axis() const
	{
		if (is_stopping)
			return 0;
		else
			return is_facing_left ? -1 : 1;
	}

	// 攻击序列
	void add_num()
	{
		num = num % 5 + 1;
	}

	void reset_num()
	{
		num = 1;
	}

	int get_num() const
	{
		return num;
	}

	void on_jump();
	void on_jump_again();
	void on_evade();
	//void on_attack();
	//void on_jump_attack();

private:
	const float CD_EVADE = 1.5f;             // 闪避冷却时间
	const float SPEED_WALK = 150.0f;         // 走路速度
	const float SPEED_RUN = 350.0f;          // 奔跑速度
	const float SPEED_EVADE = 1000.0f;       // 闪避速度
	const float SPEED_JUMP = 1000.0f;        // 跳跃速度
	const float SPEED_HURT_X = 20.0f;        // 受击碰撞箱水平速度
	const float SPEED_HURT_Y = 200.0f;       // 受击碰撞箱竖直速度

	Timer timer_evade_cd;                    // 闪避定时器
	bool is_evading = false;                 // 是否闪避
	bool is_evade_cd_comp = true;            // 闪避冷却是否结束

	bool is_attacking = false;               // 是否攻击
	bool can_next_attack = false;            // 是否可以继续攻击
	bool enter_next_attack = false;          // 进入下一击
	int num = 1;                             // 攻击序列

	bool is_jump_attacking = false;          // 是否跳跃时攻击

	bool once_jump = true;                   // 是否第一次跳跃
	bool twice_jump = true;                  // 是否第二次跳跃

	bool is_jumping_again = true;            // 是否连跳
	bool correct_key_jump = false;           // 跳跃按键检查

	bool is_walking = false;                 // 是否走路

	bool can_run = false;                    // 是否可以奔跑
	bool enter_run = false;                  // 进入奔跑
	bool is_running = false;                 // 是否奔跑
	bool accidental_touch = false;           // 误触
 
	float add_hurt_box_x;                    // 碰撞箱移动

	bool is_left_key_down = false;
	bool is_right_key_down = false;
	bool is_jump_key_down = false;
	bool is_evade_key_down = false;
	bool is_attack_key_down = false;
};

#endif // _YAN_PLAYER_H_