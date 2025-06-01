#include "scene_manager.h"

SceneManager::SceneManager() = default;
SceneManager::~SceneManager() = default;

extern Scene* menu_scene;
extern Scene* game_scene;
extern Scene* selector_scene;

void SceneManager::set_current_scene(Scene* scene)
{
	current_scene = scene;
	current_scene->on_enter();
}

void SceneManager::switch_to(SceneType type)
{
	current_scene->on_exit();
	switch (type)
	{
	case SceneType::Menu:
		current_scene = menu_scene;
		break;
	case SceneType::Game:
		current_scene = game_scene;
		break;
	case SceneType::Selector:
		current_scene = selector_scene;
		break;
	default:
		break;
	}
	current_scene->on_enter();
}

void SceneManager::on_update(float delta)
{
	current_scene->on_update(delta);
}

void SceneManager::on_draw()
{
	current_scene->on_draw();
}

void SceneManager::on_input(const ExMessage& msg)
{
	current_scene->on_input(msg);
}