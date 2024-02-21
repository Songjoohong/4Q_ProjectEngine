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

    Matrix4x4 operator*(const Matrix4x4& matrix1)
    {
        Matrix4x4 result;

        result.m_11 = matrix1.m_11 * m_11 + matrix1.m_12 * m_21 + matrix1.m_13 * m_31 + matrix1.m_14 * m_41;
        result.m_12 = matrix1.m_11 * m_12 + matrix1.m_12 * m_22 + matrix1.m_13 * m_32 + matrix1.m_14 * m_42;
        result.m_13 = matrix1.m_11 * m_13 + matrix1.m_12 * m_23 + matrix1.m_13 * m_33 + matrix1.m_14 * m_43;
        result.m_14 = matrix1.m_11 * m_14 + matrix1.m_12 * m_24 + matrix1.m_13 * m_34 + matrix1.m_14 * m_44;

        result.m_21 = matrix1.m_21 * m_11 + matrix1.m_22 * m_21 + matrix1.m_23 * m_31 + matrix1.m_24 * m_41;
        result.m_22 = matrix1.m_21 * m_12 + matrix1.m_22 * m_22 + matrix1.m_23 * m_32 + matrix1.m_24 * m_42;
        result.m_23 = matrix1.m_21 * m_13 + matrix1.m_22 * m_23 + matrix1.m_23 * m_33 + matrix1.m_24 * m_43;
        result.m_24 = matrix1.m_21 * m_14 + matrix1.m_22 * m_24 + matrix1.m_23 * m_34 + matrix1.m_24 * m_44;

        result.m_31 = matrix1.m_31 * m_11 + matrix1.m_32 * m_21 + matrix1.m_33 * m_31 + matrix1.m_34 * m_41;
        result.m_32 = matrix1.m_31 * m_12 + matrix1.m_32 * m_22 + matrix1.m_33 * m_32 + matrix1.m_34 * m_42;
        result.m_33 = matrix1.m_31 * m_13 + matrix1.m_32 * m_23 + matrix1.m_33 * m_33 + matrix1.m_34 * m_43;
        result.m_34 = matrix1.m_31 * m_14 + matrix1.m_32 * m_24 + matrix1.m_33 * m_34 + matrix1.m_34 * m_44;

        result.m_41 = matrix1.m_41 * m_11 + matrix1.m_42 * m_21 + matrix1.m_43 * m_31 + matrix1.m_44 * m_41;
        result.m_42 = matrix1.m_41 * m_12 + matrix1.m_42 * m_22 + matrix1.m_43 * m_32 + matrix1.m_44 * m_42;
        result.m_43 = matrix1.m_41 * m_13 + matrix1.m_42 * m_23 + matrix1.m_43 * m_33 + matrix1.m_44 * m_43;
        result.m_44 = matrix1.m_41 * m_14 + matrix1.m_42 * m_24 + matrix1.m_43 * m_34 + matrix1.m_44 * m_44;

        return result;
    }

};