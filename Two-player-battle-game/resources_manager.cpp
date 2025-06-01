#include "util.h"
#include "resources_manager.h"

ResourcesManager* ResourcesManager::manager = nullptr;

ResourcesManager* ResourcesManager::instance()
{
	if (!manager)
		manager = new ResourcesManager();

	return manager;
}

ResourcesManager::ResourcesManager() = default;
ResourcesManager::~ResourcesManager() = default;

struct ImageResInfo
{
	std::string id;
	LPCTSTR path;
};

// 获取资源
static const std::vector<ImageResInfo> image_info_list =
{
	{ "background",                   _T(R"(resources\background.png)") },
	{ "menu",                         _T(R"(resources\menu.png)") },
	{ "VS",                           _T(R"(resources\VS.png)") },
	{ "selector_tip",                 _T(R"(resources\selector_tip.png)") },

	{ "1P_desc",                      _T(R"(resources\1P_desc.png)") },
	{ "2P_desc",                      _T(R"(resources\2P_desc.png)") },

	{ "1P",                           _T(R"(resources\1P.png)") },
	{ "2P",                           _T(R"(resources\2P.png)") },

	{"ui_start_hovered",              _T(R"(resources\ui_start_hovered.png)") },
	{"ui_start_idle",                 _T(R"(resources\ui_start_idle.png)") },
	{"ui_start_pushed",               _T(R"(resources\ui_start_pushed.png)") },
	{"ui_quit_hovered",               _T(R"(resources\ui_quit_hovered.png)") },
	{"ui_quit_idle",                  _T(R"(resources\ui_quit_idle.png)") },
	{"ui_quit_pushed",                _T(R"(resources\ui_quit_pushed.png)") },

	{ "yan_background_right",         _T(R"(resources\yan_background.png)") },
	{ "kong_background_left",         _T(R"(resources\kong_background.png)") },

	{ "yan_avatar_right",             _T(R"(resources\yan_avatar.png)") },
	{ "kong_avatar_left",             _T(R"(resources\kong_avatar.png)") },

	{ "yan_attack_1_right",           _T(R"(resources\yan\attack_1.png)") },
	{ "yan_attack_2_right",           _T(R"(resources\yan\attack_2.png)") },
	{ "yan_attack_3_right",           _T(R"(resources\yan\attack_3.png)") },
	{ "yan_attack_4_right",           _T(R"(resources\yan\attack_4.png)") },
	{ "yan_attack_5_right",           _T(R"(resources\yan\attack_5.png)") },
	{ "yan_dead_right",               _T(R"(resources\yan\dead.png)") },
	{ "yan_idle_right",               _T(R"(resources\yan\idle.png)") },
	{ "yan_walk_right",               _T(R"(resources\yan\walk.png)") },
	{ "yan_jump_right",               _T(R"(resources\yan\jump.png)") },
	{ "yan_jump_attack_right",        _T(R"(resources\yan\jump_attack.png)") },
	{ "yan_run_right",                _T(R"(resources\yan\run.png)") },
	{ "yan_evade_right",              _T(R"(resources\yan\evade.png)") },
	{ "yan_hit_right",                _T(R"(resources\yan\hit.png)") },

	{ "yan_vfx_attack_1_right",       _T(R"(resources\yan\vfx_attack_1.png)") },
	{ "yan_vfx_attack_2_right",       _T(R"(resources\yan\vfx_attack_2.png)") },
	{ "yan_vfx_attack_3_right",       _T(R"(resources\yan\vfx_attack_3.png)") },
	{ "yan_vfx_attack_4_right",       _T(R"(resources\yan\vfx_attack_4.png)") },
	{ "yan_vfx_attack_5_right",       _T(R"(resources\yan\vfx_attack_5.png)") },
	{ "yan_vfx_jump_attack_right",    _T(R"(resources\yan\vfx_jump_attack.png)") },

	{ "kong_attack_1_left",           _T(R"(resources\kong\attack_1.png)") },
	{ "kong_attack_2_left",           _T(R"(resources\kong\attack_2.png)") },
	{ "kong_attack_3_left",           _T(R"(resources\kong\attack_3.png)") },
	{ "kong_attack_4_left",           _T(R"(resources\kong\attack_4.png)") },
	{ "kong_dead_left",               _T(R"(resources\kong\dead.png)") },
	{ "kong_idle_left",               _T(R"(resources\kong\idle.png)") },
	{ "kong_walk_left",               _T(R"(resources\kong\walk.png)") },
	{ "kong_jump_left",               _T(R"(resources\kong\jump.png)") },
	{ "kong_jump_attack_left",        _T(R"(resources\kong\jump_attack.png)") },
	{ "kong_run_left",                _T(R"(resources\kong\run.png)") },
	{ "kong_evade_left",              _T(R"(resources\kong\evade.png)") },
	{ "kong_hit_left",                _T(R"(resources\kong\hit.png)") },

	{ "kong_vfx_attack_1_left",       _T(R"(resources\kong\vfx_attack_1.png)") },
	{ "kong_vfx_attack_2_left",       _T(R"(resources\kong\vfx_attack_2.png)") },
	{ "kong_vfx_attack_3_left",       _T(R"(resources\kong\vfx_attack_3.png)") },
	{ "kong_vfx_attack_4_left",       _T(R"(resources\kong\vfx_attack_4.png)") },
	{ "kong_vfx_jump_attack_left",    _T(R"(resources\kong\vfx_jump_attack.png)") },
};

// 检查图片
static inline bool check_image_vaild(IMAGE* image)
{
	return GetImageBuffer(image);
}

// 加载资源
void ResourcesManager::load()
{
	for (const auto& info : image_info_list)
	{
		IMAGE* image = new IMAGE();
		loadimage(image, info.path);
		if (!check_image_vaild(image))
			throw info.path;
		image_pool[info.id] = image;
	}

	flip_image("yan_background_right", "yan_background_left", 1);
	flip_image("kong_background_left", "kong_background_right", 1);

	flip_image("yan_avatar_right", "yan_avatar_left", 1);
	flip_image("kong_avatar_left", "kong_avatar_right", 1);

	flip_image("yan_attack_1_right", "yan_attack_1_left", 13);
	flip_image("yan_attack_2_right", "yan_attack_2_left", 13);
	flip_image("yan_attack_3_right", "yan_attack_3_left", 13);
	flip_image("yan_attack_4_right", "yan_attack_4_left", 13);
	flip_image("yan_attack_5_right", "yan_attack_5_left", 13);
	flip_image("yan_dead_right", "yan_dead_left", 76);
	flip_image("yan_idle_right", "yan_idle_left", 49);
	flip_image("yan_walk_right", "yan_walk_left", 33);
	flip_image("yan_jump_right", "yan_jump_left", 1);
	flip_image("yan_jump_attack_right", "yan_jump_attack_left", 13);
	flip_image("yan_run_right", "yan_run_left", 19);
	flip_image("yan_evade_right", "yan_evade_left", 12);
	flip_image("yan_hit_right", "yan_hit_left", 15);

	flip_image("yan_vfx_attack_1_right", "yan_vfx_attack_1_left", 13);
	flip_image("yan_vfx_attack_2_right", "yan_vfx_attack_2_left", 13);
	flip_image("yan_vfx_attack_3_right", "yan_vfx_attack_3_left", 13);
	flip_image("yan_vfx_attack_4_right", "yan_vfx_attack_4_left", 13);
	flip_image("yan_vfx_attack_5_right", "yan_vfx_attack_5_left", 13);
	flip_image("yan_vfx_jump_attack_right", "yan_vfx_jump_attack_left", 13);

	flip_image("kong_attack_1_left", "kong_attack_1_right", 13);
	flip_image("kong_attack_2_left", "kong_attack_2_right", 13);
	flip_image("kong_attack_3_left", "kong_attack_3_right", 13);
	flip_image("kong_attack_4_left", "kong_attack_4_right", 13);
	flip_image("kong_dead_left", "kong_dead_right", 41);
	flip_image("kong_idle_left", "kong_idle_right", 49);
	flip_image("kong_walk_left", "kong_walk_right", 33);
	flip_image("kong_jump_left", "kong_jump_right", 1);
	flip_image("kong_jump_attack_left", "kong_jump_attack_right", 13);
	flip_image("kong_run_left", "kong_run_right", 19);
	flip_image("kong_evade_left", "kong_evade_right", 12);
	flip_image("kong_hit_left", "kong_hit_right", 15);

	flip_image("kong_vfx_attack_1_left", "kong_vfx_attack_1_right", 13);
	flip_image("kong_vfx_attack_2_left", "kong_vfx_attack_2_right", 13);
	flip_image("kong_vfx_attack_3_left", "kong_vfx_attack_3_right", 13);
	flip_image("kong_vfx_attack_4_left", "kong_vfx_attack_4_right", 13);
	flip_image("kong_vfx_jump_attack_left", "kong_vfx_jump_attack_right", 13);

	load_audio(_T(R"(resources\audio\bgm.mp3)"),             _T("bgm"));
	load_audio(_T(R"(resources\audio\jump.mp3)"),            _T("jump"));
	load_audio(_T(R"(resources\audio\land.mp3)"),            _T("land"));
	load_audio(_T(R"(resources\audio\hit.mp3)"),             _T("hit"));

	load_audio(_T(R"(resources\audio\kong_attack_1.mp3)"),   _T("kong_attack_1"));
	load_audio(_T(R"(resources\audio\kong_attack_2.mp3)"),   _T("kong_attack_2"));
	load_audio(_T(R"(resources\audio\kong_attack_3.mp3)"),   _T("kong_attack_3"));
	load_audio(_T(R"(resources\audio\kong_evade.mp3)"),      _T("kong_evade"));

	load_audio(_T(R"(resources\audio\yan_attack_1.mp3)"),    _T("yan_attack_1"));
	load_audio(_T(R"(resources\audio\yan_attack_2.mp3)"),    _T("yan_attack_2"));
	load_audio(_T(R"(resources\audio\yan_attack_3.mp3)"),    _T("yan_attack_3"));
	load_audio(_T(R"(resources\audio\yan_attack_4.mp3)"),    _T("yan_attack_4"));
	load_audio(_T(R"(resources\audio\yan_evade.mp3)"),       _T("yan_evade"));
	load_audio(_T(R"(resources\audio\yan_run.mp3)"),         _T("yan_run"));
	load_audio(_T(R"(resources\audio\yan_walk.mp3)"),        _T("yan_walk"));
}

// 寻找图片
IMAGE* ResourcesManager::find_image(const std::string& id) const
{
	const auto& itor = image_pool.find(id);
	if (itor == image_pool.end())
		return nullptr;

	return itor->second;
}

// 翻转图片
void ResourcesManager::flip_image(IMAGE* src_image, IMAGE* dst_image, int num_h)
{
	int w = src_image->getwidth();
	int h = src_image->getheight();
	int w_frame = w / num_h;
	Resize(dst_image, w, h);
	DWORD* src_buffer = GetImageBuffer(src_image);
	DWORD* dst_buffer = GetImageBuffer(dst_image);
	for (int i = 0; i < num_h; i++)
	{
		int x_left = i * w_frame;
		int x_right = (i + 1) * w_frame;
		for (int y = 0; y < h; y++)
		{
			for (int x = x_left; x < x_right; x++)
			{
				int idx_src = y * w + x;
				int idx_dst = y * w + x_right - (x - x_left);
				dst_buffer[idx_dst] = src_buffer[idx_src];
			}
		}
	}
}

void ResourcesManager::flip_image(const std::string& src_id, const std::string dst_id, int num_h)
{
	IMAGE* src_image = image_pool[src_id];
	IMAGE* dst_image = new IMAGE();

	flip_image(src_image, dst_image, num_h);

	image_pool[dst_id] = dst_image;
}