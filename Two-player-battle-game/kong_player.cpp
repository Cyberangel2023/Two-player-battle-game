#include "kong_player.h"
#include "resources_manager.h"
#include "kong_player_state_node.h"

#include <cmath>

extern bool player_1_left;
extern bool player_2_left;

extern Character* player_1;
extern Character* player_2;

KongPlayer::KongPlayer()
{
	is_facing_left = false;
	logic_height = 123;

	hit_box->set_size({ 70, 140 });
	hurt_box->set_size({ 70, 120 });

	hit_box->set_enabled(false);
	hit_box->set_on_collide([&]()
		{
			hit_box->set_enabled(false);
		});
	hurt_box->set_on_collide([&]()
		{
			is_hurting = true;
			decrease_hp();
		});

	timer_evade_cd.set_wait_time(CD_EVADE);
	timer_evade_cd.set_one_shot(true);
	timer_evade_cd.set_on_timeout([&]()
		{
			is_evade_cd_comp = true;
		});

	// 设置图集资源
	{
		{
			AnimationGroup& animation_kong_attack_1 = animation_pool["kong_attack_1"];

			Animation& animation_kong_attack_1_left = animation_kong_attack_1.left;
			animation_kong_attack_1_left.set_interval(0.033f);
			animation_kong_attack_1_left.set_loop(false);
			animation_kong_attack_1_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_attack_1_left.add_frame(ResourcesManager::instance()->find_image("kong_attack_1_left"), 13);

			Animation& animation_kong_attack_1_right = animation_kong_attack_1.right;
			animation_kong_attack_1_right.set_interval(0.033f);
			animation_kong_attack_1_right.set_loop(false);
			animation_kong_attack_1_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_attack_1_right.add_frame(ResourcesManager::instance()->find_image("kong_attack_1_right"), 13);

			AnimationGroup& animation_kong_attack_2 = animation_pool["kong_attack_2"];

			Animation& animation_kong_attack_2_left = animation_kong_attack_2.left;
			animation_kong_attack_2_left.set_interval(0.033f);
			animation_kong_attack_2_left.set_loop(false);
			animation_kong_attack_2_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_attack_2_left.add_frame(ResourcesManager::instance()->find_image("kong_attack_2_left"), 13);

			Animation& animation_kong_attack_2_right = animation_kong_attack_2.right;
			animation_kong_attack_2_right.set_interval(0.033f);
			animation_kong_attack_2_right.set_loop(false);
			animation_kong_attack_2_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_attack_2_right.add_frame(ResourcesManager::instance()->find_image("kong_attack_2_right"), 13);

			AnimationGroup& animation_kong_attack_3 = animation_pool["kong_attack_3"];

			Animation& animation_kong_attack_3_left = animation_kong_attack_3.left;
			animation_kong_attack_3_left.set_interval(0.033f);
			animation_kong_attack_3_left.set_loop(false);
			animation_kong_attack_3_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_attack_3_left.add_frame(ResourcesManager::instance()->find_image("kong_attack_3_left"), 13);

			Animation& animation_kong_attack_3_right = animation_kong_attack_3.right;
			animation_kong_attack_3_right.set_interval(0.033f);
			animation_kong_attack_3_right.set_loop(false);
			animation_kong_attack_3_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_attack_3_right.add_frame(ResourcesManager::instance()->find_image("kong_attack_3_right"), 13);

			AnimationGroup& animation_kong_attack_4 = animation_pool["kong_attack_4"];

			Animation& animation_kong_attack_4_left = animation_kong_attack_4.left;
			animation_kong_attack_4_left.set_interval(0.033f);
			animation_kong_attack_4_left.set_loop(false);
			animation_kong_attack_4_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_attack_4_left.add_frame(ResourcesManager::instance()->find_image("kong_attack_4_left"), 13);

			Animation& animation_kong_attack_4_right = animation_kong_attack_4.right;
			animation_kong_attack_4_right.set_interval(0.033f);
			animation_kong_attack_4_right.set_loop(false);
			animation_kong_attack_4_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_attack_4_right.add_frame(ResourcesManager::instance()->find_image("kong_attack_4_right"), 13);

			AnimationGroup& animation_kong_jump_attack = animation_pool["kong_jump_attack"];

			Animation& animation_kong_jump_attack_left = animation_kong_jump_attack.left;
			animation_kong_jump_attack_left.set_interval(0.033f);
			animation_kong_jump_attack_left.set_loop(false);
			animation_kong_jump_attack_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_jump_attack_left.add_frame(ResourcesManager::instance()->find_image("kong_jump_attack_left"), 13);

			Animation& animation_kong_jump_attack_right = animation_kong_jump_attack.right;
			animation_kong_jump_attack_right.set_interval(0.033f);
			animation_kong_jump_attack_right.set_loop(false);
			animation_kong_jump_attack_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_jump_attack_right.add_frame(ResourcesManager::instance()->find_image("kong_jump_attack_right"), 13);
		}
		{
			AnimationGroup& animation_kong_dead = animation_pool["kong_dead"];

			Animation& animation_kong_dead_left = animation_kong_dead.left;
			animation_kong_dead_left.set_interval(0.033f);
			animation_kong_dead_left.set_loop(false);
			animation_kong_dead_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_dead_left.add_frame(ResourcesManager::instance()->find_image("kong_dead_left"), 41);

			Animation& animation_kong_dead_right = animation_kong_dead.right;
			animation_kong_dead_right.set_interval(0.033f);
			animation_kong_dead_right.set_loop(false);
			animation_kong_dead_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_dead_right.add_frame(ResourcesManager::instance()->find_image("kong_dead_right"), 41);
		}
		{
			AnimationGroup& animation_kong_idle = animation_pool["kong_idle"];

			Animation& animation_kong_idle_left = animation_kong_idle.left;
			animation_kong_idle_left.set_interval(0.033f);
			animation_kong_idle_left.set_loop(true);
			animation_kong_idle_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_idle_left.add_frame(ResourcesManager::instance()->find_image("kong_idle_left"), 49);

			Animation& animation_kong_idle_right = animation_kong_idle.right;
			animation_kong_idle_right.set_interval(0.033f);
			animation_kong_idle_right.set_loop(true);
			animation_kong_idle_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_idle_right.add_frame(ResourcesManager::instance()->find_image("kong_idle_right"), 49);
		}
		{
			AnimationGroup& animation_kong_jump = animation_pool["kong_jump"];

			Animation& animation_kong_jump_left = animation_kong_jump.left;
			animation_kong_jump_left.set_interval(0.033f);
			animation_kong_jump_left.set_loop(false);
			animation_kong_jump_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_jump_left.add_frame(ResourcesManager::instance()->find_image("kong_jump_left"), 1);

			Animation& animation_kong_jump_right = animation_kong_jump.right;
			animation_kong_jump_right.set_interval(0.033f);
			animation_kong_jump_right.set_loop(false);
			animation_kong_jump_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_jump_right.add_frame(ResourcesManager::instance()->find_image("kong_jump_right"), 1);
		}
		{
			AnimationGroup& animation_kong_jump_attack = animation_pool["kong_jump_attack"];

			Animation& animation_kong_jump_attack_left = animation_kong_jump_attack.left;
			animation_kong_jump_attack_left.set_interval(0.033f);
			animation_kong_jump_attack_left.set_loop(false);
			animation_kong_jump_attack_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_jump_attack_left.add_frame(ResourcesManager::instance()->find_image("kong_jump_attack_left"), 13);

			Animation& animation_kong_jump_attack_right = animation_kong_jump_attack.right;
			animation_kong_jump_attack_right.set_interval(0.033f);
			animation_kong_jump_attack_right.set_loop(false);
			animation_kong_jump_attack_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_jump_attack_right.add_frame(ResourcesManager::instance()->find_image("kong_jump_attack_right"), 13);
		}
		{
			AnimationGroup& animation_kong_evade = animation_pool["kong_evade"];

			Animation& animation_kong_evade_left = animation_kong_evade.left;
			animation_kong_evade_left.set_interval(0.033f);
			animation_kong_evade_left.set_loop(false);
			animation_kong_evade_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_evade_left.add_frame(ResourcesManager::instance()->find_image("kong_evade_left"), 12);

			Animation& animation_kong_evade_right = animation_kong_evade.right;
			animation_kong_evade_right.set_interval(0.033f);
			animation_kong_evade_right.set_loop(false);
			animation_kong_evade_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_evade_right.add_frame(ResourcesManager::instance()->find_image("kong_evade_right"), 12);
		}
		{
			AnimationGroup& animation_kong_run = animation_pool["kong_run"];

			Animation& animation_kong_run_left = animation_kong_run.left;
			animation_kong_run_left.set_interval(0.033f);
			animation_kong_run_left.set_loop(true);
			animation_kong_run_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_run_left.add_frame(ResourcesManager::instance()->find_image("kong_run_left"), 19);

			Animation& animation_kong_run_right = animation_kong_run.right;
			animation_kong_run_right.set_interval(0.033f);
			animation_kong_run_right.set_loop(true);
			animation_kong_run_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_run_right.add_frame(ResourcesManager::instance()->find_image("kong_run_right"), 19);
		}
		{
			AnimationGroup& animation_kong_walk = animation_pool["kong_walk"];

			Animation& animation_kong_walk_left = animation_kong_walk.left;
			animation_kong_walk_left.set_interval(0.033f);
			animation_kong_walk_left.set_loop(true);
			animation_kong_walk_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_walk_left.add_frame(ResourcesManager::instance()->find_image("kong_walk_left"), 33);

			Animation& animation_kong_walk_right = animation_kong_walk.right;
			animation_kong_walk_right.set_interval(0.033f);
			animation_kong_walk_right.set_loop(true);
			animation_kong_walk_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_walk_right.add_frame(ResourcesManager::instance()->find_image("kong_walk_right"), 33);
		}
		{
			AnimationGroup& animation_kong_hit = animation_pool["kong_hit"];

			Animation& animation_kong_hit_left = animation_kong_hit.left;
			animation_kong_hit_left.set_interval(0.033f);
			animation_kong_hit_left.set_loop(false);
			animation_kong_hit_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_hit_left.add_frame(ResourcesManager::instance()->find_image("kong_hit_left"), 15);

			Animation& animation_kong_hit_right = animation_kong_hit.right;
			animation_kong_hit_right.set_interval(0.033f);
			animation_kong_hit_right.set_loop(false);
			animation_kong_hit_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_kong_hit_right.add_frame(ResourcesManager::instance()->find_image("kong_hit_right"), 15);
		}
		{
			AnimationGroup& animation_kong_vfx_attack_1 = animation_pool["kong_vfx_attack_1"];

			Animation& animation_kong_vfx_attack_1_left = animation_kong_vfx_attack_1.left;
			animation_kong_vfx_attack_1_left.set_interval(0.033f);
			animation_kong_vfx_attack_1_left.set_loop(false);
			animation_kong_vfx_attack_1_left.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_kong_vfx_attack_1_left.add_frame(ResourcesManager::instance()->find_image("kong_vfx_attack_1_left"), 13);

			Animation& animation_kong_vfx_attack_1_right = animation_kong_vfx_attack_1.right;
			animation_kong_vfx_attack_1_right.set_interval(0.033f);
			animation_kong_vfx_attack_1_right.set_loop(false);
			animation_kong_vfx_attack_1_right.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_kong_vfx_attack_1_right.add_frame(ResourcesManager::instance()->find_image("kong_vfx_attack_1_right"), 13);

			AnimationGroup& animation_kong_vfx_attack_2 = animation_pool["kong_vfx_attack_2"];

			Animation& animation_kong_vfx_attack_2_left = animation_kong_vfx_attack_2.left;
			animation_kong_vfx_attack_2_left.set_interval(0.033f);
			animation_kong_vfx_attack_2_left.set_loop(false);
			animation_kong_vfx_attack_2_left.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_kong_vfx_attack_2_left.add_frame(ResourcesManager::instance()->find_image("kong_vfx_attack_2_left"), 13);

			Animation& animation_kong_vfx_attack_2_right = animation_kong_vfx_attack_2.right;
			animation_kong_vfx_attack_2_right.set_interval(0.033f);
			animation_kong_vfx_attack_2_right.set_loop(false);
			animation_kong_vfx_attack_2_right.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_kong_vfx_attack_2_right.add_frame(ResourcesManager::instance()->find_image("kong_vfx_attack_2_right"), 13);

			AnimationGroup& animation_kong_vfx_attack_3 = animation_pool["kong_vfx_attack_3"];

			Animation& animation_kong_vfx_attack_3_left = animation_kong_vfx_attack_3.left;
			animation_kong_vfx_attack_3_left.set_interval(0.033f);
			animation_kong_vfx_attack_3_left.set_loop(false);
			animation_kong_vfx_attack_3_left.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_kong_vfx_attack_3_left.add_frame(ResourcesManager::instance()->find_image("kong_vfx_attack_3_left"), 13);

			Animation& animation_kong_vfx_attack_3_right = animation_kong_vfx_attack_3.right;
			animation_kong_vfx_attack_3_right.set_interval(0.033f);
			animation_kong_vfx_attack_3_right.set_loop(false);
			animation_kong_vfx_attack_3_right.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_kong_vfx_attack_3_right.add_frame(ResourcesManager::instance()->find_image("kong_vfx_attack_3_right"), 13);

			AnimationGroup& animation_kong_vfx_attack_4 = animation_pool["kong_vfx_attack_4"];

			Animation& animation_kong_vfx_attack_4_left = animation_kong_vfx_attack_4.left;
			animation_kong_vfx_attack_4_left.set_interval(0.033f);
			animation_kong_vfx_attack_4_left.set_loop(false);
			animation_kong_vfx_attack_4_left.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_kong_vfx_attack_4_left.add_frame(ResourcesManager::instance()->find_image("kong_vfx_attack_4_left"), 13);

			Animation& animation_kong_vfx_attack_4_right = animation_kong_vfx_attack_4.right;
			animation_kong_vfx_attack_4_right.set_interval(0.033f);
			animation_kong_vfx_attack_4_right.set_loop(false);
			animation_kong_vfx_attack_4_right.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_kong_vfx_attack_4_right.add_frame(ResourcesManager::instance()->find_image("kong_vfx_attack_4_right"), 13);

			AnimationGroup& animation_kong_vfx_jump_attack = animation_pool["kong_vfx_jump_attack"];

			Animation& animation_kong_vfx_jump_attack_left = animation_kong_vfx_jump_attack.left;
			animation_kong_vfx_jump_attack_left.set_interval(0.033f);
			animation_kong_vfx_jump_attack_left.set_loop(false);
			animation_kong_vfx_jump_attack_left.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_kong_vfx_jump_attack_left.add_frame(ResourcesManager::instance()->find_image("kong_vfx_jump_attack_left"), 13);

			Animation& animation_kong_vfx_jump_attack_right = animation_kong_vfx_jump_attack.right;
			animation_kong_vfx_jump_attack_right.set_interval(0.033f);
			animation_kong_vfx_jump_attack_right.set_loop(false);
			animation_kong_vfx_jump_attack_right.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_kong_vfx_jump_attack_right.add_frame(ResourcesManager::instance()->find_image("kong_vfx_jump_attack_right"), 13);
		}
	}
	{
		// TODO: 状态机初始化
		state_machine.register_state("kong_attack", new KongPlayerAttackState());
		state_machine.register_state("kong_dead", new KongPlayerDeadState());
		state_machine.register_state("kong_walk", new KongPlayerWalkState());
		state_machine.register_state("kong_run", new KongPlayerRunState());
		state_machine.register_state("kong_idle", new KongPlayerIdleState());
		state_machine.register_state("kong_jump", new KongPlayerJumpState());
		state_machine.register_state("kong_jump_attack", new KongPlayerJumpAttackState());
		state_machine.register_state("kong_fall", new KongPlayerFallState());
		state_machine.register_state("kong_evade", new KongPlayerEvadeState());
		state_machine.register_state("kong_hit", new KongPlayerHitState());

		state_machine.set_entry("kong_idle");
	}
}

KongPlayer::~KongPlayer() = default;

void KongPlayer::on_input(const ExMessage& msg)
{
	if (hp <= 0) return;

	auto current_tick_run = std::chrono::steady_clock::now(); // 获取当前时间点
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_tick_run - last_tick_run); // 计算时间差
	float time_diff_seconds = 0;

	switch (msg.message)
	{
	case WM_KEYDOWN:
		switch (id)
		{
		case PlayerID::P1:
			switch (msg.vkcode)
			{
			case 0x41:  // 'A'
				last_tick_run = current_tick_run; // 更新时间点
				time_diff_seconds = static_cast<float>(duration.count()) / 1000.0f; // 将时间差转换为秒
				if (time_diff_seconds < 0.5)
					if (last_left_down && !is_left_key_down && (enter_run || accidental_touch))
						can_run = true;
				is_left_key_down = true;
				is_facing_left = true;
				is_stopping = false;
				last_left_down = true;
				break;
			case 0x44:  // 'D'
				last_tick_run = current_tick_run; // 更新时间点
				time_diff_seconds = static_cast<float>(duration.count()) / 1000.0f; // 将时间差转换为秒
				if (time_diff_seconds < 0.5)
					if (!last_left_down && !is_right_key_down && (enter_run || accidental_touch))
						can_run = true;
				is_right_key_down = true;
				is_facing_left = false;
				is_stopping = false;
				last_left_down = false;
				break;
			case 0x57:  // 'W'
				is_jump_key_down = true;
				break;
			case 0x4A:  // 'J'
				is_attack_key_down = true;
				break;
			case 0x4B:  // 'K'
				is_evade_key_down = true;
				break;
			}
			break;
		case PlayerID::P2:
			switch (msg.vkcode)
			{
			case VK_LEFT:     // '←'
				last_tick_run = current_tick_run; // 更新时间点
				time_diff_seconds = static_cast<float>(duration.count()) / 1000.0f; // 将时间差转换为秒
				if (time_diff_seconds < 0.5)
					if (last_left_down && !is_left_key_down && (enter_run || accidental_touch))
						can_run = true;
				is_left_key_down = true;
				is_facing_left = true;
				is_stopping = false;
				last_left_down = true;
				break;
			case VK_RIGHT:    // '→'
				last_tick_run = current_tick_run; // 更新时间点
				time_diff_seconds = static_cast<float>(duration.count()) / 1000.0f; // 将时间差转换为秒
				if (time_diff_seconds < 0.5)
					if (!last_left_down && !is_right_key_down && (enter_run || accidental_touch))
						can_run = true;
				is_right_key_down = true;
				is_facing_left = false;
				is_stopping = false;
				last_left_down = false;
				break;
			case VK_UP:       // '↑'
				is_jump_key_down = true;
				break;
			case VK_NUMPAD1:  // '1'
				is_attack_key_down = true;
				break;
			case VK_NUMPAD2:  // '2'
				is_evade_key_down = true;
				break;
			}
			break;
		}
		break;
	case WM_KEYUP:
		switch (id)
		{
		case PlayerID::P1:
			switch (msg.vkcode)
			{
			case 0x41:  // 'A'
				is_left_key_down = false;
				break;
			case 0x44:  // 'D'
				is_right_key_down = false;
				break;
			case 0x57:  // 'W'
				is_jump_key_down = false;
				break;
			case 0x4A:  // 'J'
				is_attack_key_down = false;
				break;
			case 0x4B:  // 'K'
				is_evade_key_down = false;
				break;
			}
			break;
		case PlayerID::P2:
			switch (msg.vkcode)
			{
			case VK_LEFT:  // '←'
				is_left_key_down = false;
				break;
			case VK_RIGHT:  // '→'
				is_right_key_down = false;
				break;
			case VK_UP:       // '↑'
				is_jump_key_down = false;
				break;
			case VK_NUMPAD1:  // '1'
				is_attack_key_down = false;
				break;
			case VK_NUMPAD2:  // '2'
				is_evade_key_down = false;
				break;
			}
			break;
		}
		break;
	}

	if (!is_left_key_down && !is_right_key_down)
	{
		is_stopping = true;

		can_run = false;
	}

	if (get_move_axis() != 0 && !is_evading && !is_attacking && !is_jump_attacking)
	{
		if (is_left_key_down && !is_right_key_down)
			is_facing_left = true;
		else if (!is_left_key_down && is_right_key_down)
			is_facing_left = false;
	}
	//printf("%d", is_facing_left);
}

void KongPlayer::on_update(float delta)
{	
	if (get_id() == PlayerID::P1)
		player_1_left = render_facing_left;
	else if (get_id() == PlayerID::P2)
		player_2_left = render_facing_left;

	if (hp > 0 && !is_evading && !is_attacking && !is_hurting)
	{
		if (is_running || can_run)
			velocity.x = get_move_axis() * SPEED_RUN;
		else
			velocity.x = get_move_axis() * SPEED_WALK;
	}

	if (is_hurting)
	{
		if (get_id() == PlayerID::P1)
		{
			if (player_2_left)
				velocity.x = -SPEED_HURT_X;
			else
				velocity.x = SPEED_HURT_X;
		}
		else if (get_id() == PlayerID::P2)
		{
			if (player_1_left)
				velocity.x = -SPEED_HURT_X;
			else
				velocity.x = SPEED_HURT_X;
		}
	}

	if (get_hp() <= 0)
	{
		velocity.x = 0;
		velocity.y = 0;
	}

	timer_evade_cd.on_update(delta);

	hurt_box->set_position(get_logic_center());

	if (!is_attacking)
		hurt_box->set_position(get_logic_center());

	if (is_attacking)
		velocity.x = 0;

	Character::on_update(delta);

	if (current_slash_animation)
	{
		Animation& slash_animation = (render_facing_left
			? current_slash_animation->left : current_slash_animation->right);
		slash_animation.on_update(delta);
		Vector2 pos_slash;
		if (is_attacking)
		{
			if (CheckIn())
			{
				if (get_id() == PlayerID::P1)
					pos_slash = { render_facing_left ? player_2->get_position().x - 30: player_2->get_position().x + 30, get_logic_center().y - 10};
				else if (get_id() == PlayerID::P2)
					pos_slash = { render_facing_left ? player_1->get_position().x - 30 : player_1->get_position().x + 30, get_logic_center().y - 10 };
			}
			else
				pos_slash = { get_logic_center().x + (render_facing_left ? -250 : 250), get_logic_center().y - 10 };
		}
		else if (is_jump_attacking)
			pos_slash = { get_logic_center().x + (render_facing_left ? -50 : 50), get_logic_center().y - 10 };
		slash_animation.set_position(pos_slash);
	}
}

void KongPlayer::on_render()
{
	Character::on_render();

	if (is_attacking || is_jump_attacking)
		(render_facing_left ? current_slash_animation->left : current_slash_animation->right).on_render();
}

void KongPlayer::on_hurt()
{
	velocity.y = 0;
	velocity.y -= SPEED_HURT_Y;
}

void KongPlayer::on_jump()
{
	velocity.y -= SPEED_JUMP;
}

void KongPlayer::on_jump_again()
{
	velocity.y = 0;
	velocity.y -= SPEED_JUMP;
}

void KongPlayer::on_evade()
{
	timer_evade_cd.restart();
	is_evade_cd_comp = false;
}