#pragma once
#include "../ReflectionLib/PropertyMacro.h"
#include "../ReflectionLib/TypeInfoMacro.h"

class Test
{
public:
	Test() {}
	~Test() {}

	int a = 0;
	int b = 1;
	int c = 2;

	GENERATE_CLASS_TYPE_INFO(Test)
	PROPERTY(a)
	PROPERTY(b)
	PROPERTY(c)

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Test, a, b, c)
};

struct
{

};

struct
{

};
