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

	// �����޵�
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
	// ���Ҷ�����
	struct AnimationGroup
	{
		Animation left;
		Animation right;
	};

protected:
	const float FLOOR_Y = 720;                                                          // �ذ����ֱ��������
	const float GRAVITY = 980 * 2;                                                      // ������С

	std::chrono::steady_clock::time_point last_tick_run = std::chrono::steady_clock::now(); // �ϴθ��µ�ʱ���
	bool last_left_down = false;                                                          // �ϴΰ����Ƿ�����

protected:
	static Character* player;

	std::string J_is_down;                                          // �Ƿ񹥻�
	int hurt_num = 0;                                               // �ܻ�����
	int hp = 100;                                                   // ��ɫ����ֵ
	PlayerID id = PlayerID::P1;                                     // ������ ID
	PlayerType type = PlayerType::Yan;                              // ��ҽ�ɫ TYPE
	Vector2 position;                                               // ��ɫ�ŵ�λ��
	Vector2 velocity;                                               // ��ɫ�ٶ�
	float logic_height = 0;                                         // ��ɫ���߼��߶�
	bool is_facing_left = true;                                     // ��ǰ��ɫ�Ƿ�����
	bool render_facing_left = true;                                 // ��ǰ��ɫ��������
	bool can_change = true;                                         // ��ǰ��ɫ�Ƿ���Ըı䷽��
	bool is_stopping = true;                                        // ��ǰ��ɫ�Ƿ�ֹͣ
	bool is_hurting = false;                                        // ��ǰ��ɫ�Ƿ��ܵ�����
	StateMachine state_machine;                                     // ��ɫ�߼�״̬��
	bool enable_gravity = true;                                     // ��������ģ��
	bool is_invulnerable = false;                                   // ��ǰ�Ƿ��޵�
	Timer timer_invulnerable_blink;                                 // �޵���˸״̬��ʱ��
	Timer timer_invulnerable_status;                                // �޵�״̬��ʱ��
	Timer timer_hurt_num;                                           // �ܻ����ö�ʱ��
	bool is_blink_invisible = false;                                // ��ǰ�Ƿ�����˸�Ĳ��ɼ�֡
	CollisionBox* hit_box = nullptr;                                // ������ײ��
	CollisionBox* hurt_box = nullptr;                               // �ܻ���ײ��
	AnimationGroup* current_animation = nullptr;                    // ��ǰ��ɫ����
	AnimationGroup* current_slash_animation = nullptr;              // ��ǰ��ɫ��Ч����
	std::unordered_map<std::string, AnimationGroup> animation_pool; // ��ɫ������
};

#endif // !_CHARACTER_H_