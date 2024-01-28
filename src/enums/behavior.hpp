#pragma once

enum behavior_t {
	BHV_CHILL,
	BHV_WORRIED,
	BHV_ATTACK,
	BHV_PANIC,
	BHV_STUNNED,
	BHV_REST
};

const char* get_enum_name(behavior_t val);
