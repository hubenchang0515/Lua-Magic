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

#ifndef LUA_MAGIC_HPP
#define LUA_MAGIC_HPP

#include "luaMagicRead.hpp"
#include "luaMagicWrite.hpp"

/* bind a value to first argument of normal function , return the new function */
template<typename ReturnType,typename FirstType, typename... ArgTypes>
std::function<ReturnType(ArgTypes...)> luaMagic_warp(ReturnType(func)(FirstType,ArgTypes...) , FirstType value)
{
	std::function<ReturnType(ArgTypes...)> rfunc = [func,value](ArgTypes... args)->ReturnType{return func(value, args...);};
	return rfunc;
}

/* bind a value to first argument of std::function , return the new function */
template<typename ReturnType,typename FirstType, typename... ArgTypes>
std::function<ReturnType(ArgTypes...)> luaMagic_warp(std::function<ReturnType(FirstType,ArgTypes...)> func, FirstType value)
{
	std::function<ReturnType(ArgTypes...)> rfunc = [func,value](ArgTypes... args)->ReturnType{return func(value, args...);};
	return rfunc;
}

/* Invoke normal function */
template<int index = 1, typename RetType, typename FirstType, typename... ArgTypes>
RetType luaMagic_invoke(lua_State* L,  RetType (func)(FirstType, ArgTypes...))
{
//	printf("%s %d\n", __FILE__, __LINE__);
	auto newFunc = luaMagic_warp(func, luaMagic_read<FirstType>(L, index));
	return luaMagic_invoke<index+1>(L, newFunc);
}

/* Invoke std::function */
template<int index = 1, typename RetType, typename FirstType, typename... ArgTypes>
RetType luaMagic_invoke(lua_State* L,  std::function<RetType(FirstType, ArgTypes...)> func)
{
//	printf("%s %d\n", __FILE__, __LINE__);
	auto newFunc = luaMagic_warp(func, luaMagic_read<FirstType>(L, index));
	return luaMagic_invoke<index+1>(L, newFunc);
}

/* Invoke normal function with only 1 argument */
template<int index = 1, typename RetType, typename ArgType>
RetType luaMagic_invoke(lua_State* L,  RetType (func)(ArgType))
{
//	printf("%s %d\n", __FILE__, __LINE__);
	return func(luaMagic_read<ArgType>(L, index));
}

/* Invoke std::function with only 1 argument */
template<int index = 1, typename RetType, typename ArgType>
RetType luaMagic_invoke(lua_State* L,  std::function<RetType(ArgType)> func)
{
//	printf("%s %d\n", __FILE__, __LINE__);
	return func(luaMagic_read<ArgType>(L, index));
}

/* Bind to Lua */
template<typename RetType, typename... ArgTypes>
int luaMagic_bind2Lua(lua_State* L, const char* name, RetType (func)(ArgTypes...))
{
	int (*bridgeFunc)(lua_State*) = [](lua_State* L) -> int 
	{
		/* get the true funtion from closure */
		RetType (*fp)(ArgTypes...) = reinterpret_cast<RetType (*)(ArgTypes...)>(lua_touserdata(L,lua_upvalueindex(1)));
		luaMagic_write<RetType>(L, luaMagic_invoke(L, fp)); 
		
		return 1;
	};
	
	/* func is a upvalue of bridge function   */
	lua_pushlightuserdata(L, reinterpret_cast<void*>(func));
	/* create a closure of function and set upvalue */
	lua_pushcclosure(L, bridgeFunc, 1);
	/* set the lua function name */
	lua_setglobal(L, name);
	
	return 0;
}


/* Bind to Lua : WHILE return void */
template<typename... ArgTypes>
inline int luaMagic_bind2Lua(lua_State* L, const char* name, void (func)(ArgTypes...))
{
	int (*bridgeFunc)(lua_State*) = [](lua_State* L) -> int 
	{
		/* get the true funtion from closure */
		void (*fp)(ArgTypes...) = reinterpret_cast<void (*)(ArgTypes...)>(lua_touserdata(L,lua_upvalueindex(1)));
		luaMagic_invoke(L, fp); 
		
		return 0;
	};
	
	/* func is a upvalue of bridge function   */
	lua_pushlightuserdata(L, reinterpret_cast<void*>(func));
	/* create a closure of function and set upvalue */
	lua_pushcclosure(L, bridgeFunc, 1);
	/* set the lua function name */
	lua_setglobal(L, name);
	
	return 0;
}

#endif