#pragma once

#include "plungs_lib.h"

enum SpriteID
{
    SPRITE_ATLAS,

    SPRITE_COUNT
};

struct Sprite
{
    IVec2 atlasOffset;
    IVec2 spriteSize;
};

Sprite get_Sprite(SpriteID spriteid)
{
    Sprite sprite = {};
    
    switch(spriteid)
    {
        case SPRITE_ATLAS:
        {
            sprite.atlasOffset = {0, 0};
            sprite.spriteSize = {16, 16};
        }
    }

    return sprite;
}   