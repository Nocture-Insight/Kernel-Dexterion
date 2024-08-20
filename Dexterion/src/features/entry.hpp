#pragma once

#include "../util/Memory.hpp"
#include "../offsets/offsets.hpp"
#include "../config/config.hpp"

#include "esp/esp.cpp"

void mainLoop(bool state, MemoryManagement::moduleData client);