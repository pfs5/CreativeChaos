#pragma once

#define ensure(condition) assert(condition)
#define ensureMsg(condition, message) assert((void(message), condition))