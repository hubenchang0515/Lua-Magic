# Lua Magic
Bind C functions to Lua automatically by variadic templates of C++ .    
* C++ 11  
* Lua 5.3.5  

## API
```C++
template<typename T>
int luaMagic_bind2Lua(lua_State* L, const char* name, T function);
```

## Install
```shell
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
```
* The bind
```C++
luaMagic_bind2Lua(L, "hello", hello);
```


## Demo
* Source
```C++
#include "luaMagic.hpp"
#include <cstring>


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
	luaMagic_bind2Lua(L, "add", add);
	luaMagic_bind2Lua(L, "upper", upper);
	luaMagic_bind2Lua(L, "hello", hello);
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
	gets(cmd);
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