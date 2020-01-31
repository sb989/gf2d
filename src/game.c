#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_draw.h"
#include "gfc_list.h"
#include "gfc_vector.h"
int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    Sprite *block;
    int mx,my;
    int count;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);

    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    block = gf2d_sprite_load_image("images/block.png");

    SDL_Rect rec;
    rec.w = 50;
    rec.h = 50;

    SDL_Event e;
    List * coord;
    Vector2D *t;
    PointL * pl;
    Bool lMouseDown = false;
    Bool rMouseDown = false;
    coord = gfc_list_new();

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_PollEvent(&e);
        SDL_GetMouseState(&mx,&my);

        mf+=0.1;
        if (mf >= 16.0)mf = 0;

        if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
        {
          lMouseDown = true;
        }
        if(e.type==SDL_MOUSEBUTTONUP)
        {
          lMouseDown = false;
          rMouseDown = false;
        }
        if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button ==SDL_BUTTON_RIGHT)
        {
          rMouseDown = true;
        }
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
        //backgrounds drawn first
        gf2d_sprite_draw_image(sprite,vector2d(0,0));

      /*  gf2d_sprite_draw(
          block,
          vector2d(0,0),
          NULL,
          NULL,
          NULL,
          NULL,
          NULL,
          0);*/
        //UI elements last
        gf2d_sprite_draw(
            mouse,
            vector2d(mx,my),
            NULL,
            NULL,
            NULL,
            NULL,
            &mouseColor,
            (int)mf);
        if(lMouseDown)
        {

          if(gfc_list_get_count(coord) == 0)
          {
            t = vector2d_dup(vector2d(mx,my));
            coord = gfc_list_append(coord,t);
          }
          else
          {
            for(count = 0;count<gfc_list_get_count(coord);count++)
            {
              slog("count is %d",count);
              t = gfc_list_get_nth(coord,count);
              if(vector2d_equal((*t),vector2d(mx,my)))
              {
                slog("breaking");
                break;
              }
              if(count == gfc_list_get_count(coord)-1)
              {
                t = vector2d_dup(vector2d(mx,my));
                coord = gfc_list_append(coord,t);
                break;
              }
            }
          }



        }

        for(count = 0;count<gfc_list_get_count(coord);count++)
        {
          t = gfc_list_get_nth(coord,count);
          rec.x = t->x;
          rec.y = t->y;
          pl = gf2d_draw_rectangle(50,50,(int)t->x,(int)t->y);
          gf2d_draw_point_list(pl);
          //gf2d_draw_point_list_delete(pl);
          pl = gf2d_draw_fill_polygon(pl,50*50);
          gf2d_draw_point_list(pl);
          //slog("x coord is %d y coord is %d",t->x,t->y);
          //gf2d_draw_rectangle(50,50,t->x,t->y);

        }

        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    //free(temp);

    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
