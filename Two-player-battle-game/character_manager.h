#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include "character.h"

extern bool is_player_1;

extern Character* player_1;
extern Character* player_2;

class CharacterManager
{
public:
	static CharacterManager* instance();

	
	Character* get_player();

	void on_input(const ExMessage& mag);
	void on_update(float delta);
	void on_render();

private:
	static CharacterManager* manager;
};

#endif // !_CHARACTER_MANAGER_H_#pragma once