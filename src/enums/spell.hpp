#pragma once

enum spell_t {
	FIREBALL,
	PUNCH,
	LIGHTING,
	THROW,
	NOTHING_SPELL
};

const char* get_enum_name(spell_t val);
