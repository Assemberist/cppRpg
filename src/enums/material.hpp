#pragma once

enum material_t {
	FLESH,
	WOOD,
	IRON,
	ICE,
	STONE_MAT,
	EFIR,
	BONES
};

const char* get_enum_name(material_t val);
