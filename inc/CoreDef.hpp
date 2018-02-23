

#ifndef XIANGQI_COREDEF_HPP
#define XIANGQI_COREDEF_HPP

// STL headers
#include <algorithm>
#include <array>
#include <cassert>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <memory>
#include <iomanip>
#include <random>

#define _USE_MATH_DEFINES

#include <cmath>

using namespace std;

// Eigen
#include <Dense>
#include <StdVector>
#include <StdList>
#include <StdDeque>



// type definition
using Integer = int32_t;
using Byte = unsigned char;

// tells CImg to use the png extension
#define cimg_use_png
#include "CImg.h"

using CImgType = cimg_library::CImg<Byte>;


//region Eigen related definitions

// this should not be change for the compatibility with OpenGL
constexpr const auto _EigenMatrixStorageOrder = Eigen::ColMajor;

template<typename DataType, int Row>
using StaticEigenVector = Eigen::Matrix<DataType, Row, 1, _EigenMatrixStorageOrder>;

template<typename DataType, int Rank>
using StaticEigenSquareMatrix = Eigen::Matrix<DataType, Rank, Rank, _EigenMatrixStorageOrder>;

template<typename DataType>
using DynamicEigenMatrix = Eigen::Matrix<DataType, -1, -1, _EigenMatrixStorageOrder>;

using Vec2i = StaticEigenVector<Integer, 2>;

using Vec2f = StaticEigenVector<float, 2>;
using Vec3f = StaticEigenVector<float, 3>;
using Vec4f = StaticEigenVector<float, 4>;

using Vec2d = StaticEigenVector<double, 2>;
using Vec3d = StaticEigenVector<double, 3>;
using Vec4d = StaticEigenVector<double, 4>;

using Mat2f = StaticEigenSquareMatrix<float, 2>;
using Mat3f = StaticEigenSquareMatrix<float, 3>;
using Mat4f = StaticEigenSquareMatrix<float, 4>;

using Mat2d = StaticEigenSquareMatrix<double, 2>;
using Mat3d = StaticEigenSquareMatrix<double, 3>;
using Mat4d = StaticEigenSquareMatrix<double, 4>;

using VecXi = DynamicEigenMatrix<Integer>;
using VecXd = DynamicEigenMatrix<double>;
using VecXf = DynamicEigenMatrix<float>;


template<typename DataType, typename Less = std::less<DataType>>
using ASet = set<DataType, Less, Eigen::aligned_allocator<DataType>>;

template<typename DataType>
using AVector = vector<DataType, Eigen::aligned_allocator<DataType>>;

template<typename KeyType, typename ValueType, typename Less = std::less<KeyType>>
using AMap = map<KeyType, ValueType, Less, Eigen::aligned_allocator<std::pair<KeyType, ValueType>>>;

template<typename DataType>
using AList = list<DataType, Eigen::aligned_allocator<DataType>>;

// defines comparison between Vec2i's
namespace std {
    template<>
    struct less<Vec2i> {
        bool operator()(const Vec2i &left, const Vec2i &right) const {
            if (left[0] < right[0]) {
                return true;
            } else if (left[0] == right[0]) {
                return left[1] < right[1];
            } else {
                return false;
            }
        }
    };

    template<>
    struct greater<Vec2i> {
        bool operator()(const Vec2i &left, const Vec2i &right) const {
            if (left[0] > right[0]) {
                return true;
            } else if (left[0] == right[0]) {
                return left[1] > right[1];
            } else {
                return false;
            }
        }
    };
}
//endregion

// a helper function to assert statements regardless of build option
template<typename ErrorType = std::runtime_error>
inline void AlwaysAssert(bool expr, const char *info) {
    if (!expr) {
        cerr << info << "\n";
        throw ErrorType{info};
    }
}



#endif //XIANGQI_COREDEF_HPP
