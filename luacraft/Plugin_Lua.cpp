/*
** Lua binding: Plugin
** Generated automatically by tolua++-1.0.92 on 04/10/10 13:06:05.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"
#include "PluginManager.h"

/* Exported function */
TOLUA_API int  tolua_Plugin_open (lua_State* tolua_S);


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Plugin");
}

/* method: Print of class  Plugin */
#ifndef TOLUA_DISABLE_tolua_Plugin_Plugin_Print00
static int tolua_Plugin_Plugin_Print00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Plugin",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Plugin* self = (Plugin*)  tolua_tousertype(tolua_S,1,0);
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Print'", NULL);
#endif
  {
   self->Print(msg);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Print'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddHook of class  Plugin */
#ifndef TOLUA_DISABLE_tolua_Plugin_Plugin_AddHook00
static int tolua_Plugin_Plugin_AddHook00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Plugin",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Plugin* self = (Plugin*)  tolua_tousertype(tolua_S,1,0);
  const char* _hook = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* _function = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddHook'", NULL);
#endif
  {
   self->AddHook(_hook,_function);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddHook'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetName of class  Plugin */
#ifndef TOLUA_DISABLE_tolua_Plugin_Plugin_SetName00
static int tolua_Plugin_Plugin_SetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Plugin",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Plugin* self = (Plugin*)  tolua_tousertype(tolua_S,1,0);
  const char* _name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetName'", NULL);
#endif
  {
   self->SetName(_name);
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

/* method: GetName of class  Plugin */
#ifndef TOLUA_DISABLE_tolua_Plugin_Plugin_GetName00
static int tolua_Plugin_Plugin_GetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Plugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Plugin* self = (Plugin*)  tolua_tousertype(tolua_S,1,0);
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

/* method: Include of class  Plugin */
#ifndef TOLUA_DISABLE_tolua_Plugin_Plugin_Include00
static int tolua_Plugin_Plugin_Include00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Plugin",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Plugin* self = (Plugin*)  tolua_tousertype(tolua_S,1,0);
  const char* _file = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Include'", NULL);
#endif
  {
   self->Include(_file);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Include'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Unload of class  Plugin */
#ifndef TOLUA_DISABLE_tolua_Plugin_Plugin_Unload00
static int tolua_Plugin_Plugin_Unload00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Plugin",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Plugin* self = (Plugin*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Unload'", NULL);
#endif
  {
   self->Unload();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Unload'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_Plugin_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"Plugin","Plugin","",NULL);
  tolua_beginmodule(tolua_S,"Plugin");
   tolua_function(tolua_S,"Print",tolua_Plugin_Plugin_Print00);
   tolua_function(tolua_S,"AddHook",tolua_Plugin_Plugin_AddHook00);
   tolua_function(tolua_S,"SetName",tolua_Plugin_Plugin_SetName00);
   tolua_function(tolua_S,"GetName",tolua_Plugin_Plugin_GetName00);
   tolua_function(tolua_S,"Include",tolua_Plugin_Plugin_Include00);
   tolua_function(tolua_S,"Unload",tolua_Plugin_Plugin_Unload00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Plugin (lua_State* tolua_S) {
 return tolua_Plugin_open(tolua_S);
};
#endif

