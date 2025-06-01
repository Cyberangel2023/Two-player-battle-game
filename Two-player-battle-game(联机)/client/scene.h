#ifndef _SCENE_H_
#define _SCENE_H_

#include "player_id.h"
#include "player_type.h"
#include "animation.h"
#include "scene_manager.h"
#include "character_manager.h"
#include "resources_manager.h"
#include "yan_player.h"
#include "kong_player.h"

#include <graphics.h>

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	virtual void on_enter() { }
	virtual void on_update(float delta) { }
	virtual void on_draw() { }
	virtual void on_input(std::string ch, std::string ch_other) { }
	virtual void on_exit() { }

protected:
	// 左右动画组
	struct IMAGEGroup
	{
		IMAGE* left;
		IMAGE* right;
	};

	IMAGE* current_avatar_1_img = nullptr;                          // 当前玩家1角色头像
	IMAGE* current_avatar_2_img = nullptr;                          // 当前玩家2角色头像
	IMAGE* current_background_1_img = nullptr;                      // 当前玩家1角色背景
	IMAGE* current_background_2_img = nullptr;                      // 当前玩家2角色背景
	std::unordered_map<std::string, IMAGEGroup> img_pool;           // 角色图片池

	IMAGEGroup& img_yan_avatar = img_pool["yan_avatar"];            // 薪炎头像
	IMAGEGroup& img_kong_avatar = img_pool["kong_avatar"];          // 空律头像
};

#endif // !_SCENE_H_