#pragma once
#include <directxtk/SimpleMath.h>
#include "Vector3D.h"


struct Matrix4x4
{
    float m_11, m_12, m_13, m_14;
    float m_21, m_22, m_23, m_24;
    float m_31, m_32, m_33, m_34;
    float m_41, m_42, m_43, m_44;

    [[nodiscard]]DirectX::SimpleMath::Matrix ConvertToMatrix() const
    {
        DirectX::SimpleMath::Matrix mat = { m_11, m_12, m_13, m_14,
                                            m_21, m_22, m_23, m_24,
                                            m_31, m_32, m_33, m_34,
                                            m_41, m_42, m_43, m_44 };
        return mat;
    }

    Vector3D ConverToVector3D() const
    {
        return Vector3D(m_41, m_42, m_43);
    }

    Matrix4x4& operator=(const DirectX::SimpleMath::Matrix& mat)
    {
        *this =  Matrix4x4{ mat._11, mat._12, mat._13, mat._14,
                          mat._21, mat._22, mat._23, mat._24,
                           mat._31,mat._32, mat._33, mat._34,
                          mat._41, mat._42, mat._43, mat._44 };

        return *this;
    }

};