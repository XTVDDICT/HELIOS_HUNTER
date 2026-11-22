#pragma once
#include <stddef.h>
#include <stdint.h>
struct HunterZoneTransition { uint32_t utc; int32_t offset; };
struct HunterTimezone { const char* name; uint16_t first; uint16_t count; };
extern const HunterZoneTransition HUNTER_ZONE_TRANSITIONS[];
extern const HunterTimezone HUNTER_TIMEZONES[];
extern const size_t HUNTER_TIMEZONE_COUNT;
extern const char HUNTER_TIMEZONE_NAMES_JSON[];
int hunterTimezoneIndex(const char* name);
int32_t hunterTimezoneOffset(int zone, int64_t utc);
