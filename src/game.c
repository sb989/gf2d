#include <SDL.h>
#include "gf2d_gtk.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_draw.h"
#include "gfc_list.h"
#include "gfc_vector.h"
#include "gf2d_entity.h"
#include "gf2d_tilemap.h"
#include "gf2d_controls_state.h"
#include "gf2d_ui_box.h"
#include "gf2d_ui.h"
#include "gf2d_physics.h"
#include "gf2d_level_editor.h"
#include "gf2d_game_state.h"
#include "gf2d_enemy.h"
#include "gf2d_main_game.h"
int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    Sprite *mouse;
    int resx,resy;
    cpShapeFilter filter;
    resx = 1280;
    resy = 720;
    Entity * m;
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize("gf2d",resx,resy,resx,resy,vector4d(0,0,0,255),0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);

    gf2d_game_state_init();
    gf2d_gtk_init(argc,argv);
    gf2d_controls_manager_init();
    gf2d_physics_init();
    //gf2d_ui_init();
    gf2d_button_functions_init();
    gf2d_entity_init_manager();



    mouse = gf2d_sprite_load_image("images/cursor_pointer3D.png");
    filter = gf2d_mouseFilter();
    m = gf2d_entity_new("mouse",mouse,vector2d(0,0),MOUSE,filter,vector2d(1,1),vector2d(mouse->frame_w,mouse->frame_h));
    m->update = &gf2d_update_mouse_position;
    m->updateData = m;
    m->animateData = m;
    m->animate = &gf2d_entity_animate;
    m->maxFrame = 1;
    m = gf2d_entity_setup_collision_body(m,1,1,0,0,MOUSE,filter,vector2d(0,0));
    gf2d_ui_init();
    gf2d_main_menu_init();
    gf2d_level_editor_init();
    gf2d_tilemap_init_manager(32,18,resx,resy);
    gf2d_player_init();
    slog("finish player init");
    gf2d_enemy_init();
    slog("finish setup ");
    /*main game loop*/
    while(!done)
    {

        /*update things here*/


        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
        //backgrounds drawn first
        //gf2d_game_state_update();
        gf2d_controls_update();


        gf2d_physics_update();

        if(gf2d_game_state_get_state() == 2 && gf2d_game_state_get_update() == 1)
        {
          gf2d_level_editor_update();
        }
        else if(gf2d_game_state_get_state() == 0&& gf2d_game_state_get_update() == 1)
        {
          gf2d_main_game_update();
        }

        gf2d_entity_animate_all();
        gf2d_beam_animate_all();
        gf2d_ui_update();
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        gf2d_game_state_update();
        gf2d_entity_update_all();

        if (gf2d_key_pressed(SDL_SCANCODE_ESCAPE))done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    //free(temp);

    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
