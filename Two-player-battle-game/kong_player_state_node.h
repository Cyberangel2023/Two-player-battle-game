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
	Timer timer;                      // ������ʱ��
	Timer timer_next;                 // ������ʱ��
	Timer timer_attack;               // ������Ч��ʱ��

	bool attack_key_window = false;   // ������������

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
	Timer timer_attack;               // ������Ч��ʱ��

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
	Timer timer;                   // ���ܶ�ʱ��
	Timer timer_move;              // �ƶ���ʱ��
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
	std::chrono::steady_clock::time_point last_tick_run = std::chrono::steady_clock::now(); // �ϴθ��µ�ʱ���
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

	bool can_hit = true;    // �Ƿ��ܻ���
};

#endif // !_KONG_PLAYER_STATE_NODE_H_