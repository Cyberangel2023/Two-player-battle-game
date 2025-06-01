#ifndef _RESOURCES_MANAGER_H_
#define _RESOURCES_MANAGER_H_

#include <string>
#include <graphics.h>
#include <unordered_map>

class ResourcesManager
{
public:
	static ResourcesManager* instance();

	void load();

	IMAGE* find_image(const std::string& id) const;

private:
	static ResourcesManager* manager;

	// Í¼Æ¬³Ø
	std::unordered_map<std::string, IMAGE*> image_pool;

private:
	ResourcesManager();
	~ResourcesManager();

	void flip_image(IMAGE* src_image, IMAGE* dst_image, int num_h = 1);
	void flip_image(const std::string& src_id, const std::string dst_id, int num_h = 1);

};

#endif // _RESOURCES_MANAGER_H_