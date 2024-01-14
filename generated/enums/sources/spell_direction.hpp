#pragma once

enum spell_direction_t {
	OWNER,
	NEAR_ENEMY,
	NEAR_FRIEND,
	OWNER_TARGET
};

const char* get_enum_name(spell_direction_t val);
