//Platform Globals
#include "plungs_lib.h"
#include "input.h"
#include "game.h"
#include "platform.h"

#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

//Windows Platform
#ifdef _WIN32
#include "win32_platform.cpp"
#endif

#include "gl_renderer.cpp"

typedef decltype(update_game) update_game_type;
static update_game_type* update_game_ptr;

void reload_game_dll(BumpAllocator* transientStorage);

int main()
{
    BumpAllocator transientStorage = make_bump_allocator(MB(50));
    BumpAllocator persistentStorage = make_bump_allocator(MB(50));

    input = (Input*)bump_alloc(&persistentStorage, sizeof(Input));
    if(!input)
    {
        SM_ERROR("Failed To Allocate Input");
        return -1;
    }

    renderData = (RenderData*)bump_alloc(&persistentStorage, sizeof(RenderData));
    if(!renderData)
    {
        SM_ERROR("Failed To Allocate RenderData");
        return -1;
    }

    platform_create_window(1280,720,"Temp");
    input->screenSizeX = 1280;
    input->screenSizeY = 720;

    gl_init(&transientStorage);

    while (running)
    {
        //Update
        platform_update_window();
        update_game(renderData,input);
        gl_render();

        platform_swap_buffers();

        transientStorage.used = 0;
    }
    
    return 0;
}


void update_game(RenderData* renderDataIn , Input* inputIn)
{
    update_game_ptr(renderDataIn, inputIn); 
}

void reload_game_dll(BumpAllocator* transientStorage)
{
    static void* gameDLL;
    static long long lastEditTimestampGameDLL;

    long long currentTimeStampGameDLL = get_timestamp("game.dll");
    if(currentTimeStampGameDLL > lastEditTimestampGameDLL)
    {
        if(gameDLL)
        {
            bool freeResult = platform_free_dynamic_library(gameDLL);
            SM_ASSERT(freeResult, "Failed To Free game.dll");
            gameDLL = nullptr;
            SM_TRACE("Freed game.dll");
        }

        while(!copy_file("game.dll", "game_load.dll", transientStorage))
        {
            Sleep(10);
        }
        SM_TRACE("Copied game.dll into game_load.dll");

        gameDLL = platform_load_dynamic_library("game_load.dll");
        SM_ASSERT(gameDLL, "Failed to Load game.dll");

        update_game_ptr = (update_game_type*)platform_load_dynamic_function(gameDLL, "update_game");
        SM_ASSERT(update_game_ptr, "Failed to Load update_game function");
        lastEditTimestampGameDLL = currentTimeStampGameDLL;
    }
}