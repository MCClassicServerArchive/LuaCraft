@echo off
set INPUT=
set /P INPUT=Name of Lua class: 
cls
echo You selected to create a Lua class out of %INPUT%.
echo.
echo Working...
echo.
@"tolua++.exe" -o %INPUT%_Lua.cpp -H %INPUT%_Lua.h -n %INPUT% %INPUT%.pkg
echo Done.
echo.
@pause