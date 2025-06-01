#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "scene.h"

#include <graphics.h>

class SceneManager
{
public:
	enum class SceneType
	{
		Menu,
		Game,
		Selector
	};

public:
	SceneManager();
	~SceneManager();

	void set_current_scene(Scene* scene); // 设置当前场景
	void switch_to(SceneType type);       // 切换场景
	void on_update(float delta);
	void on_draw();
	void on_input(const ExMessage& msg);

private:
	Scene* current_scene = nullptr;
};

#endif // !_SCENE_MANAGER_H_