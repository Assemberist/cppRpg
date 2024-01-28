#pragma once

enum action_t {
	ACT_STAND,
    ACT_ATTACK,
    ACT_ATTACK_NEARLEST_ENEMY,
    ACT_SEARCH_ENEMY,
    ACT_WALK,
    ACT_RUN_AVAY,
    ACT_REST
};

const char* get_enum_name(action_t val);
