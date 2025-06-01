#include "kong_player.h"
#include "character_manager.h"
#include "kong_player_state_node.h"

extern bool ending;

extern Character* player_1;
extern Character* player_2;

KongPlayerAttackState::KongPlayerAttackState()
{
	timer_attack.set_wait_time(0.15f);
	timer_attack.set_one_shot(true);
	timer_attack.set_on_timeout([&]()
		{
			KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();
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
			KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();
			attack_key_window = false;
			player->set_attacking(false);
		});
}

void KongPlayerAttackState::on_enter()
{
	//printf("attack");

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	std::string name = "kong_attack_" + std::to_string(player->get_num());
	CharacterManager::instance()->get_player()->set_animation(name);

	std::string vfx_name = "kong_vfx_attack_" + std::to_string(player->get_num());
	CharacterManager::instance()->get_player()->set_slash_animation(vfx_name);
	
	player->set_attacking(true);
	player->set_next_attack(true);
	player->set_can_change(false);
	update_hit_box_position();
	timer_attack.restart();
	timer_next.restart();
	timer.restart();

	switch (player->get_num())
	{
	case 1:	
		play_audio(_T("kong_attack_1"), false);
		break;
	case 2:
	case 3:
		play_audio(_T("kong_attack_2"), false);
		break;
	case 4:
		play_audio(_T("kong_attack_3"), false);
		break;
	}
}

void KongPlayerAttackState::on_update(float delta)
{
	timer.on_update(delta);
	timer_next.on_update(delta);
	timer_attack.on_update(delta);

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	if (player->can_attack_next() && attack_key_window)
		player->enter_attack_next(true);

	if (player->get_hp() <= 0)
		player->switch_state("kong_dead");
	else if (player->get_hurting())
	{
		player->reset_num();
		player->switch_state("kong_hit");
	}
	else if (!player->get_attacking())
	{
		if (!player->get_enter_attack_next())
			player->reset_num();
		if (player->get_enter_attack_next())
		{
			player->add_num();
			player->switch_state("kong_attack");
		}
		else if (player->get_velocity().y > 0)
			player->switch_state("kong_fall");
		else if (player->get_move_axis() == 0)
			player->switch_state("kong_idle");
		else if (player->is_on_floor() && player->get_move_axis() != 0)
			player->switch_state("kong_walk");
	}
}

void KongPlayerAttackState::on_exit()
{
	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();
	player->get_hit_box()->set_enabled(false);
	player->set_attacking(false);
	player->set_next_attack(false);
	player->enter_attack_next(false);
	player->set_can_change(true);
	player->get_hit_box()->set_size({ 70, 140 });
}

void KongPlayerAttackState::update_hit_box_position()
{
	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();
	Vector2 pos_center = player->get_logic_center();
	CollisionBox* hit_box = player->get_hit_box();
	const Vector2& size_hit_box = hit_box->get_size();
	Vector2 pos_hit_box;
	Character* player_else = player->get_id() == PlayerID::P1 ? player_2 : player_1;
	int num = player->get_num();
	if (player->CheckIn())
	{
		if (num == 1)
			pos_hit_box = { player->get_render_facing_left() ? player_else->get_position().x + 45 : player_else->get_position().x - 45, pos_center.y };
		else if (num == 2)
			pos_hit_box = { player->get_render_facing_left() ? player_else->get_position().x - 45 : player_else->get_position().x + 45, pos_center.y };
		else if (num == 3)
			pos_hit_box = { player->get_render_facing_left() ? player_else->get_position().x : player_else->get_position().x, pos_center.y };
		else if (num == 4)
		{
			pos_hit_box = { player->get_render_facing_left() ? player_else->get_position().x - 10 : player_else->get_position().x + 10, pos_center.y };
			hit_box->set_size({ 170, 140 });
		}
	}
	else
	{
		if (num == 1)
			pos_hit_box = { pos_center.x + (player->get_render_facing_left() ? -size_hit_box.x - 100 : size_hit_box.x + 100), pos_center.y };
		else if (num == 2)
			pos_hit_box = { pos_center.x + (player->get_render_facing_left() ? -size_hit_box.x - 200 : size_hit_box.x + 200), pos_center.y };
		else if (num == 3)
			pos_hit_box = { pos_center.x + (player->get_render_facing_left() ? -size_hit_box.x - 150 : size_hit_box.x + 150), pos_center.y };
		else if (num == 4)
		{
			pos_hit_box = { pos_center.x + (player->get_render_facing_left() ? -size_hit_box.x - 150 : size_hit_box.x + 150), pos_center.y };
			hit_box->set_size({ 170, 140 });
		}
	}
	hit_box->set_position(pos_hit_box);
}

KongPlayerDeadState::KongPlayerDeadState()
{
	timer.set_wait_time(1.553f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]()
		{
			ending = true;
		});
}

void KongPlayerDeadState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("kong_dead");

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();
	player->set_position(player->get_position());
	player->set_can_change(false);

	play_audio(_T("hit"), false);
}

void KongPlayerDeadState::on_update(float delta)
{
	//printf("dead");

	timer.on_update(delta);
}

void KongPlayerFallState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("kong_jump");
}

void KongPlayerFallState::on_update(float delta)
{
	//printf("fall");

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	if (player->get_hp() <= 0)
		player->switch_state("kong_dead");
	else if (player->get_hurting())
		player->switch_state("kong_hit");
	else if (player->is_on_floor())
	{
		if (player->can_running())
			player->switch_state("kong_run");
		player->switch_state("kong_idle");
	}
	else if (player->can_evade())
		player->switch_state("kong_evade");
	else if (player->can_jump_attack())
		player->switch_state("kong_jump_attack");
	else if (player->can_jump_again() || player->can_jump())
		player->switch_state("kong_jump");
}

void KongPlayerFallState::on_exit()
{
	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	if (player->is_on_floor())
	{
		player->set_jump_again(true);
		player->set_once_jump(true);
		player->set_twice_jump(true);

		play_audio(_T("land"), false);
	}
}

void KongPlayerIdleState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("kong_idle");
}

void KongPlayerIdleState::on_update(float delta)
{
	//printf("idle");

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();
	if (player->get_hp() <= 0)
		player->switch_state("kong_dead");
	else if (player->get_hurting())
			player->switch_state("kong_hit");
	if (player->can_walk())
		player->switch_state("kong_walk");
	else if (player->can_attack())
		player->switch_state("kong_attack");
	else if (player->get_velocity().y > 0)
		player->switch_state("kong_fall");
	else if (player->can_jump())
		player->switch_state("kong_jump");
	else if (player->can_evade())
		player->switch_state("kong_evade");
}

void KongPlayerJumpState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("kong_jump");

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

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

void KongPlayerJumpState::on_update(float delta)
{
	//printf("jump");

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	if (shotted && player->correct_key_jump_again())
	{
		player->set_jump_again(false);
	}

	if (player->get_hp() <= 0)
		player->switch_state("kong_dead");
	else if (player->get_hurting())
		player->switch_state("kong_hit");
	else if (player->get_velocity().y > 0)
		player->switch_state("kong_fall");
	else if (player->can_jump_attack())
		player->switch_state("kong_jump_attack");
	else if (player->can_jump_again())
		player->switch_state("kong_jump");
	else if (player->can_evade())
		player->switch_state("kong_evade");
}

KongPlayerJumpAttackState::KongPlayerJumpAttackState()
{
	timer_attack.set_wait_time(0.2f);
	timer_attack.set_one_shot(true);
	timer_attack.set_on_timeout([&]()
		{
			KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();
			player->get_hit_box()->set_enabled(true);
		});

	timer.set_wait_time(0.429f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]()
		{
			KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();
			player->set_jump_attacking(false);
		});
}

void KongPlayerJumpAttackState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("kong_jump_attack");

	CharacterManager::instance()->get_player()->set_slash_animation("kong_vfx_jump_attack");

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	player->set_jump_attacking(true);
	player->set_can_change(false);
	update_hit_box_position();
	//player->on_jump_attack();
	timer.restart();
	timer_attack.restart();

	play_audio(_T("kong_attack_1"), false);
}

void KongPlayerJumpAttackState::on_update(float delta)
{
	//printf("jump_attack");

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	timer.on_update(delta);
	timer_attack.on_update(delta);
	update_hit_box_position();

	if (player->get_hp() <= 0)
		player->switch_state("kong_dead");
	else if (player->get_hurting())
		player->switch_state("kong_hit");
	if (!player->get_jump_attacking())
	{
		if (player->get_velocity().y > 0)
			player->switch_state("kong_fall");
		else if (player->can_jump() || player->can_jump_again())
			player->switch_state("kong_jump");
		else if (player->is_on_floor())
			player->switch_state("kong_idle");
	}
}

void KongPlayerJumpAttackState::on_exit()
{
	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	player->get_hit_box()->set_enabled(false);
	player->set_jump_attacking(false);
	player->set_can_change(true);
	player->get_hit_box()->set_size({ 70, 140 });

	if (!player->is_on_floor() && !player->get_once_jump() && player->get_twice_jump())
		player->set_jump_again(false);
	if (player->is_on_floor())
		player->set_once_jump(true);
}

void KongPlayerJumpAttackState::update_hit_box_position()
{
	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();
	Vector2 pos_center = player->get_logic_center();
	CollisionBox* hit_box = player->get_hit_box();
	const Vector2& size_hit_box = hit_box->get_size();
	Vector2 pos_hit_box;
	player->get_hit_box()->set_size({ 140, 140 });
	pos_hit_box = { pos_center.x + (player->get_render_facing_left() ? -size_hit_box.x / 2 : size_hit_box.x / 2), pos_center.y };
	hit_box->set_position(pos_hit_box);
}

KongPlayerEvadeState::KongPlayerEvadeState()
{
	timer_move.set_wait_time(0.198f);
	timer_move.set_one_shot(true);
	timer_move.set_on_timeout([&]()
		{
			KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();
			Vector2 pos = { player->get_position().x + (player->get_render_facing_left() ? -player->get_move_evade() : player->get_move_evade()),
				player->get_position().y };
			player->set_position(pos);
		});

	timer.set_wait_time(0.3f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]()
		{
			KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();
			player->set_evading(false);
		});
}

void KongPlayerEvadeState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("kong_evade");

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();
	player->get_hurt_box()->set_enabled(false);
	player->set_evading(true);
	player->set_can_change(false);
	player->on_evade();
	timer.restart();
	timer_move.restart();

	play_audio(_T("kong_evade"), false);
}

void KongPlayerEvadeState::on_update(float delta)
{
	//printf("evade");

	timer.on_update(delta);
	timer_move.on_update(delta);

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	if (!player->get_evading())
	{
		if (player->get_velocity().x != 0 && player->is_on_floor())
		{
			if (player->can_running())
				player->switch_state("kong_run");
			else
				player->switch_state("kong_walk");
		}
		else if (player->get_hurting())
			player->switch_state("kong_hit");
		else if (player->can_jump() || player->can_jump_again())
			player->switch_state("kong_jump");
		else if (player->is_on_floor())
			player->switch_state("kong_idle");
		else if (player->get_velocity().y > 0)
			player->switch_state("kong_fall");
	}
}

void KongPlayerEvadeState::on_exit()
{
	CharacterManager::instance()->get_player()->get_hurt_box()->set_enabled(true);

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	if (!player->is_on_floor() && !player->get_once_jump() && player->get_twice_jump())
		player->set_jump_again(false);
	if (player->is_on_floor())
		player->set_once_jump(true);
	player->set_can_change(true);
}

void KongPlayerWalkState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("kong_walk");

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	player->set_walking(true);
}

void KongPlayerWalkState::on_update(float delta)
{
	//printf("walk");

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	if (player->get_hp() <= 0)
		player->switch_state("kong_dead");
	else if (player->get_hurting())
		player->switch_state("kong_hit");
	else if (player->can_running())
		player->switch_state("kong_run");
	else if (player->get_move_axis() == 0)
		player->switch_state("kong_idle");
	else if (player->can_jump())
		player->switch_state("kong_jump");
	else if (player->can_attack())
		player->switch_state("kong_attack");
	else if (player->can_evade())
		player->switch_state("kong_evade");
}

void KongPlayerWalkState::on_exit()
{
	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	player->set_walking(false);
	player->set_enter_run(true);
}

void KongPlayerRunState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("kong_run");

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	player->set_running(true);

	auto current_tick_run = std::chrono::steady_clock::now(); // 获取当前时间点
	last_tick_run = current_tick_run;
}

void KongPlayerRunState::on_update(float delta)
{
	//printf("run");

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	if (player->get_hp() <= 0)
		player->switch_state("kong_dead");
	else if (player->get_hurting())
		player->switch_state("kong_hit");
	else if (player->get_move_axis() == 0)
		player->switch_state("kong_idle");
	else if (player->can_jump())
		player->switch_state("kong_jump");
	else if (player->can_attack())
		player->switch_state("kong_attack");
	else if (player->can_evade())
		player->switch_state("kong_evade");
}

void KongPlayerRunState::on_exit()
{
	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

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

KongPlayerHitState::KongPlayerHitState()
{
	timer.set_wait_time(0.4f);
	timer.set_one_shot(true);
	timer.set_on_timeout([&]()
		{
			can_hit = false;
		});
}

void KongPlayerHitState::on_enter()
{
	CharacterManager::instance()->get_player()->set_animation("kong_hit");
	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	player->on_hurt();
	timer.restart();

	play_audio(_T("hit"), false);
}

void KongPlayerHitState::on_update(float delta)
{
	//printf("hit");

	timer.on_update(delta);

	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	if (player->get_hp() <= 0)
		player->switch_state("kong_dead");
	if (!can_hit)
	{
		if (player->can_walk())
			player->switch_state("kong_walk");
		else if (player->get_move_axis() == 0)
			player->switch_state("kong_idle");
		else if (player->can_attack())
			player->switch_state("kong_attack");
		else if (player->get_velocity().y > 0)
			player->switch_state("kong_fall");
		else if (player->can_jump() || player->can_jump_again())
			player->switch_state("kong_jump");
		else if (player->can_evade())
			player->switch_state("kong_evade");
	}
}

void KongPlayerHitState::on_exit()
{
	KongPlayer* player = (KongPlayer*)CharacterManager::instance()->get_player();

	if (player->is_on_floor())
	{
		player->set_once_jump(true);
		player->set_twice_jump(true);
	}

	player->set_hurting(false);
	can_hit = true;
}