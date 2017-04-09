#pragma once
#include <inttypes.h>
#include "world.h"

void kill(const uint16_t id);
void printagent(const uint16_t id);

void makeagent(const uint16_t id);
void moveagent(const uint16_t id);
void give_agent_input(const uint16_t id);
void execute_agent_input(const uint16_t id);
void execute_agent_output(const uint16_t id);
