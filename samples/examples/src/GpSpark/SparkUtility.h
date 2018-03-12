#pragma once

#include "Vector3.h"
#include "Vector4.h"

namespace SPK {

class Vector3D;
class Color;

namespace GP3D {


    ////////////////////////////
    /// Conversion functions ///
    ////////////////////////////

    /**
    * @brief Converts a SPARK Vector3D to a GPlay3D Vector3
    * @param v : the Vector3D to convert
    * @return the GPlay3D Vector3
    */
    inline gameplay::Vector3 spk2gp3d(const Vector3D& v);

    /**
    * @brief Converts a GPlay3D Vector3 to a SPARK Vector3D
    * @param v : the Vector3 to convert
    * @return the SPARK Vector3D
    */
    inline SPK::Vector3D gp3d2spk(const gameplay::Vector3& v);

}}
