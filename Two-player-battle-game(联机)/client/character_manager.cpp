#include "yan_player.h"
#include "kong_player.h"
#include "character_manager.h"

extern bool is_player_1;

extern Character* player_1;
extern Character* player_2;

extern std::string status_1;
extern std::string status_2;

CharacterManager* CharacterManager::manager = nullptr;

CharacterManager* CharacterManager::instance()
{
	if (!manager)
		manager = new CharacterManager();

	return manager;
}

Character* CharacterManager::get_player()
{
	if (is_player_1)
		return player_1;
	else
		return player_2;
}

void CharacterManager::on_input(std::string ch, std::string ch_other)
{
	player_2->on_input(ch, ch_other);
	is_player_1 = true;
	player_1->on_input(ch, ch_other);
	is_player_1 = false;
}

void CharacterManager::on_update(float delta)
{
	player_2->on_update(delta);
	is_player_1 = true;
	player_1->on_update(delta);
	is_player_1 = false;
}

void CharacterManager::on_render()
{
	player_2->on_render();
	is_player_1 = true;
	player_1->on_render();
	is_player_1 = false;
}