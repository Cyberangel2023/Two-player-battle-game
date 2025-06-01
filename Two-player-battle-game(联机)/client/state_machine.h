#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "state_node.h"

#include <string>
#include <graphics.h>
#include <unordered_map>

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void on_update(float delta);

	void set_entry(const std::string& id);                               // ³õÊ¼×´Ì¬
	void switch_to(const std::string& id);                               // ×´Ì¬ÇÐ»»
	void register_state(const std::string& id, StateNode* state_node);   // ×¢²áÐÂµÄ×´Ì¬

private:
	bool need_init = true;                                   // ÊÇ·ñÒÑ³õÊ¼»¯
	StateNode* current_state = nullptr;                      // µ±Ç°×´Ì¬
	std::unordered_map<std::string, StateNode*> state_pool;  // ×´Ì¬³Ø                                      // µ±Ç°×´Ì¬
};

#endif // !_STATE_MACHINE_H_