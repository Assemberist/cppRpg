#pragma once

enum slot_type_t {
	CHARGE,
	BATTERY,
	ENCHANCEMENT
};

const char* get_enum_name(slot_type_t val);
