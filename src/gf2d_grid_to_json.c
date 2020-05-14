#include "gf2d_grid_to_json.h"

void gf2d_grid_to_json_create_json_grids(List * grids)
{
  int i,k,j,count,cells_per_row,num_rows,num_hitboxes;
  int hitx,hity,hitwidth,hitheight;
  Grid * grid;
  ButtonInfo * cell;
  ForegroundObject *env;
  ObjectHitBox *hitbox;
  char * filename;
  SJString *str;
  SJson *val;
  SJson * jcell;
  SJson *jenv;
  SJson *jhitbox;
  SJson *object = sj_object_new();
  SJson *cellArray = sj_array_new();
  SJson *envArray = sj_array_new();
  SJson *hitboxArray = sj_array_new();
  count = gfc_list_get_count(grids);
  filename = gf2d_level_editor_get_save_file();
  if(count ==0)
    return;

  cells_per_row = ((Grid *)(gfc_list_get_nth(grids,0)))->columns;
  num_rows = ((Grid *)(gfc_list_get_nth(grids,0)))->rows;

  str = sj_string_new_integer(cells_per_row*count);
  val = sj_string_to_value(str);
  sj_object_insert(object,"tiles_per_row",val);

  str = sj_string_new_integer(num_rows);
  val = sj_string_to_value(str);
  sj_object_insert(object,"num_rows",val);

  for(k=0;k<num_rows;k++)
  {
    for(i=0;i<count;i++)
    {
      grid = gfc_list_get_nth(grids,i);
      for(j = 0;j<cells_per_row;j++)
      {
        cell = gfc_list_get_nth(grid->cells,j+(k*cells_per_row));
        env = gfc_list_get_nth(grid->objects,j+(k*cells_per_row));

        jcell = gf2d_grid_to_json_create_json_cell(cell->boxInfo,(i*cells_per_row)+j,k);
        jenv = gf2d_grid_to_json_create_json_cell(env->box,(i*cells_per_row)+j,k);
        sj_array_append(cellArray,jcell);
        sj_array_append(envArray,jenv);
      }
    }
  }
  sj_object_insert(object,"cells",cellArray);
  sj_object_insert(object,"env",envArray);

  for(i = 0;i<count;i++)
  {
    grid = gfc_list_get_nth(grids,i);
    num_hitboxes = gfc_list_get_count(grid->hitBoxes);
    for(j=0;j<num_hitboxes;j++)
    {
      hitbox = gfc_list_get_nth(grid->hitBoxes,j);
      hitx = hitbox->box->posx;
      hitx = ((hitx-grid->posx)/grid->xspacing) + (cells_per_row*i);

      hity = hitbox->box->posy;
      hity = (hity-grid->posy)/grid->yspacing;

      hitheight = (hitbox->box->height)/grid->yspacing;
      hitwidth = (hitbox->box->width)/grid->xspacing;
      jhitbox = gf2d_grid_to_json_create_hitbox(hitx,hity,hitheight,hitwidth);
      sj_array_append(hitboxArray,jhitbox);
    }
  }
  sj_object_insert(object,"hitboxes",hitboxArray);




  sj_save(object,filename);
}

SJson * gf2d_grid_to_json_create_hitbox(int posx,int posy, int width,int height)
{
  SJson *hitbox;
  SJString *str;
  SJson *val;
  hitbox = sj_object_new();

  str=sj_string_new_integer(posx);
  val = sj_string_to_value(str);
  sj_object_insert(hitbox,"posx",val);

  str=sj_string_new_integer(posy);
  val = sj_string_to_value(str);
  sj_object_insert(hitbox,"posy",val);

  str=sj_string_new_integer(height);
  val = sj_string_to_value(str);
  sj_object_insert(hitbox,"height",val);

  str=sj_string_new_integer(width);
  val = sj_string_to_value(str);
  sj_object_insert(hitbox,"width",val);

  return hitbox;
}


SJson * gf2d_grid_to_json_create_json_cell(BoxInfo * cell,int posx,int posy)
{
  SJson *jcell;
  SJString *str;
  SJson *val;
  jcell = sj_object_new();

  if(!(cell->s))
  {
    str = sj_string_new_text("yes");
    val = sj_string_to_value(str);
    sj_object_insert(jcell,"empty cell?",val);

    str = sj_string_new_integer(posx);
    val = sj_string_to_value(str);
    sj_object_insert(jcell,"relative x position",val);

    str = sj_string_new_integer(posy);
    val = sj_string_to_value(str);
    sj_object_insert(jcell,"relative y position",val);
  }
  else
  {
    str = sj_string_new_text("no");
    val = sj_string_to_value(str);
    sj_object_insert(jcell,"empty cell?",val);

    str = sj_string_new_integer(posx);
    val = sj_string_to_value(str);
    sj_object_insert(jcell,"relative x position",val);

    str = sj_string_new_integer(posy);
    val = sj_string_to_value(str);
    sj_object_insert(jcell,"relative y position",val);

    str = sj_string_new_integer(cell->width);
    val = sj_string_to_value(str);
    sj_object_insert(jcell,"width",val);

    str = sj_string_new_integer(cell->height);
    val = sj_string_to_value(str);
    sj_object_insert(jcell,"height",val);

    str = sj_string_new_float(cell->resizex);
    val = sj_string_to_value(str);
    sj_object_insert(jcell,"resizex",val);

    str = sj_string_new_float(cell->resizey);
    val = sj_string_to_value(str);
    sj_object_insert(jcell,"resizey",val);

    str = sj_string_new_integer(cell->sprite_num);
    val = sj_string_to_value(str);
    sj_object_insert(jcell,"sprite_num",val);

    str = sj_string_new_text((cell->s->filepath));
    val = sj_string_to_value(str);
    sj_object_insert(jcell,"file_location",val);

    str = sj_string_new_integer(cell->s->frames_per_line);
    val = sj_string_to_value(str);
    sj_object_insert(jcell,"frames_per_line",val);

    str = sj_string_new_integer(cell->s->total_w);
    val = sj_string_to_value(str);
    sj_object_insert(jcell,"total_w",val);

    str = sj_string_new_integer(cell->s->total_h);
    val = sj_string_to_value(str);
    sj_object_insert(jcell,"total_h",val);
  }
  return jcell;
}
