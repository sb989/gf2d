#include "gf2d_game_state.h"
#include "simple_logger.h"
enum state {game,main_menu,level_editor,load_level,pause_menu};
static enum state gameState;
static enum state oldState;
enum updated {yes,no};
static enum updated update;


void gf2d_game_state_init()
{
  gameState = main_menu;
  oldState = main_menu;
  update = no;
}

void gf2d_game_state_update()
{
  if(update == yes)
  {
    slog("updating state");
    switch(oldState)
    {
      case game:
        if(gameState != pause_menu)
        {
          gf2d_main_game_close();
        }
        break;
      case main_menu:
        gf2d_main_menu_close();
        break;
      case level_editor:
        slog("opening level_editor");
        gf2d_level_editor_close();
        break;
      case load_level:
        break;
      case pause_menu:
        if(gameState == game)
        {
          slog("closing pause menu");
          gf2d_pause_menu_close();
        }
        else if(gameState == main_menu)
        {
          gf2d_pause_menu_close();
          gf2d_main_game_close();
        }
        break;
      default:
        break;
    }

    switch(gameState)
    {
      case game:
        if(oldState != pause_menu)
        {
          gf2d_main_game_init();
          slog("oldstate is not pause_menu");
        }
        else if(oldState == pause_menu)
          gf2d_main_game_init_ui();
        break;
      case main_menu:
        gf2d_main_menu_init();
        break;
      case level_editor:
        gf2d_level_editor_load();
        break;
      case load_level:
        break;
      case pause_menu:
        gf2d_pause_menu_init();
        break;
      default:
        break;
    }

    update = no;
  }
}

int gf2d_game_state_get_state()
{
  return gameState;
}

int gf2d_game_state_get_old_state()
{
  return oldState;
}

int gf2d_game_state_get_update()
{
  return update;
}

void gf2d_game_state_set_main_menu(void * nothing)
{
  oldState = gameState;
  gameState = main_menu;
  update = yes;
}

void gf2d_game_state_set_level_editor(void * nothing)
{
  oldState = gameState;
  gameState = level_editor;
  update = yes;
}

void gf2d_game_state_set_game(void * nothing)
{
  oldState = gameState;
  gameState = game;
  update = yes;
}

void gf2d_game_state_set_pause_menu(void * nothing)
{
  oldState = gameState;
  gameState = pause_menu;
  update = yes;
}
