#include "character.h"
#include "collision_manager.h"

extern std::string node_2;
extern std::string status_1;

Character::Character()
{
	hit_box = CollisionManager::instance()->create_collision_box();
	hurt_box = CollisionManager::instance()->create_collision_box();

	timer_invulnerable_status.set_wait_time(3.0f);
	timer_invulnerable_status.set_one_shot(true);
	timer_invulnerable_status.set_on_timeout([&]()
		{
			is_invulnerable = false;
			hurt_box->set_enabled(true);
		});

	timer_invulnerable_blink.set_wait_time(0.075f);
	timer_invulnerable_blink.set_one_shot(false);
	timer_invulnerable_blink.set_on_timeout([&]()
		{
			is_blink_invisible = !is_blink_invisible;
		});

	timer_hurt_num.set_wait_time(10.0f);
	timer_hurt_num.set_one_shot(true);
	timer_hurt_num.set_on_timeout([&]()
		{
			hurt_num = 0;
		});
}

Character::~Character()
{
	CollisionManager::instance()->destroy_collision_box(hit_box);
	CollisionManager::instance()->destroy_collision_box(hurt_box);
}

void Character::decrease_hp()
{
	if (is_invulnerable) return;

	hp -= range_random(3, 5);
	hurt_num++; // 受击次数加一
	if (hp > 0 && hurt_num >= 10)
	{
		make_invulnerable();
		hurt_num = 0;
	}
	on_hurt();
}

void Character::on_input(std::string ch, std::string ch_other) { }

void Character::on_update(float delta)
{
	if (hurt_num > 0)
		timer_hurt_num.on_update(delta);
	else
		timer_hurt_num.restart();

	if (can_change)
		render_facing_left = is_facing_left;

	state_machine.on_update(delta);

	if (hp <= 0)
		velocity.x = 0;
	if (enable_gravity)
		velocity.y += GRAVITY * delta;

	position += velocity * delta;

	if (position.y >= FLOOR_Y)
	{
		position.y = FLOOR_Y;
		velocity.y = 0;
	}

	if (position.x <= 0) position.x = 0;
	if (position.x >= getwidth()) position.x = (float)getwidth();

	timer_invulnerable_status.on_update(delta);

	if (is_invulnerable)
		timer_invulnerable_blink.on_update(delta);

	if (!current_animation) return;

	Animation& animation = (render_facing_left
		? current_animation->left : current_animation->right);
	animation.on_update(delta);
	animation.set_position(position);
}

void Character::on_render()
{
	if (!current_animation || (is_invulnerable && is_blink_invisible)) return;

	(render_facing_left ? current_animation->left : current_animation->right).on_render();
}

void Character::on_hurt() { }

void Character::switch_state(const std::string& id)
{
	state_machine.switch_to(id);
}

void Character::set_animation(const std::string& id)
{
	current_animation = &animation_pool[id];

	current_animation->left.reset();
	current_animation->right.reset();
}

void Character::set_slash_animation(const std::string& id)
{
	current_slash_animation = &animation_pool[id];

	current_slash_animation->left.reset();
	current_slash_animation->right.reset();
}