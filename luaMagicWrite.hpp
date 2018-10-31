/******************************************************************************
MIT License

Copyright (c) 2018 Plan C

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef LUA_MAGIC_WRITE_HPP
#define LUA_MAGIC_WRITE_HPP

#include <lua.hpp>
#include <functional>

/* Write value to Lua */
template<typename T>
void luaMagic_write(lua_State* L, T value)
{
	T* ptr = lua_newuserdata(L, sizeof(L));
	T* object = new(ptr) T();
}

template<>
inline void luaMagic_write<bool>(lua_State* L, bool value)
{
	if(value)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
}

template<> 
inline void luaMagic_write<short>(lua_State* L, short value)
{
	lua_pushinteger(L, value);
}

template<> 
inline void luaMagic_write<int>(lua_State* L, int value)
{
	lua_pushinteger(L, value);
}

template<> 
inline void luaMagic_write<long>(lua_State* L, long value)
{
	lua_pushinteger(L, value);
}

template<> 
inline void luaMagic_write<long long>(lua_State* L, long long value)
{
	lua_pushinteger(L, value);
}


template<>
inline void luaMagic_write<float>(lua_State* L, float value)
{
	lua_pushnumber(L, value);
}

template<>
inline void luaMagic_write<double>(lua_State* L, double value)
{
	lua_pushnumber(L, value);
}

template<>
inline void luaMagic_write<const char*>(lua_State* L, const char* value)
{
	lua_pushstring(L, value);
}


template<>
inline void luaMagic_write<void*>(lua_State* L, void* value)
{
	lua_pushlightuserdata(L, value);
}


#endif