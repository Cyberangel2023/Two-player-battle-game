#ifndef _KONG_PLAYER_STATE_NODE_H_
#define _KONG_PLAYER_STATE_NODE_H_

#include "timer.h"
#include "state_node.h"

class KongPlayerAttackState : public StateNode
{
public:
	KongPlayerAttackState();
	~KongPlayerAttackState() = default;

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

class KongPlayerJumpAttackState : public StateNode
{
public:
	KongPlayerJumpAttackState();
	~KongPlayerJumpAttackState() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;

private:
	Timer timer;
	Timer timer_attack;               // 攻击生效定时器

private:
	void update_hit_box_position();
};

class KongPlayerDeadState : public StateNode
{
public:
	KongPlayerDeadState();
	~KongPlayerDeadState() = default;

	void on_enter() override;
	void on_update(float delta) override;

private:
	Timer timer;
};

class KongPlayerFallState : public StateNode
{
public:
	KongPlayerFallState() = default;
	~KongPlayerFallState() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;
};

class KongPlayerIdleState : public StateNode
{
public:
	KongPlayerIdleState() = default;
	~KongPlayerIdleState() = default;

	void on_enter() override;
	void on_update(float delta) override;
};

class KongPlayerJumpState : public StateNode
{
public:
	KongPlayerJumpState() = default;
	~KongPlayerJumpState() = default;

	void on_enter() override;
	void on_update(float delta) override;

private:
	bool shotted = false;
};

class KongPlayerEvadeState : public StateNode
{
public:
	KongPlayerEvadeState();
	~KongPlayerEvadeState() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;

private:
	Timer timer;                   // 闪避定时器
	Timer timer_move;              // 移动定时器
};

class KongPlayerWalkState : public StateNode
{
public:
	KongPlayerWalkState() = default;
	~KongPlayerWalkState() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;
};

class KongPlayerRunState : public StateNode
{
public:
	KongPlayerRunState() = default;
	~KongPlayerRunState() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;

private:
	std::chrono::steady_clock::time_point last_tick_run = std::chrono::steady_clock::now(); // 上次更新的时间点
};

class KongPlayerHitState : public StateNode
{
public:
	KongPlayerHitState();
	~KongPlayerHitState() = default;

	void on_enter() override;
	void on_update(float delta) override;
	void on_exit() override;

private:
	Timer timer;

	bool can_hit = true;    // 是否受击中
};

#endif // !_KONG_PLAYER_STATE_NODE_H_