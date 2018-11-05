# Lua Magic
Bind C functions to Lua automatically by variadic templates of C++ .  

## Environment
* C++ 11  
* Lua 5.3.5  

## Notice
* Override function must be indicated type.
* Lambda funtion must not catch variable.
* ``std::function`` is unsupported.

## API
```C++
template<typename T>
int luaMagic::setValue(lua_State* L, const char* name, T value);

template<typename T>
int luaMagic::bind(lua_State* L, const char* name, T function);

template<typename T>
int luaMagicOverride<T>::bind()(lua_State* L, const char* name, T function);
```

## Install
```bash
mkdir -p /usr/local/include/luaMagic
cp luaMagic.hpp /usr/local/include/luaMagic/luaMagic.hpp
cp luaMagicRead.hpp /usr/local/include/luaMagic/luaMagicRead.hpp
cp luaMagicWrite.hpp /usr/local/include/luaMagic/luaMagicWrite.hpp
```

## Usage
* Include
```C++
#include <luaMagic/luaMagic.hpp>
```

* Define your C function.
```C++
int hello(const char* str, int x, double y)
{
	printf("%s %d %lf\n", str, x, y);
	return 0;
}

int add(int x, int y)
{
	printf("add int\n");
	return x + y;
}

double add(double x, double y)
{
	printf("add double\n");
	return x + y;
}
```

* The bind
```C++
luaMagic::bind(L, "hello", hello);

luaMagicOverride<int(int,int)>::bind(L, "intadd", add);

luaMagicOverride<double(double,double)>::bind(L, "realadd", add);
```

## Complex Type
If using complex variable type like struct , you have to specialize ``luaMagic_read`` and ``luaMagic_write``.  
```C++
/* Read parameter from Lua */
template<typename T>
T luaMagic_read(lua_State* L, int index);

/* Write return value to Lua */
template<typename T>
void luaMagic_write(lua_State* L, T value);
```
You can refer to [here](https://github.com/hubenchang0515/SDLuna)


## Demo
* Source
```C++
#include "luaMagic.hpp"
#include <cstring>
#include <iostream>


int luaMagic_load(lua_State* L);
double add(double x, double y);
void upper(const char* str);
void hello(const char* str, int x, double y);
void commandLine(lua_State *L);

int main(int argc,char* argv[])
{
    /* 创建lua_State */
    lua_State* L = luaL_newstate();
    /* 创建失败，退出 */
    if(L == NULL)
    {
        fprintf(stderr,"%s : Cannot create state.\n",argv[0]);
        return 1;
    }
    /* 打开所有Lua标准库 */
    luaL_openlibs(L);
	
	/* 绑定函数 */
	luaMagic_load(L);
	
    /* 运行交互式命令行 */
    commandLine(L);
}

int luaMagic_load(lua_State* L)
{
	luaMagic::bind(L, "add", add);
	luaMagic::bind(L, "upper", upper);
	luaMagic::bind(L, "hello", hello);
}

double add(double x, double y)
{
	return x+y;
}

void upper(const char* str)
{
	for(int i = 0 ; i < strlen(str); i++)
	{
		putchar(str[i] & ~(1<<5));
	}
	putchar('\n');
}

void hello(const char* str, int x, double y)
{
	printf("%s %d %lf\n", str, x, y);
}

void commandLine(lua_State *L)
{
	while (1) 
	{
	char cmd[256];
	printf(">> ");
	std::cin.get(cmd,256);
	getchar();
	luaL_dostring(L, cmd);
	}
}
```
  
* Result  
```
>> hello("hello", 2018, 10.31)
hello 2018 10.310000
>> upper("hello WORLD")
HELLO WORLD
>> print(add(233, 100.55))
333.55
>>
```
