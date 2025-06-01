#ifndef _YAN_PlAYER_STATE_NODE_H_
#define _YAN_PlAYER_STATE_NODE_H_

#include "timer.h"
#include "state_node.h"

class YanPlayerAttackState : public StateNode
{
public:
	YanPlayerAttackState();
	~YanPlayerAttackState() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;

private:
	Timer timer;                      // 攻击定时器
	Timer timer_next;                 // 连击定时器
	Timer timer_attack;               // 攻击生效定时器
	
	bool attack_key_window = false;   // 连击按键窗口

private:
	void update_hit_box_position();
};

class YanPlayerJumpAttackState : public StateNode
{
public:
	YanPlayerJumpAttackState();
	~YanPlayerJumpAttackState() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;

private:
	Timer timer;
	Timer timer_attack;

private:
	void update_hit_box_position();
};

class YanPlayerDeadState : public StateNode
{
public:
	YanPlayerDeadState();
	~YanPlayerDeadState() = default;

	void on_enter() override;
	void on_update(float delta) override;

private:
	Timer timer;
};

class YanPlayerFallState : public StateNode
{
public:
	YanPlayerFallState() = default;
	~YanPlayerFallState() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;
};

class YanPlayerIdleState : public StateNode
{
public:
	YanPlayerIdleState() = default;
	~YanPlayerIdleState() = default;

	void on_enter() override;
	void on_update(float delta) override;
};

class YanPlayerJumpState : public StateNode
{
public:
	YanPlayerJumpState() = default;
	~YanPlayerJumpState() = default;

	void on_enter() override;
	void on_update(float delta) override;

private:
	bool shotted = false;
};

class YanPlayerEvadeState : public StateNode
{
public:
	YanPlayerEvadeState();
	~YanPlayerEvadeState() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;

private:
	Timer timer;
};

class YanPlayerWalkState : public StateNode
{
public:
	YanPlayerWalkState() = default;
	~YanPlayerWalkState() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;
};

class YanPlayerRunState : public StateNode
{
public:
	YanPlayerRunState() = default;
	~YanPlayerRunState() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;

private:
	std::chrono::steady_clock::time_point last_tick_run = std::chrono::steady_clock::now(); // 上次更新的时间点
};

class YanPlayerHitState : public StateNode
{
public:
	YanPlayerHitState();
	~YanPlayerHitState() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;

private:
	Timer timer;
	bool can_hit = true;   // 受击中
};

#endif // !_YAN_PlAYER_STATE_NODE_H_