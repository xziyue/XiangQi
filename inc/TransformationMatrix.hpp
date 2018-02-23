// transformation matrices

#ifndef DEF_TFMATRIX_HPP
#define DEF_TFMATRIX_HPP

#include "CoreDef.hpp"

template<typename T>
inline T RadToDeg(T rad) {
    return rad * (T) 180 / M_PI;
}

template<typename T>
inline T DegToRad(T deg) {
    return deg / (T) 180 * M_PI;
}

template<typename T>
using _3DTfMatrixType = StaticEigenSquareMatrix<T, 4>;


template<typename T>
inline _3DTfMatrixType<T> GetTranslationMatrix(T x, T y, T z) {
    _3DTfMatrixType<T> result = _3DTfMatrixType<T>::Identity();
    result(0, 3) = x;
    result(1, 3) = y;
    result(2, 3) = z;
    return result;
};

template<typename T>
inline StaticEigenSquareMatrix<T, 4> GetScaleMatrix(T x, T y, T z) {
    _3DTfMatrixType<T> result = _3DTfMatrixType<T>::Identity();
    result(0, 0) = x;
    result(1, 1) = y;
    result(2, 2) = z;
    return result;
};

template<typename T>
inline StaticEigenSquareMatrix<T, 4> GetRotationMatrix(T rad, const StaticEigenVector<T, 3> &axis) {
    assert(axis.isUnitary());
    _3DTfMatrixType<T> result = _3DTfMatrixType<T>::Identity();
    StaticEigenSquareMatrix<T, 3> _3dRotationMatrix{Eigen::AngleAxis < T > {rad, axis}.toRotationMatrix()};
    result.block(0, 0, 3, 3) = _3dRotationMatrix.block(0, 0, 3, 3);
    return result;
};

template<typename T>
inline _3DTfMatrixType<T> GetOrthoMatrix(T left, T right, T bottom, T top) {
    auto width = right - left;
    auto height = top - bottom;
    _3DTfMatrixType<T> result = GetTranslationMatrix((T) -1.0, (T) -1.0, (T) 0.0) *
                                GetScaleMatrix((T) 2.0 / width, (T) 2.0 / height, (T) 1.0) *
                                GetTranslationMatrix(-left, -bottom, (T) 0.0);
    return result;
};

#endif