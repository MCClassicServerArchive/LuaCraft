/*
** Lua binding: Server
** Generated automatically by tolua++-1.0.92 on 04/23/10 20:25:20.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"
#include "Server.h"

/* Exported function */
TOLUA_API int  tolua_Server_open (lua_State* tolua_S);


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_block (lua_State* tolua_S)
{
 block* self = (block*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Player");
 tolua_usertype(tolua_S,"Zone");
 tolua_usertype(tolua_S,"Level");
 tolua_usertype(tolua_S,"block");
 tolua_usertype(tolua_S,"Server");
}

/* get function: Quit of class  Server */
#ifndef TOLUA_DISABLE_tolua_get_Server_Quit
static int tolua_get_Server_Quit(lua_State* tolua_S)
{
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Quit'",NULL);
#endif
  tolua_pushboolean(tolua_S,(bool)self->Quit);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Quit of class  Server */
#ifndef TOLUA_DISABLE_tolua_set_Server_Quit
static int tolua_set_Server_Quit(lua_State* tolua_S)
{
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Quit'",NULL);
  if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Quit = ((bool)  tolua_toboolean(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: CreateWorld of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_CreateWorld00
static int tolua_Server_Server_CreateWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  const char* worldName = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* fileName = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CreateWorld'", NULL);
#endif
  {
   self->CreateWorld(worldName,fileName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreateWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLevelByName of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_GetLevelByName00
static int tolua_Server_Server_GetLevelByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  const char* worldName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLevelByName'", NULL);
#endif
  {
   Level* tolua_ret = (Level*)  self->GetLevelByName(worldName);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Level");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLevelByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DeleteWorld of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_DeleteWorld00
static int tolua_Server_Server_DeleteWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  const char* worldName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DeleteWorld'", NULL);
#endif
  {
   self->DeleteWorld(worldName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DeleteWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetGamemodeName of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_GetGamemodeName00
static int tolua_Server_Server_GetGamemodeName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetGamemodeName'", NULL);
#endif
  {
   char* tolua_ret = (char*)  self->GetGamemodeName();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetGamemodeName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPluginByName of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_GetPluginByName00
static int tolua_Server_Server_GetPluginByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  const char* pluginName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPluginByName'", NULL);
#endif
  {
   self->GetPluginByName(pluginName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPluginByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: LoadPlugin of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_LoadPlugin00
static int tolua_Server_Server_LoadPlugin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  const char* fileName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LoadPlugin'", NULL);
#endif
  {
   self->LoadPlugin(fileName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LoadPlugin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPlayerByName of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_GetPlayerByName00
static int tolua_Server_Server_GetPlayerByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPlayerByName'", NULL);
#endif
  {
   Player* tolua_ret = (Player*)  self->GetPlayerByName(name);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Player");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPlayerByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendChat of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_SendChat00
static int tolua_Server_Server_SendChat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  const char* message = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendChat'", NULL);
#endif
  {
   self->SendChat(message);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendChat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendChat of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_SendChat01
static int tolua_Server_Server_SendChat01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  const char* message = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* world = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendChat'", NULL);
#endif
  {
   self->SendChat(message,world);
  }
 }
 return 0;
tolua_lerror:
 return tolua_Server_Server_SendChat00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendMove of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_SendMove00
static int tolua_Server_Server_SendMove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Player",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  Player* player = ((Player*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendMove'", NULL);
#endif
  {
   self->SendMove(player);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendMove'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: PlaceBlock of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_PlaceBlock00
static int tolua_Server_Server_PlaceBlock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"block",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  block nblock = *((block*)  tolua_tousertype(tolua_S,2,0));
  char* world = ((char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'PlaceBlock'", NULL);
#endif
  {
   self->PlaceBlock(nblock,world);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PlaceBlock'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBlock of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_GetBlock00
static int tolua_Server_Server_GetBlock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  char* world = ((char*)  tolua_tostring(tolua_S,2,0));
  short x = ((short)  tolua_tonumber(tolua_S,3,0));
  short y = ((short)  tolua_tonumber(tolua_S,4,0));
  short z = ((short)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBlock'", NULL);
#endif
  {
   block tolua_ret = (block)  self->GetBlock(world,x,y,z);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((block)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"block");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(block));
     tolua_pushusertype(tolua_S,tolua_obj,"block");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBlock'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWorldSizeX of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_GetWorldSizeX00
static int tolua_Server_Server_GetWorldSizeX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  char* world = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWorldSizeX'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetWorldSizeX(world);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWorldSizeX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWorldSizeY of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_GetWorldSizeY00
static int tolua_Server_Server_GetWorldSizeY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  char* world = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWorldSizeY'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetWorldSizeY(world);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWorldSizeY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWorldSizeZ of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_GetWorldSizeZ00
static int tolua_Server_Server_GetWorldSizeZ00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  char* world = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWorldSizeZ'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetWorldSizeZ(world);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWorldSizeZ'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetName of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_GetName00
static int tolua_Server_Server_GetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetName'", NULL);
#endif
  {
   char* tolua_ret = (char*)  self->GetName();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetMotd of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_GetMotd00
static int tolua_Server_Server_GetMotd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetMotd'", NULL);
#endif
  {
   char* tolua_ret = (char*)  self->GetMotd();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetMotd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetName of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_SetName00
static int tolua_Server_Server_SetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetName'", NULL);
#endif
  {
   self->SetName(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMotd of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_SetMotd00
static int tolua_Server_Server_SetMotd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  const char* motd = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMotd'", NULL);
#endif
  {
   self->SetMotd(motd);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMotd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: NumPlayers of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_NumPlayers00
static int tolua_Server_Server_NumPlayers00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'NumPlayers'", NULL);
#endif
  {
   int tolua_ret = (int)  self->NumPlayers();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'NumPlayers'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: NumPlayersInWorld of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_NumPlayersInWorld00
static int tolua_Server_Server_NumPlayersInWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  const char* world = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'NumPlayersInWorld'", NULL);
#endif
  {
   int tolua_ret = (int)  self->NumPlayersInWorld(world);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'NumPlayersInWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MaxPlayers of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_MaxPlayers00
static int tolua_Server_Server_MaxPlayers00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MaxPlayers'", NULL);
#endif
  {
   int tolua_ret = (int)  self->MaxPlayers();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MaxPlayers'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CreateZone of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_CreateZone00
static int tolua_Server_Server_CreateZone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"block",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"block",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  char* _world = ((char*)  tolua_tostring(tolua_S,2,0));
  block _block1 = *((block*)  tolua_tousertype(tolua_S,3,0));
  block _block2 = *((block*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CreateZone'", NULL);
#endif
  {
   Zone* tolua_ret = (Zone*)  self->CreateZone(_world,_block1,_block2);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Zone");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreateZone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DeleteZone of class  Server */
#ifndef TOLUA_DISABLE_tolua_Server_Server_DeleteZone00
static int tolua_Server_Server_DeleteZone00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Zone",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  Zone* _zone = ((Zone*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DeleteZone'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->DeleteZone(_zone);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DeleteZone'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_Server_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"Server","Server","",NULL);
  tolua_beginmodule(tolua_S,"Server");
   tolua_variable(tolua_S,"Quit",tolua_get_Server_Quit,tolua_set_Server_Quit);
   tolua_function(tolua_S,"CreateWorld",tolua_Server_Server_CreateWorld00);
   tolua_function(tolua_S,"GetLevelByName",tolua_Server_Server_GetLevelByName00);
   tolua_function(tolua_S,"DeleteWorld",tolua_Server_Server_DeleteWorld00);
   tolua_function(tolua_S,"GetGamemodeName",tolua_Server_Server_GetGamemodeName00);
   tolua_function(tolua_S,"GetPluginByName",tolua_Server_Server_GetPluginByName00);
   tolua_function(tolua_S,"LoadPlugin",tolua_Server_Server_LoadPlugin00);
   tolua_function(tolua_S,"GetPlayerByName",tolua_Server_Server_GetPlayerByName00);
   tolua_function(tolua_S,"SendChat",tolua_Server_Server_SendChat00);
   tolua_function(tolua_S,"SendChat",tolua_Server_Server_SendChat01);
   tolua_function(tolua_S,"SendMove",tolua_Server_Server_SendMove00);
   tolua_function(tolua_S,"PlaceBlock",tolua_Server_Server_PlaceBlock00);
   tolua_function(tolua_S,"GetBlock",tolua_Server_Server_GetBlock00);
   tolua_function(tolua_S,"GetWorldSizeX",tolua_Server_Server_GetWorldSizeX00);
   tolua_function(tolua_S,"GetWorldSizeY",tolua_Server_Server_GetWorldSizeY00);
   tolua_function(tolua_S,"GetWorldSizeZ",tolua_Server_Server_GetWorldSizeZ00);
   tolua_function(tolua_S,"GetName",tolua_Server_Server_GetName00);
   tolua_function(tolua_S,"GetMotd",tolua_Server_Server_GetMotd00);
   tolua_function(tolua_S,"SetName",tolua_Server_Server_SetName00);
   tolua_function(tolua_S,"SetMotd",tolua_Server_Server_SetMotd00);
   tolua_function(tolua_S,"NumPlayers",tolua_Server_Server_NumPlayers00);
   tolua_function(tolua_S,"NumPlayersInWorld",tolua_Server_Server_NumPlayersInWorld00);
   tolua_function(tolua_S,"MaxPlayers",tolua_Server_Server_MaxPlayers00);
   tolua_function(tolua_S,"CreateZone",tolua_Server_Server_CreateZone00);
   tolua_function(tolua_S,"DeleteZone",tolua_Server_Server_DeleteZone00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Server (lua_State* tolua_S) {
 return tolua_Server_open(tolua_S);
};
#endif

