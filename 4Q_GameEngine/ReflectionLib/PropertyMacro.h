#pragma once
#include "Property.h"
#define PROPERTY( Name )	\
	inline static struct RegistPropertyExecutor_##Name	\
	{	\
		RegistPropertyExecutor_##Name()	\
		{	\
			static PropertyRegister<ThisType, decltype(##Name), decltype(&ThisType::##Name), &ThisType::##Name> property_register_##Name{ #Name, ThisType::StaticTypeInfo() }; \
		}	\
		\
	} regist_##Name;