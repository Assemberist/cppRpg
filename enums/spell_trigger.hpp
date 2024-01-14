#pragma once

enum spell_trigger_t {
	EVERY_TURN,
	BY_OWNER,
	BEING_ATTACKED,
	ANY_EFFECT
};

const char* get_enum_name(spell_trigger_t val);
