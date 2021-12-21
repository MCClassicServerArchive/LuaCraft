/*
** Lua binding: Zone
** Generated automatically by tolua++-1.0.92 on 04/23/10 19:45:45.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"
#include "Zone.h"

/* Exported function */
TOLUA_API int  tolua_Zone_open (lua_State* tolua_S);


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Zone");
}

/* method: Fill of class  Zone */
#ifndef TOLUA_DISABLE_tolua_Zone_Zone_Fill00
static int tolua_Zone_Zone_Fill00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Zone",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Zone* self = (Zone*)  tolua_tousertype(tolua_S,1,0);
  unsigned char blocktype = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Fill'", NULL);
#endif
  {
   self->Fill(blocktype);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Fill'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_Zone_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"Zone","Zone","",NULL);
  tolua_beginmodule(tolua_S,"Zone");
   tolua_function(tolua_S,"Fill",tolua_Zone_Zone_Fill00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Zone (lua_State* tolua_S) {
 return tolua_Zone_open(tolua_S);
};
#endif

