#ifndef _SELECTOR_SCENE_H_
#define _SELECTOR_SCENE_H_

#include "scene.h"

extern IMAGE* img_player_1_avatar;
extern IMAGE* img_player_2_avatar;

extern Character* player_1;
extern Character* player_2;

extern SceneManager scene_manager;

extern bool is_selector;

class SelectorScene : public Scene
{
public:
	SelectorScene() = default;
	~SelectorScene() = default;

	void on_enter()
	{
		is_selector = false;

		static const int OFFSET_X = 50;

		img_yan_avatar.left = ResourcesManager::instance()->find_image("yan_avatar_left");
		img_yan_avatar.right = ResourcesManager::instance()->find_image("yan_avatar_right");

		img_kong_avatar.left = ResourcesManager::instance()->find_image("kong_avatar_left");
		img_kong_avatar.right = ResourcesManager::instance()->find_image("kong_avatar_right");

		img_yan_background.left = ResourcesManager::instance()->find_image("yan_background_left");
		img_yan_background.right = ResourcesManager::instance()->find_image("yan_background_right");

		img_kong_background.left = ResourcesManager::instance()->find_image("kong_background_left");
		img_kong_background.right = ResourcesManager::instance()->find_image("kong_background_right");

		img_VS = ResourcesManager::instance()->find_image("VS");
		img_1P = ResourcesManager::instance()->find_image("1P");
		img_2P = ResourcesManager::instance()->find_image("2P");
		img_selector_tip = ResourcesManager::instance()->find_image("selector_tip");

		img_1P_desc = ResourcesManager::instance()->find_image("1P_desc");
		img_2P_desc = ResourcesManager::instance()->find_image("2P_desc");
		
		// VS
		pos_img_VS.x = (getwidth() - img_VS->getwidth()) / 2;
		pos_img_VS.y = (getheight() - img_VS->getheight()) / 2;

		// 1P 2P
		pos_img_1P.x = (getwidth() / 2 - img_1P->getwidth()) / 2 - OFFSET_X;
		pos_img_1P.y = 200;
		pos_img_2P.x = getwidth() / 2 + (getwidth() / 2 - img_2P->getwidth()) / 2 + OFFSET_X;
		pos_img_2P.y = pos_img_1P.y;

		// ��ʾ
		pos_img_tip.x = (getwidth() - img_selector_tip->getwidth()) / 2;
		pos_img_tip.y = getheight() - 125;

		// ��λ
		pos_img_1P_desc.x = (getwidth() / 2 - img_1P_desc->getwidth()) / 2 - OFFSET_X;
		pos_img_1P_desc.y = getheight() - 150;
		pos_img_2P_desc.x = getwidth() / 2 + (getwidth() / 2 - img_2P_desc->getwidth()) / 2 + OFFSET_X;
		pos_img_2P_desc.y = pos_img_1P_desc.y;

		// ͷ��
		pos_img_avatar_1P.x = pos_img_1P.x + (img_1P->getwidth() - img_yan_avatar.right->getwidth()) / 2;
		pos_img_avatar_1P.y = pos_img_1P.y + 90;
		pos_img_avatar_2P.x = pos_img_2P.x + (img_2P->getwidth() - img_kong_avatar.left->getwidth()) / 2;
		pos_img_avatar_2P.y = pos_img_avatar_1P.y;

		// ����
		pos_img_1P_name.y = pos_img_avatar_1P.y + 70;
		pos_img_2P_name.y = pos_img_1P_name.y;
	}

	void on_update(float delta) { }

	void on_draw()
	{
		IMAGE* img_p1_selector_background = nullptr;
		IMAGE* img_p2_selector_background = nullptr;

		IMAGE* img_p1_selector_avatar = nullptr;
		IMAGE* img_p2_selector_avatar = nullptr;

		// ���ñ���ͷ��
		switch (player_type_1)
		{
		case PlayerType::Yan:
			img_p1_selector_background = img_yan_background.right;
			img_p1_selector_avatar = img_yan_avatar.right;
			break;
		case PlayerType::Kong:
			img_p1_selector_background = img_kong_background.right;
			img_p1_selector_avatar = img_kong_avatar.right;
			break;
		default:
			img_p1_selector_background = img_yan_background.right;
			img_p1_selector_avatar = img_yan_avatar.right;
			break;
		}

		switch (player_type_2)
		{
		case PlayerType::Yan:
			img_p2_selector_background = img_yan_background.left;
			img_p2_selector_avatar = img_yan_avatar.left;
			break;
		case PlayerType::Kong:
			img_p2_selector_background = img_kong_background.left;
			img_p2_selector_avatar = img_kong_avatar.left;
			break;
		default:
			img_p2_selector_background = img_kong_background.left;
			img_p2_selector_avatar = img_kong_avatar.left;
			break;
		}

		// ���Ʊ���
		putimage_alpha(0, 0, img_p1_selector_background);
		putimage_alpha(getwidth() / 2, 0, img_p2_selector_background);

		// ����������
		putimage_alpha(pos_img_VS.x, pos_img_VS.y, img_VS);

		putimage_alpha(pos_img_1P.x, pos_img_1P.y, img_1P);
		putimage_alpha(pos_img_2P.x, pos_img_2P.y, img_2P);

		// ����ͷ��
		putimage_alpha(pos_img_avatar_1P.x, pos_img_avatar_1P.y, img_p1_selector_avatar);
		putimage_alpha(pos_img_avatar_2P.x, pos_img_avatar_2P.y, img_p2_selector_avatar);

		// ���Ƽ�λ��ʾ
		putimage_alpha(pos_img_1P_desc.x, pos_img_1P_desc.y, img_1P_desc);
		putimage_alpha(pos_img_2P_desc.x, pos_img_2P_desc.y, img_2P_desc);

		// ��������
		switch (player_type_1)
		{
		case PlayerType::Yan:
			pos_img_1P_name.x = pos_img_avatar_1P.x + (img_yan_avatar.right->getwidth() - textwidth(str_yan_name)) / 2;
			outtextxy(pos_img_1P_name.x, pos_img_1P_name.y, str_yan_name);
			break;
		case PlayerType::Kong:
			pos_img_1P_name.x = pos_img_avatar_1P.x + (img_kong_avatar.right->getwidth() - textwidth(str_kong_name)) / 2;
			outtextxy(pos_img_1P_name.x, pos_img_1P_name.y, str_kong_name);
			break;
		}

		switch (player_type_2)
		{
		case PlayerType::Yan:
			pos_img_2P_name.x = pos_img_avatar_2P.x + (img_yan_avatar.left->getwidth() - textwidth(str_yan_name)) / 2;
			outtextxy(pos_img_2P_name.x, pos_img_2P_name.y, str_yan_name);
			break;
		case PlayerType::Kong:
			pos_img_2P_name.x = pos_img_avatar_2P.x + (img_kong_avatar.left->getwidth() - textwidth(str_kong_name)) / 2;
			outtextxy(pos_img_2P_name.x, pos_img_2P_name.y, str_kong_name);
			break;
		}

		// �����ı���ʾ��Ϣ
		putimage_alpha(pos_img_tip.x, pos_img_tip.y, img_selector_tip);
	}

	void on_input(const ExMessage& msg)
	{
		// ѡ���ɫ
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (msg.vkcode)
			{
				// 'A'
			case 0x41:
				printf("down\n");
				is_btn_1P_left_down = true;
				break;
				// 'D'
			case 0x44:
				is_btn_1P_right_down = true;
				break;
				// '��'
			case VK_LEFT:
				is_btn_2P_left_down = true;
				break;
				// '��'
			case VK_RIGHT:
				is_btn_2P_right_down = true;
				break;
			}
			break;
		case WM_KEYUP:
			switch (msg.vkcode)
			{
				// 'A'
			case 0x41:
				printf("up\n");
				is_btn_1P_left_down = false;
				player_type_1 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_1 - 1) % (int)PlayerType::Invalid);
				break;
				// 'D'
			case 0x44:
				is_btn_1P_right_down = false;
				player_type_1 = (PlayerType)(((int)player_type_1 + 1) % (int)PlayerType::Invalid);
				break;
				// '��'
			case VK_LEFT:
				is_btn_2P_left_down = false;
				player_type_2 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_2 - 1) % (int)PlayerType::Invalid);
				break;
				// '��'
			case VK_RIGHT:
				is_btn_2P_right_down = false;
				player_type_2 = (PlayerType)(((int)player_type_2 + 1) % (int)PlayerType::Invalid);
				break;
			case VK_RETURN:
				scene_manager.switch_to(SceneManager::SceneType::Game);
				break;
			}
			break;
		default:
			break;
		}
	}

	void on_exit()
	{
		// ������ɫ
		switch (player_type_1)
		{
		case PlayerType::Yan:
			player_1 = new YanPlayer();
			img_player_1_avatar = img_yan_avatar.right;
			break;
		case PlayerType::Kong:
			player_1 = new KongPlayer();
			img_player_1_avatar = img_kong_avatar.right;
			break;
		}
		player_1->set_id(PlayerID::P1);
		player_1->set_type(player_type_1);
		player_1->set_position({ 300, 300 });
		player_1->set_facing_left(false);

		player_1->get_hit_box()->set_layer_src(CollisionLayer::None);
		player_1->get_hit_box()->set_layer_dst(CollisionLayer::P2);

		player_1->get_hurt_box()->set_layer_src(CollisionLayer::P1);
		player_1->get_hurt_box()->set_layer_dst(CollisionLayer::None);

		switch (player_type_2)
		{
		case PlayerType::Yan:
			player_2 = new YanPlayer();
			img_player_2_avatar = img_yan_avatar.left;
			break;
		case PlayerType::Kong:
			player_2 = new KongPlayer();
			img_player_2_avatar = img_kong_avatar.left;
			break;
		}
		player_2->set_id(PlayerID::P2);
		player_2->set_type(player_type_2);
		player_2->set_position({ 1120, 300 });
		player_2->set_facing_left(true);

		player_2->get_hit_box()->set_layer_src(CollisionLayer::None);
		player_2->get_hit_box()->set_layer_dst(CollisionLayer::P1);

		player_2->get_hurt_box()->set_layer_src(CollisionLayer::P2);
		player_2->get_hurt_box()->set_layer_dst(CollisionLayer::None);
	}

private:
	IMAGEGroup& img_yan_background = img_pool["yan_background"];         // н��ѡ�Ǳ���
	IMAGEGroup& img_kong_background = img_pool["kong_background"];       // ����ѡ�Ǳ���

	IMAGE* img_selector_tip;

	IMAGE* img_VS;                                                       // VS ������ͼƬ
	IMAGE* img_1P;													   	 // 1P ������ͼƬ
	IMAGE* img_2P;													     // 2P ������ͼƬ
	IMAGE* img_1P_desc;													 // 1P ��λͼƬ
	IMAGE* img_2P_desc;												     // 2P ��λͼƬ

	POINT pos_img_tip = { 0 };                                           // ��ʾ��Ϣ�ı�ͼƬλ��

	PlayerType player_type_1 = PlayerType::Yan;                          // 1P ��ɫ����
	PlayerType player_type_2 = PlayerType::Kong;                         // 2P ��ɫ����

	LPCTSTR str_yan_name = _T("н��֮����");                             // н�׽�ɫ��
	LPCTSTR str_kong_name = _T("��֮����");                              // ���ɽ�ɫ��

	bool is_btn_1P_left_down = false;                                    // 1P �����л���ť�Ƿ���
	bool is_btn_1P_right_down = false;                                   // 1P �����л���ť�Ƿ���	
	bool is_btn_2P_left_down = false;                                    // 2P �����л���ť�Ƿ���
	bool is_btn_2P_right_down = false;									 // 2P �����л���ť�Ƿ���

private:
	void outtextxy_shaded(int x, int y, LPCTSTR str)
	{
		settextcolor(RGB(45, 45, 45));
		outtextxy(x + 3, y + 3, str);
		settextcolor(RGB(255, 255, 255));
		outtextxy(x, y, str);
	}

	POINT pos_img_VS = { 0 };                           // VS ������ͼƬλ��
	POINT pos_img_1P = { 0 };                           // 1P ������ͼƬλ��
	POINT pos_img_2P = { 0 };                           // 2P ������ͼƬλ��
	POINT pos_img_1P_desc = { 0 };                      // 1P ��λ����ͼƬλ��
	POINT pos_img_2P_desc = { 0 };                      // 2P ��λ����ͼƬλ��
	POINT pos_img_1P_name = { 0 };                      // 1P ��ɫ����ͼƬλ��
	POINT pos_img_2P_name = { 0 };                      // 2P ��ɫ����ͼƬλ��	
	POINT pos_img_avatar_1P = { 0 };                    // 1P ��ɫͷ��λ��
	POINT pos_img_avatar_2P = { 0 };                    // 2P ��ɫͷ��λ��
};

#endif // !_SELECTOR_SCENE_H_