#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

#include "collision_box.h"

#include <vector>

class CollisionManager
{
public:
	static CollisionManager* instance();

	CollisionBox* create_collision_box();                     // ��ײ�䴴��
	void destroy_collision_box(CollisionBox* collision_box);  // ��ײ������

	// ��ײ�����
	void process_collide();
	void on_debug_render();

private:
	static CollisionManager* manager;

	std::vector<CollisionBox*> collision_box_list;  // ��ײ���б�

private:
	CollisionManager();
	~CollisionManager();
};

#endif // !_COLLISION_MANAGER_H_