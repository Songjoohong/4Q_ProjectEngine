#pragma once

#include "TypeInfo.h"

#define GENERATE_CLASS_TYPE_INFO(TypeName) \
private: \
    friend struct SuperClassTypeDeduction<TypeName>; \
    friend struct TypeInfoInitializer<TypeName>; \
\
public: \
    using Super = typename SuperClassTypeDeduction<TypeName>::Type; \
    using ThisType = TypeName; \
\
    static TypeInfo& StaticTypeInfo() \
    { \
        static TypeInfo typeInfo{ TypeInfoInitializer<TypeName>(#TypeName) }; \
        return typeInfo; \
    } \
\
    virtual const TypeInfo& GetTypeInfo() const\
    { \
        return m_typeInfo; \
    } \
\
private: \
	inline static TypeInfo& m_typeInfo = StaticTypeInfo(); \
\
private: \
