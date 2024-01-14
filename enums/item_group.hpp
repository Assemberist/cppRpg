#pragma once

enum item_group_t {
	ARMORY_HEAD,
	ARMORY_HAND,
	ARMORY_FOOT,
	CLOTH_BODY,
	CLOTH_FOOT,
	AMULET,
	HOLDABLE_ONE_HAND,
	HOLDABLE_TWO_HAND,
	USEABLE_ITEM
};

const char* get_enum_name(item_group_t val);
