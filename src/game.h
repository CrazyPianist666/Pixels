#pragma once

#include "input.h"
#include "plungs_lib.h"
#include"render_interface.h"

constexpr int tset = 5;

struct GameState
{
    IVec2 playerPos;
};

extern "C"
{
    EXPORT_FN void update_game(RenderData* renderDataIn, Input* inputIn);
}