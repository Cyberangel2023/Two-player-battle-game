#ifndef _KONG_PLAYER_H_
#define _KONG_PLAYER_H_

#include "character.h"
#include "player_id.h"
#include "player_type.h"

extern Character* player_1;
extern Character* player_2;

class KongPlayer : public Character
{
public:
	KongPlayer();
	~KongPlayer();

	void on_input(const ExMessage& mag) override;
	void on_update(float delta) override;
	void on_render() override;

	void on_hurt() override;

	// ��ɫ��Ϣ
	void set_hp(int val)
	{
		hp = val;
	}

	int get_hp() const
	{
		return hp;
	}

	bool get_facing_left() const
	{
		return is_facing_left;
	}

	// �ܻ�
	bool get_hurting() const
	{
		return is_hurting;
	}

	void set_hurting(bool flag)
	{
		is_hurting = flag;
	}

	// ����
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

	float get_move_evade() const
	{
		return MOVE_EVADE;
	}

	// ����
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

	// �߶�
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

	// ����
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

	// ��Ծ
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

	// ��Ծ����
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

	// �ƶ�����
	int get_move_axis() const
	{
		if (is_stopping)
			return 0;
		else
			return is_facing_left ? -1 : 1;
	}

	// ��������
	void add_num()
	{
		num = num % 4 + 1;
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

public: 
	// �жԽ�ɫ�Ƿ��ڹ�����Χ��
	bool CheckIn()
	{
		if (get_id() == PlayerID::P1)
		{
			if (render_facing_left)
				return player_2->get_position().x >= get_position().x - 350 &&
					player_2->get_position().x <= get_position().x;
			else
				return player_2->get_position().x >= get_position().x &&
				player_2->get_position().x <= get_position().x + 350;
		}
		else if (get_id() == PlayerID::P2)
		{
			if (render_facing_left)
				return player_1->get_position().x >= get_position().x - 350 &&
				player_1->get_position().x <= get_position().x;
			else
				return player_1->get_position().x >= get_position().x &&
				player_1->get_position().x <= get_position().x + 350;
		}
		return false;
	}

private:
	const float CD_EVADE = 1.5f;             // ������ȴʱ��
	const float SPEED_WALK = 150.0f;         // ��·�ٶ�
	const float SPEED_RUN = 350.0f;          // �����ٶ�
	const float MOVE_EVADE = 250.0f;         // �����ƶ�����
	const float SPEED_JUMP = 1000.0f;        // ��Ծ�ٶ�
	const float SPEED_HURT_X = 20.0f;        // �ܻ���ײ��ˮƽ�ٶ�
	const float SPEED_HURT_Y = 200.0f;       // �ܻ���ײ����ֱ�ٶ�

	Timer timer_evade_cd;                    // ���ܶ�ʱ��
	bool is_evading = false;                 // �Ƿ�����
	bool is_evade_cd_comp = true;            // ������ȴ�Ƿ����

	bool is_attacking = false;               // �Ƿ񹥻�
	bool can_next_attack = false;            // �Ƿ���Լ�������
	bool enter_next_attack = false;          // ������һ��
	int num = 1;                             // ��������

	bool is_jump_attacking = false;          // �Ƿ���Ծʱ����

	bool once_jump = true;                   // �Ƿ��һ����Ծ
	bool twice_jump = true;                  // �Ƿ�ڶ�����Ծ

	bool is_jumping_again = true;            // �Ƿ�����
	bool correct_key_jump = false;           // ��Ծ�������

	bool is_walking = false;                 // �Ƿ���·

	bool can_run = false;                    // �Ƿ���Ա���
	bool enter_run = false;                  // ���뱼��
	bool is_running = false;                 // �Ƿ���
	bool accidental_touch = false;           // ��

	bool is_left_key_down = false;
	bool is_right_key_down = false;
	bool is_jump_key_down = false;
	bool is_evade_key_down = false;
	bool is_attack_key_down = false;
};


#endif // _KONG_PLAYER_H_