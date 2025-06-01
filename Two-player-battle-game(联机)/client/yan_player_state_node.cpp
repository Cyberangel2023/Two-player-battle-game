#include "yan_player.h"
#include "character_manager.h"
#include "yan_player_state_node.h"

extern bool ending;

YanPlayerAttackState::YanPlayerAttackState()
{
	timer_attack.set_wait_time(0.15f);
	timer_attack.set_one_shot(true);
	timer_attack.set_on_timeout([&]()
		{
			YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();
			player->get_hit_box()->set_enabled(true);
		});

	timer_next.set_wait_time(0.329f);
	timer_next.set_one_shot(true);
	timer_next.set_on_timeout([&]()
		{
			attack_key_window = true;
		});

	timer.set_wait_time(0.429f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]()
		{
			YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();
			attack_key_window = false;
			player->set_attacking(false);
			if (player->get_num() == 5)
				player->reset_add_hurt_box_x();
		});
}

void YanPlayerAttackState::on_enter()
{
	//printf("attack");

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	std::string name = "yan_attack_" + std::to_string(player->get_num());
	CharacterManager::instance()->get_player()->set_animation(name);

	std::string vfx_name = "yan_vfx_attack_" + std::to_string(player->get_num());
	CharacterManager::instance()->get_player()->set_slash_animation(vfx_name);

	player->set_attacking(true);
	player->set_next_attack(true);
	player->set_can_change(false);
	if (player->get_num() == 1)
		update_hit_box_position();
	timer_attack.restart();
	timer_next.restart();
	timer.restart();

	switch (player->get_num())
	{
	case 1:
	case 2:
		play_audio(_T("yan_attack_1"), false);
		break;
	case 3:
		play_audio(_T("yan_attack_2"), false);
		break;	
	case 4:
		play_audio(_T("yan_attack_3"), false);
		break;
	case 5:
		play_audio(_T("yan_attack_4"), false);
		break;
	}
}

void YanPlayerAttackState::on_update(float delta)
{
	timer.on_update(delta);
	timer_next.on_update(delta);
	timer_attack.on_update(delta);

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	if (player->can_attack_next() && attack_key_window)
		player->enter_attack_next(true);

	if (player->get_hp() <= 0)
		player->switch_state("yan_dead");
	else if (player->get_hurting())
	{
		player->reset_num();
		player->switch_state("yan_hit");
	}
	else if (!player->get_attacking())
	{
		if (!player->get_enter_attack_next())
		{
			if (player->get_num() >= 3 && player->get_num() < 5)
				player->set_attack_position((float)(player->get_num() - 1) * player->get_render_facing_left() ? -45 : 45);
			else if (player->get_num() >= 2 && player->get_num() < 3)
				player->set_attack_position((float)(player->get_num() - 1) * player->get_render_facing_left() ? -8 : 8);
			else if (player->get_num() >= 5)
				player->set_attack_position((float)(player->get_num() - 1) * player->get_render_facing_left() ? -65 : 65);
			player->reset_num();
		}
		else if (player->get_num() >= 5)
			player->set_attack_position((float)(player->get_num() - 1) * player->get_render_facing_left() ? -65 : 65);
		if (player->get_enter_attack_next())
		{
			player->add_num();
			player->switch_state("yan_attack");
		}
		else if (player->get_velocity().y > 0)
			player->switch_state("yan_fall");
		else if (player->get_move_axis() == 0)
			player->switch_state("yan_idle");
		else if (player->is_on_floor() && player->get_move_axis() != 0)
			player->switch_state("yan_walk");
	}
}

void YanPlayerAttackState::on_exit()
{
	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();
	player->get_hit_box()->set_enabled(false);
	player->set_attacking(false);
	player->set_next_attack(false);
	player->enter_attack_next(false);
	player->set_can_change(true);
}

void YanPlayerAttackState::update_hit_box_position()
{
	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();
	Vector2 pos_center = player->get_logic_center();
	CollisionBox* hit_box = player->get_hit_box();
	const Vector2& size_hit_box = hit_box->get_size();
	Vector2 pos_hit_box;
	pos_hit_box = { pos_center.x + (player->get_render_facing_left() ? -size_hit_box.x / 4 : size_hit_box.x / 4), pos_center.y };
	hit_box->set_position(pos_hit_box);
}

YanPlayerDeadState::YanPlayerDeadState()
{
	timer.set_wait_time(2.708f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]()
		{
			ending = true;
		});
}

void YanPlayerDeadState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("yan_dead");

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();
	player->set_position(player->get_position());
	player->set_can_change(false);

	play_audio(_T("hit"), false);
}

void YanPlayerDeadState::on_update(float delta)
{
	//printf("dead");

	timer.on_update(delta);
}

void YanPlayerFallState::on_enter() 
{
	CharacterManager::instance()->get_player()->set_animation("yan_jump");
}

void YanPlayerFallState::on_update(float delta)
{
	//printf("fall");

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	if (player->get_hp() <= 0)
		player->switch_state("yan_dead");
	else if (player->get_hurting())
		player->switch_state("yan_hit");
	else if (player->is_on_floor())
	{
		if (player->can_running())
			player->switch_state("yan_run");
		player->switch_state("yan_idle");
	}
	else if (player->can_evade())
		player->switch_state("yan_evade");
	else if (player->can_jump_attack())
		player->switch_state("yan_jump_attack");
	else if (player->can_jump_again() || player->can_jump())
		player->switch_state("yan_jump");
}

void YanPlayerFallState::on_exit()
{
	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	if (player->is_on_floor())
	{
		player->set_jump_again(true);
		player->set_once_jump(true);
		player->set_twice_jump(true);	
		
		play_audio(_T("land"), false);
	}
}

void YanPlayerIdleState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("yan_idle");
}

void YanPlayerIdleState::on_update(float delta)
{
	//printf("idle");

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();
	
	if (player->get_hp() <= 0)
		player->switch_state("yan_dead");
	else if (player->get_hurting())
		player->switch_state("yan_hit");
	else if (player->can_walk())
		player->switch_state("yan_walk");
	else if (player->can_attack())
		player->switch_state("yan_attack");
	else if (player->get_velocity().y > 0)
		player->switch_state("yan_fall");
	else if (player->can_jump())
		player->switch_state("yan_jump");
	else if (player->can_evade())
		player->switch_state("yan_evade");
}

void YanPlayerJumpState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("yan_jump");

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	if (player->is_on_floor())
	{
		player->on_jump();
		player->set_once_jump(false);
		shotted = true;
	}
	else if (!player->is_on_floor())
	{
		player->on_jump_again();
		player->set_jump_again(true);
		player->set_twice_jump(false);
		shotted = false;
	}

	play_audio(_T("jump"), false);
}

void YanPlayerJumpState::on_update(float delta)
{
	//printf("jump");

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	if (shotted && player->correct_key_jump_again())
	{
		player->set_jump_again(false);
	}

	if (player->get_hp() <= 0)
		player->switch_state("yan_dead");
	else if (player->get_hurting())
		player->switch_state("yan_hit");
	else if (player->get_velocity().y > 0)
		player->switch_state("yan_fall");
	else if (player->can_jump_attack())
		player->switch_state("yan_jump_attack");
	else if (player->can_jump_again())
		player->switch_state("yan_jump");
	else if (player->can_evade())
		player->switch_state("yan_evade");
}

YanPlayerJumpAttackState::YanPlayerJumpAttackState()
{
	timer_attack.set_wait_time(0.15f);
	timer_attack.set_one_shot(true);
	timer_attack.set_on_timeout([&]()
		{
			YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();
			player->get_hit_box()->set_enabled(true);
		});

	timer.set_wait_time(0.429f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]()
		{
			YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();
			player->set_jump_attacking(false);
		});
}

void YanPlayerJumpAttackState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("yan_jump_attack");

	CharacterManager::instance()->get_player()->set_slash_animation("yan_vfx_jump_attack");

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	player->set_jump_attacking(true);
	player->set_can_change(false);
	update_hit_box_position();
	//player->on_jump_attack();
	timer.restart();
	timer_attack.restart();

	play_audio(_T("yan_attack_4"), false);
}

void YanPlayerJumpAttackState::on_update(float delta)
{
	//printf("jump_attack");

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	timer.on_update(delta);
	timer_attack.on_update(delta);
	update_hit_box_position();

	if (player->get_hp() <= 0)
		player->switch_state("yan_dead");
	else if (player->get_hurting())
		player->switch_state("yan_hit");
	if (!player->get_jump_attacking())
	{
		if (player->get_velocity().y > 0)
			player->switch_state("yan_fall");
		else if (player->can_jump() || player->can_jump_again())
			player->switch_state("yan_jump");
		else if (player->is_on_floor())
			player->switch_state("yan_idle");
	}
}

void YanPlayerJumpAttackState::on_exit()
{
	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	player->get_hit_box()->set_enabled(false);
	player->set_jump_attacking(false);
	player->set_can_change(true);

	if (!player->is_on_floor() && !player->get_once_jump() && player->get_twice_jump())
		player->set_jump_again(false);
	if (player->is_on_floor())
		player->set_once_jump(true);
}

void YanPlayerJumpAttackState::update_hit_box_position()
{
	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();
	Vector2 pos_center = player->get_logic_center();
	CollisionBox* hit_box = player->get_hit_box();
	const Vector2& size_hit_box = hit_box->get_size();
	Vector2 pos_hit_box;
	pos_hit_box = { pos_center.x + (player->get_render_facing_left() ? -size_hit_box.x / 18 : size_hit_box.x / 18), pos_center.y + 30};
	hit_box->set_position(pos_hit_box);
}

YanPlayerEvadeState::YanPlayerEvadeState()
{
	timer.set_wait_time(0.3f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]()
		{
			YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();
			player->set_evading(false);
		});
}

void YanPlayerEvadeState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("yan_evade");

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();
	player->get_hurt_box()->set_enabled(false);
	player->set_evading(true);
	player->set_can_change(false);
	player->on_evade();
	timer.restart();

	play_audio(_T("yan_evade"), false);
}

void YanPlayerEvadeState::on_update(float delta)
{
	//printf("evade");

	timer.on_update(delta);

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	if (!player->get_evading())
	{
		if (player->get_velocity().x != 0 && player->is_on_floor())
		{
			if (player->can_running())
				player->switch_state("yan_run");
			else
		    	player->switch_state("yan_walk");
		}
		else if (player->get_hurting())
			player->switch_state("yan_hit");
		else if (player->can_jump() || player->can_jump_again())
			player->switch_state("yan_jump");
		else if (player->is_on_floor())
			player->switch_state("yan_idle");
		else if (player->get_velocity().y > 0)
			player->switch_state("yan_fall");
	}
}

void YanPlayerEvadeState::on_exit()
{
	CharacterManager::instance()->get_player()->get_hurt_box()->set_enabled(true);

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	if (!player->is_on_floor() && !player->get_once_jump() && player->get_twice_jump())
		player->set_jump_again(false);
	if (player->is_on_floor())
		player->set_once_jump(true);
	player->set_can_change(true);
}

void YanPlayerWalkState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("yan_walk");

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	player->set_walking(true);

	play_audio(_T("yan_walk"), true);
}

void YanPlayerWalkState::on_update(float delta)
{
	//printf("walk");

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	if (player->get_hp() <= 0)
		player->switch_state("yan_dead");
	else if (player->get_hurting())
		player->switch_state("yan_hit");
	else if (player->can_running())
		player->switch_state("yan_run");
	else if (player->get_move_axis() == 0)
		player->switch_state("yan_idle");
	else if (player->can_jump())
		player->switch_state("yan_jump");
	else if (player->can_attack())
		player->switch_state("yan_attack");
	else if (player->can_evade())
		player->switch_state("yan_evade");
}

void YanPlayerWalkState::on_exit()
{
	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();
	
	stop_audio(_T("yan_walk"));
	
	player->set_walking(false);
	player->set_enter_run(true);
}

void YanPlayerRunState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("yan_run");

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	player->set_running(true);

	auto current_tick_run = std::chrono::steady_clock::now(); // 获取当前时间点
	last_tick_run = current_tick_run;

	play_audio(_T("yan_run"), true);
}

void YanPlayerRunState::on_update(float delta)
{
	//printf("run");

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	if (player->get_hp() <= 0)
		player->switch_state("yan_dead");
	else if (player->get_hurting())
		player->switch_state("yan_hit");
	else if (player->get_move_axis() == 0)
		player->switch_state("yan_idle");
	else if (player->can_jump())
		player->switch_state("yan_jump");
	else if (player->can_attack())
		player->switch_state("yan_attack");
	else if (player->can_evade())
		player->switch_state("yan_evade");
}

void YanPlayerRunState::on_exit() 
{
	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	stop_audio(_T("yan_run"));

	auto current_tick_run = std::chrono::steady_clock::now(); // 获取当前时间点
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_tick_run - last_tick_run); // 计算时间差
	float time_diff_seconds = static_cast<float>(duration.count()) / 1000.0f; // 将时间差转换为秒
	if (time_diff_seconds < 0.5)
	{
		player->set_accidental_touch(true);
		player->set_running(false);
		player->set_enter_run(false);
		return;
	}
	player->set_accidental_touch(false);
	player->set_running(false);
	player->set_enter_run(false);
}

YanPlayerHitState::YanPlayerHitState()
{
	timer.set_wait_time(0.4f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]()
		{
			can_hit = false;
		});
}

void YanPlayerHitState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("yan_hit");
	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	player->on_hurt();
	timer.restart();

	play_audio(_T("hit"), false);
}

void YanPlayerHitState::on_update(float delta)
{
	//printf("hit");

	timer.on_update(delta);

	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();
	
	if (player->get_hp() <= 0)
		player->switch_state("yan_dead");
	if (!can_hit)
	{
		if (player->can_walk())
			player->switch_state("yan_walk");
		else if (player->get_move_axis() == 0)
			player->switch_state("yan_idle");
		else if (player->can_attack())
			player->switch_state("yan_attack");
		else if (player->get_velocity().y > 0)
			player->switch_state("yan_fall");
		else if (player->can_jump() || player->can_jump_again())
			player->switch_state("yan_jump");
		else if (player->can_evade())
			player->switch_state("yan_evade");
	}
}

void YanPlayerHitState::on_exit()
{
	YanPlayer* player = (YanPlayer*)CharacterManager::instance()->get_player();

	if (player->is_on_floor())
	{
		player->set_once_jump(true);
		player->set_twice_jump(true);
	}

	player->set_hurting(false);
	can_hit = true;
}