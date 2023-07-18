#pragma once

#define ARRAY_AS_BYTE_VECTOR(v) (std::vector((uint8_t*)v, (uint8_t*)v + sizeof(v)))
