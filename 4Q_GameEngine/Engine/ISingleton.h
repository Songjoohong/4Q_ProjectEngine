#pragma once
#pragma once
#include <memory>
// 스마트 포인터로 만들면 소멸 순서에 문제가 생길 수 있고
// 전체 프로그램 기간 동안 유지되어야 하기 때문에 
// 굳이 참조 계산같은 추가 오버헤드를 발생시킬 필요가 없다

template<typename T>
class ISingleton
{
protected:
    ISingleton() = default;
    virtual ~ISingleton() {}

public:
    static T* GetInstance()
    {
        if (m_pInstance == nullptr)
            m_pInstance = new T;
        return m_pInstance;
    }

private:
    static T* m_pInstance;
};

template<typename T>
T* ISingleton<T>::m_pInstance = nullptr;