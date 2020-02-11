#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_draw.h"
#include "gfc_list.h"
#include "gfc_vector.h"
#include "gf2d_entity.h"
#include "gf2d_tilemap.h"
#include "gf2d_controls_state.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;

    Sprite * tile;
    int resx,resy;
    resx = 1080;
    resy = 720;

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize("gf2d",resx,resy,resx,resy,vector4d(0,0,0,255),0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    gf2d_entity_init_manager();
    gf2d_controls_manager_init();


    gf2d_tilemap_init_manager(resx,resy);
    tile = gf2d_sprite_load_all("images/tilemap_packed.png",16,16,27);
    gf2d_tilemap_new(tile,28,vector2d(00,00));


    /*main game loop*/
    while(!done)
    {
        gf2d_controls_update();
        /*update things here*/


        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
        //backgrounds drawn first
        gf2d_tilemap_draw_all();
        gf2d_entity_update_all();


        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

        if (gf2d_key_pressed(SDL_SCANCODE_ESCAPE))done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    //free(temp);

    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
