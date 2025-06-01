#include "yan_player.h"
#include "resources_manager.h"
#include "yan_player_state_node.h"

#include <cmath>

extern bool player_1_left;
extern bool player_2_left;

YanPlayer::YanPlayer()
{
	add_hurt_box_x = 0;
	is_facing_left = false;
	logic_height = 123;

	hit_box->set_size({ 180, 180 });
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
			AnimationGroup& animation_yan_attack_1 = animation_pool["yan_attack_1"];

			Animation& animation_yan_attack_1_left = animation_yan_attack_1.left;
			animation_yan_attack_1_left.set_interval(0.033f);
			animation_yan_attack_1_left.set_loop(false);
			animation_yan_attack_1_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_attack_1_left.add_frame(ResourcesManager::instance()->find_image("yan_attack_1_left"), 13);

			Animation& animation_yan_attack_1_right = animation_yan_attack_1.right;
			animation_yan_attack_1_right.set_interval(0.033f);
			animation_yan_attack_1_right.set_loop(false);
			animation_yan_attack_1_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_attack_1_right.add_frame(ResourcesManager::instance()->find_image("yan_attack_1_right"), 13);
			
			AnimationGroup& animation_yan_attack_2 = animation_pool["yan_attack_2"];

			Animation& animation_yan_attack_2_left = animation_yan_attack_2.left;
			animation_yan_attack_2_left.set_interval(0.033f);
			animation_yan_attack_2_left.set_loop(false);
			animation_yan_attack_2_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_attack_2_left.add_frame(ResourcesManager::instance()->find_image("yan_attack_2_left"), 13);

			Animation& animation_yan_attack_2_right = animation_yan_attack_2.right;
			animation_yan_attack_2_right.set_interval(0.033f);
			animation_yan_attack_2_right.set_loop(false);
			animation_yan_attack_2_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_attack_2_right.add_frame(ResourcesManager::instance()->find_image("yan_attack_2_right"), 13);
			
			AnimationGroup& animation_yan_attack_3 = animation_pool["yan_attack_3"];

			Animation& animation_yan_attack_3_left = animation_yan_attack_3.left;
			animation_yan_attack_3_left.set_interval(0.033f);
			animation_yan_attack_3_left.set_loop(false);
			animation_yan_attack_3_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_attack_3_left.add_frame(ResourcesManager::instance()->find_image("yan_attack_3_left"), 13);

			Animation& animation_yan_attack_3_right = animation_yan_attack_3.right;
			animation_yan_attack_3_right.set_interval(0.033f);
			animation_yan_attack_3_right.set_loop(false);
			animation_yan_attack_3_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_attack_3_right.add_frame(ResourcesManager::instance()->find_image("yan_attack_3_right"), 13);
			
			AnimationGroup& animation_yan_attack_4 = animation_pool["yan_attack_4"];

			Animation& animation_yan_attack_4_left = animation_yan_attack_4.left;
			animation_yan_attack_4_left.set_interval(0.033f);
			animation_yan_attack_4_left.set_loop(false);
			animation_yan_attack_4_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_attack_4_left.add_frame(ResourcesManager::instance()->find_image("yan_attack_4_left"), 13);

			Animation& animation_yan_attack_4_right = animation_yan_attack_4.right;
			animation_yan_attack_4_right.set_interval(0.033f);
			animation_yan_attack_4_right.set_loop(false);
			animation_yan_attack_4_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_attack_4_right.add_frame(ResourcesManager::instance()->find_image("yan_attack_4_right"), 13);
			
			AnimationGroup& animation_yan_attack_5 = animation_pool["yan_attack_5"];

			Animation& animation_yan_attack_5_left = animation_yan_attack_5.left;
			animation_yan_attack_5_left.set_interval(0.033f);
			animation_yan_attack_5_left.set_loop(false);
			animation_yan_attack_5_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_attack_5_left.add_frame(ResourcesManager::instance()->find_image("yan_attack_5_left"), 13);

			Animation& animation_yan_attack_5_right = animation_yan_attack_5.right;
			animation_yan_attack_5_right.set_interval(0.033f);
			animation_yan_attack_5_right.set_loop(false);
			animation_yan_attack_5_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_attack_5_right.add_frame(ResourcesManager::instance()->find_image("yan_attack_5_right"), 13);
  
			AnimationGroup& animation_yan_jump_attack = animation_pool["yan_jump_attack"];

			Animation& animation_yan_jump_attack_left = animation_yan_jump_attack.left;
			animation_yan_jump_attack_left.set_interval(0.033f);
			animation_yan_jump_attack_left.set_loop(false);
			animation_yan_jump_attack_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_jump_attack_left.add_frame(ResourcesManager::instance()->find_image("yan_jump_attack_left"), 13);

			Animation& animation_yan_jump_attack_right = animation_yan_jump_attack.right;
			animation_yan_jump_attack_right.set_interval(0.033f);
			animation_yan_jump_attack_right.set_loop(false);
			animation_yan_jump_attack_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_jump_attack_right.add_frame(ResourcesManager::instance()->find_image("yan_jump_attack_right"), 13);
    	}
		{
			AnimationGroup& animation_yan_dead = animation_pool["yan_dead"];

			Animation& animation_yan_dead_left = animation_yan_dead.left;
			animation_yan_dead_left.set_interval(0.033f);
			animation_yan_dead_left.set_loop(false);
			animation_yan_dead_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_dead_left.add_frame(ResourcesManager::instance()->find_image("yan_dead_left"), 76);

			Animation& animation_yan_dead_right = animation_yan_dead.right;
			animation_yan_dead_right.set_interval(0.033f);
			animation_yan_dead_right.set_loop(false);
			animation_yan_dead_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_dead_right.add_frame(ResourcesManager::instance()->find_image("yan_dead_right"), 76);
		}
		{
			AnimationGroup& animation_yan_idle = animation_pool["yan_idle"];

			Animation& animation_yan_idle_left = animation_yan_idle.left;
			animation_yan_idle_left.set_interval(0.033f);
			animation_yan_idle_left.set_loop(true);
			animation_yan_idle_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_idle_left.add_frame(ResourcesManager::instance()->find_image("yan_idle_left"), 49);

			Animation& animation_yan_idle_right = animation_yan_idle.right;
			animation_yan_idle_right.set_interval(0.033f);
			animation_yan_idle_right.set_loop(true);
			animation_yan_idle_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_idle_right.add_frame(ResourcesManager::instance()->find_image("yan_idle_right"), 49);
		}
		{
			AnimationGroup& animation_yan_jump = animation_pool["yan_jump"];

			Animation& animation_yan_jump_left = animation_yan_jump.left;
			animation_yan_jump_left.set_interval(0.033f);
			animation_yan_jump_left.set_loop(false);
			animation_yan_jump_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_jump_left.add_frame(ResourcesManager::instance()->find_image("yan_jump_left"), 1);

			Animation& animation_yan_jump_right = animation_yan_jump.right;
			animation_yan_jump_right.set_interval(0.033f);
			animation_yan_jump_right.set_loop(false);
			animation_yan_jump_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_jump_right.add_frame(ResourcesManager::instance()->find_image("yan_jump_right"), 1);
		}
		{
			AnimationGroup& animation_yan_jump_attack = animation_pool["yan_jump_attack"];

			Animation& animation_yan_jump_attack_left = animation_yan_jump_attack.left;
			animation_yan_jump_attack_left.set_interval(0.033f);
			animation_yan_jump_attack_left.set_loop(false);
			animation_yan_jump_attack_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_jump_attack_left.add_frame(ResourcesManager::instance()->find_image("yan_jump_attack_left"), 13);

			Animation& animation_yan_jump_attack_right = animation_yan_jump_attack.right;
			animation_yan_jump_attack_right.set_interval(0.033f);
			animation_yan_jump_attack_right.set_loop(false);
			animation_yan_jump_attack_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_jump_attack_right.add_frame(ResourcesManager::instance()->find_image("yan_jump_attack_right"), 13);
		}
		{
			AnimationGroup& animation_yan_evade = animation_pool["yan_evade"];

			Animation& animation_yan_evade_left = animation_yan_evade.left;
			animation_yan_evade_left.set_interval(0.033f);
			animation_yan_evade_left.set_loop(false);
			animation_yan_evade_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_evade_left.add_frame(ResourcesManager::instance()->find_image("yan_evade_left"), 12);

			Animation& animation_yan_evade_right = animation_yan_evade.right;
			animation_yan_evade_right.set_interval(0.033f);
			animation_yan_evade_right.set_loop(false);
			animation_yan_evade_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_evade_right.add_frame(ResourcesManager::instance()->find_image("yan_evade_right"), 12);
		}
		{
			AnimationGroup& animation_yan_run = animation_pool["yan_run"];
			 
			Animation& animation_yan_run_left = animation_yan_run.left;
			animation_yan_run_left.set_interval(0.033f);
			animation_yan_run_left.set_loop(true);
			animation_yan_run_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_run_left.add_frame(ResourcesManager::instance()->find_image("yan_run_left"), 19);

			Animation& animation_yan_run_right = animation_yan_run.right;
			animation_yan_run_right.set_interval(0.033f);
			animation_yan_run_right.set_loop(true);
			animation_yan_run_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_run_right.add_frame(ResourcesManager::instance()->find_image("yan_run_right"), 19);
		}	
		{
			AnimationGroup& animation_yan_walk = animation_pool["yan_walk"];

			Animation& animation_yan_walk_left = animation_yan_walk.left;
			animation_yan_walk_left.set_interval(0.033f);
			animation_yan_walk_left.set_loop(true);
			animation_yan_walk_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_walk_left.add_frame(ResourcesManager::instance()->find_image("yan_walk_left"), 33);

			Animation& animation_yan_walk_right = animation_yan_walk.right;
			animation_yan_walk_right.set_interval(0.033f);
			animation_yan_walk_right.set_loop(true);
			animation_yan_walk_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_walk_right.add_frame(ResourcesManager::instance()->find_image("yan_walk_right"), 33);
		}	
		{
			AnimationGroup& animation_yan_hit = animation_pool["yan_hit"];

			Animation& animation_yan_hit_left = animation_yan_hit.left;
			animation_yan_hit_left.set_interval(0.033f);
			animation_yan_hit_left.set_loop(false);
			animation_yan_hit_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_hit_left.add_frame(ResourcesManager::instance()->find_image("yan_hit_left"), 15);

			Animation& animation_yan_hit_right = animation_yan_hit.right;
			animation_yan_hit_right.set_interval(0.033f);
			animation_yan_hit_right.set_loop(false);
			animation_yan_hit_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
			animation_yan_hit_right.add_frame(ResourcesManager::instance()->find_image("yan_hit_right"), 15);
		}
		{
			AnimationGroup& animation_yan_vfx_attack_1 = animation_pool["yan_vfx_attack_1"];

			Animation& animation_yan_vfx_attack_1_left = animation_yan_vfx_attack_1.left;
			animation_yan_vfx_attack_1_left.set_interval(0.033f);
			animation_yan_vfx_attack_1_left.set_loop(false);
			animation_yan_vfx_attack_1_left.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_yan_vfx_attack_1_left.add_frame(ResourcesManager::instance()->find_image("yan_vfx_attack_1_left"), 13);

			Animation& animation_yan_vfx_attack_1_right = animation_yan_vfx_attack_1.right;
			animation_yan_vfx_attack_1_right.set_interval(0.033f);
			animation_yan_vfx_attack_1_right.set_loop(false);
			animation_yan_vfx_attack_1_right.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_yan_vfx_attack_1_right.add_frame(ResourcesManager::instance()->find_image("yan_vfx_attack_1_right"), 13);
			
			AnimationGroup& animation_yan_vfx_attack_2 = animation_pool["yan_vfx_attack_2"];

			Animation& animation_yan_vfx_attack_2_left = animation_yan_vfx_attack_2.left;
			animation_yan_vfx_attack_2_left.set_interval(0.033f);
			animation_yan_vfx_attack_2_left.set_loop(false);
			animation_yan_vfx_attack_2_left.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_yan_vfx_attack_2_left.add_frame(ResourcesManager::instance()->find_image("yan_vfx_attack_2_left"), 13);

			Animation& animation_yan_vfx_attack_2_right = animation_yan_vfx_attack_2.right;
			animation_yan_vfx_attack_2_right.set_interval(0.033f);
			animation_yan_vfx_attack_2_right.set_loop(false);
			animation_yan_vfx_attack_2_right.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_yan_vfx_attack_2_right.add_frame(ResourcesManager::instance()->find_image("yan_vfx_attack_2_right"), 13);
			
			AnimationGroup& animation_yan_vfx_attack_3 = animation_pool["yan_vfx_attack_3"];

			Animation& animation_yan_vfx_attack_3_left = animation_yan_vfx_attack_3.left;
			animation_yan_vfx_attack_3_left.set_interval(0.033f);
			animation_yan_vfx_attack_3_left.set_loop(false);
			animation_yan_vfx_attack_3_left.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_yan_vfx_attack_3_left.add_frame(ResourcesManager::instance()->find_image("yan_vfx_attack_3_left"), 13);

			Animation& animation_yan_vfx_attack_3_right = animation_yan_vfx_attack_3.right;
			animation_yan_vfx_attack_3_right.set_interval(0.033f);
			animation_yan_vfx_attack_3_right.set_loop(false);
			animation_yan_vfx_attack_3_right.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_yan_vfx_attack_3_right.add_frame(ResourcesManager::instance()->find_image("yan_vfx_attack_3_right"), 13);
			
			AnimationGroup& animation_yan_vfx_attack_4 = animation_pool["yan_vfx_attack_4"];

			Animation& animation_yan_vfx_attack_4_left = animation_yan_vfx_attack_4.left;
			animation_yan_vfx_attack_4_left.set_interval(0.033f);
			animation_yan_vfx_attack_4_left.set_loop(false);
			animation_yan_vfx_attack_4_left.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_yan_vfx_attack_4_left.add_frame(ResourcesManager::instance()->find_image("yan_vfx_attack_4_left"), 13);

			Animation& animation_yan_vfx_attack_4_right = animation_yan_vfx_attack_4.right;
			animation_yan_vfx_attack_4_right.set_interval(0.033f);
			animation_yan_vfx_attack_4_right.set_loop(false);
			animation_yan_vfx_attack_4_right.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_yan_vfx_attack_4_right.add_frame(ResourcesManager::instance()->find_image("yan_vfx_attack_4_right"), 13);
			
			AnimationGroup& animation_yan_vfx_attack_5 = animation_pool["yan_vfx_attack_5"];

			Animation& animation_yan_vfx_attack_5_left = animation_yan_vfx_attack_5.left;
			animation_yan_vfx_attack_5_left.set_interval(0.033f);
			animation_yan_vfx_attack_5_left.set_loop(false);
			animation_yan_vfx_attack_5_left.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_yan_vfx_attack_5_left.add_frame(ResourcesManager::instance()->find_image("yan_vfx_attack_5_left"), 13);

			Animation& animation_yan_vfx_attack_5_right = animation_yan_vfx_attack_5.right;
			animation_yan_vfx_attack_5_right.set_interval(0.033f);
			animation_yan_vfx_attack_5_right.set_loop(false);
			animation_yan_vfx_attack_5_right.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_yan_vfx_attack_5_right.add_frame(ResourcesManager::instance()->find_image("yan_vfx_attack_5_right"), 13);
			
			AnimationGroup& animation_yan_vfx_jump_attack = animation_pool["yan_vfx_jump_attack"];

			Animation& animation_yan_vfx_jump_attack_left = animation_yan_vfx_jump_attack.left;
			animation_yan_vfx_jump_attack_left.set_interval(0.033f);
			animation_yan_vfx_jump_attack_left.set_loop(false);
			animation_yan_vfx_jump_attack_left.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_yan_vfx_jump_attack_left.add_frame(ResourcesManager::instance()->find_image("yan_vfx_jump_attack_left"), 13);

			Animation& animation_yan_vfx_jump_attack_right = animation_yan_vfx_jump_attack.right;
			animation_yan_vfx_jump_attack_right.set_interval(0.033f);
			animation_yan_vfx_jump_attack_right.set_loop(false);
			animation_yan_vfx_jump_attack_right.set_anchor_mode(Animation::AnchorMode::Centered);
			animation_yan_vfx_jump_attack_right.add_frame(ResourcesManager::instance()->find_image("yan_vfx_jump_attack_right"), 13);
		}
	}
	{
		// TODO: 状态机初始化
		state_machine.register_state("yan_attack", new YanPlayerAttackState());
		state_machine.register_state("yan_dead", new YanPlayerDeadState());
		state_machine.register_state("yan_walk", new YanPlayerWalkState());
		state_machine.register_state("yan_run", new YanPlayerRunState());
		state_machine.register_state("yan_idle", new YanPlayerIdleState());
		state_machine.register_state("yan_jump", new YanPlayerJumpState());
		state_machine.register_state("yan_jump_attack", new YanPlayerJumpAttackState());
		state_machine.register_state("yan_fall", new YanPlayerFallState());
		state_machine.register_state("yan_evade", new YanPlayerEvadeState());
		state_machine.register_state("yan_hit", new YanPlayerHitState());

		state_machine.set_entry("yan_idle");
	}
}

YanPlayer::~YanPlayer() = default;

void YanPlayer::on_input(const ExMessage& msg)
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

void YanPlayer::on_update(float delta)
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

	if (!is_attacking)
	{
		hurt_box->set_position(get_logic_center());
		add_hurt_box_x = 0;
	}
	else
	{
		if (num == 5)
			add_hurt_box_x += 0.7;
		else if (num == 4)
			add_hurt_box_x += 0.3;
		else
			add_hurt_box_x += 0.5;

		render_facing_left ? hurt_box->set_position(get_logic_center().add_x(-add_hurt_box_x)) :
			hurt_box->set_position(get_logic_center().add_x(add_hurt_box_x));	

		hit_box->add_position_x(render_facing_left ? -add_hurt_box_x * delta : add_hurt_box_x * delta);
	}

	if (is_jump_attacking)
	{
		hit_box->add_position_x(render_facing_left ? -add_hurt_box_x * delta / 2 : add_hurt_box_x * delta / 2);
	}

	if (is_attacking)
		velocity.x = 0;

	Character::on_update(delta);

	if (current_slash_animation)
	{
		Animation& slash_animation = (render_facing_left
			? current_slash_animation->left : current_slash_animation->right);
		slash_animation.on_update(delta);
		Vector2 pos_slash = { get_logic_center().x + (render_facing_left ? -5 : 5), get_logic_center().y + 8 };
		slash_animation.set_position(pos_slash);
	}
}

void YanPlayer::on_render()
{
	Character::on_render();

	if (is_attacking || is_jump_attacking)
		(render_facing_left ? current_slash_animation->left : current_slash_animation->right).on_render();
}

void YanPlayer::on_hurt() 
{
	velocity.y = 0;
	velocity.y -= SPEED_HURT_Y;
}

void YanPlayer::on_jump()
{
	velocity.y -= SPEED_JUMP;
}

void YanPlayer::on_jump_again()
{
	velocity.y = 0;
	velocity.y -= SPEED_JUMP;
}

void YanPlayer::on_evade()
{
	timer_evade_cd.restart();
	is_evade_cd_comp = false;
	velocity.x = render_facing_left ? -SPEED_EVADE : SPEED_EVADE;
}