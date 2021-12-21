/*
** Lua binding: Structs
** Generated automatically by tolua++-1.0.92 on 04/19/10 16:32:56.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"
#include "Structs.h"

/* Exported function */
TOLUA_API int  tolua_Structs_open (lua_State* tolua_S);


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"block");
 tolua_usertype(tolua_S,"Position");
}

/* get function: x of class  Position */
#ifndef TOLUA_DISABLE_tolua_get_Position_unsigned_x
static int tolua_get_Position_unsigned_x(lua_State* tolua_S)
{
  Position* self = (Position*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  Position */
#ifndef TOLUA_DISABLE_tolua_set_Position_unsigned_x
static int tolua_set_Position_unsigned_x(lua_State* tolua_S)
{
  Position* self = (Position*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((unsigned double)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  Position */
#ifndef TOLUA_DISABLE_tolua_get_Position_unsigned_y
static int tolua_get_Position_unsigned_y(lua_State* tolua_S)
{
  Position* self = (Position*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  Position */
#ifndef TOLUA_DISABLE_tolua_set_Position_unsigned_y
static int tolua_set_Position_unsigned_y(lua_State* tolua_S)
{
  Position* self = (Position*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((unsigned double)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: z of class  Position */
#ifndef TOLUA_DISABLE_tolua_get_Position_unsigned_z
static int tolua_get_Position_unsigned_z(lua_State* tolua_S)
{
  Position* self = (Position*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: z of class  Position */
#ifndef TOLUA_DISABLE_tolua_set_Position_unsigned_z
static int tolua_set_Position_unsigned_z(lua_State* tolua_S)
{
  Position* self = (Position*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->z = ((unsigned double)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: heading of class  Position */
#ifndef TOLUA_DISABLE_tolua_get_Position_unsigned_heading
static int tolua_get_Position_unsigned_heading(lua_State* tolua_S)
{
  Position* self = (Position*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'heading'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->heading);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: heading of class  Position */
#ifndef TOLUA_DISABLE_tolua_set_Position_unsigned_heading
static int tolua_set_Position_unsigned_heading(lua_State* tolua_S)
{
  Position* self = (Position*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'heading'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->heading = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pitch of class  Position */
#ifndef TOLUA_DISABLE_tolua_get_Position_unsigned_pitch
static int tolua_get_Position_unsigned_pitch(lua_State* tolua_S)
{
  Position* self = (Position*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pitch'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->pitch);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pitch of class  Position */
#ifndef TOLUA_DISABLE_tolua_set_Position_unsigned_pitch
static int tolua_set_Position_unsigned_pitch(lua_State* tolua_S)
{
  Position* self = (Position*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pitch'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pitch = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  block */
#ifndef TOLUA_DISABLE_tolua_get_block_unsigned_x
static int tolua_get_block_unsigned_x(lua_State* tolua_S)
{
  block* self = (block*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  block */
#ifndef TOLUA_DISABLE_tolua_set_block_unsigned_x
static int tolua_set_block_unsigned_x(lua_State* tolua_S)
{
  block* self = (block*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((unsigned short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  block */
#ifndef TOLUA_DISABLE_tolua_get_block_unsigned_y
static int tolua_get_block_unsigned_y(lua_State* tolua_S)
{
  block* self = (block*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  block */
#ifndef TOLUA_DISABLE_tolua_set_block_unsigned_y
static int tolua_set_block_unsigned_y(lua_State* tolua_S)
{
  block* self = (block*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((unsigned short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: z of class  block */
#ifndef TOLUA_DISABLE_tolua_get_block_unsigned_z
static int tolua_get_block_unsigned_z(lua_State* tolua_S)
{
  block* self = (block*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: z of class  block */
#ifndef TOLUA_DISABLE_tolua_set_block_unsigned_z
static int tolua_set_block_unsigned_z(lua_State* tolua_S)
{
  block* self = (block*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->z = ((unsigned short)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: blocktype of class  block */
#ifndef TOLUA_DISABLE_tolua_get_block_unsigned_blocktype
static int tolua_get_block_unsigned_blocktype(lua_State* tolua_S)
{
  block* self = (block*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'blocktype'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->blocktype);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: blocktype of class  block */
#ifndef TOLUA_DISABLE_tolua_set_block_unsigned_blocktype
static int tolua_set_block_unsigned_blocktype(lua_State* tolua_S)
{
  block* self = (block*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'blocktype'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->blocktype = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mode of class  block */
#ifndef TOLUA_DISABLE_tolua_get_block_unsigned_mode
static int tolua_get_block_unsigned_mode(lua_State* tolua_S)
{
  block* self = (block*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mode'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->mode);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mode of class  block */
#ifndef TOLUA_DISABLE_tolua_set_block_unsigned_mode
static int tolua_set_block_unsigned_mode(lua_State* tolua_S)
{
  block* self = (block*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mode'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mode = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: levelblock of class  block */
#ifndef TOLUA_DISABLE_tolua_get_block_unsigned_levelblock
static int tolua_get_block_unsigned_levelblock(lua_State* tolua_S)
{
  block* self = (block*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'levelblock'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->levelblock);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: levelblock of class  block */
#ifndef TOLUA_DISABLE_tolua_set_block_unsigned_levelblock
static int tolua_set_block_unsigned_levelblock(lua_State* tolua_S)
{
  block* self = (block*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'levelblock'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->levelblock = ((unsigned char)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_Structs_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"Position","Position","",NULL);
  tolua_beginmodule(tolua_S,"Position");
   tolua_variable(tolua_S,"x",tolua_get_Position_unsigned_x,tolua_set_Position_unsigned_x);
   tolua_variable(tolua_S,"y",tolua_get_Position_unsigned_y,tolua_set_Position_unsigned_y);
   tolua_variable(tolua_S,"z",tolua_get_Position_unsigned_z,tolua_set_Position_unsigned_z);
   tolua_variable(tolua_S,"heading",tolua_get_Position_unsigned_heading,tolua_set_Position_unsigned_heading);
   tolua_variable(tolua_S,"pitch",tolua_get_Position_unsigned_pitch,tolua_set_Position_unsigned_pitch);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"block","block","",NULL);
  tolua_beginmodule(tolua_S,"block");
   tolua_variable(tolua_S,"x",tolua_get_block_unsigned_x,tolua_set_block_unsigned_x);
   tolua_variable(tolua_S,"y",tolua_get_block_unsigned_y,tolua_set_block_unsigned_y);
   tolua_variable(tolua_S,"z",tolua_get_block_unsigned_z,tolua_set_block_unsigned_z);
   tolua_variable(tolua_S,"blocktype",tolua_get_block_unsigned_blocktype,tolua_set_block_unsigned_blocktype);
   tolua_variable(tolua_S,"mode",tolua_get_block_unsigned_mode,tolua_set_block_unsigned_mode);
   tolua_variable(tolua_S,"levelblock",tolua_get_block_unsigned_levelblock,tolua_set_block_unsigned_levelblock);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Structs (lua_State* tolua_S) {
 return tolua_Structs_open(tolua_S);
};
#endif

