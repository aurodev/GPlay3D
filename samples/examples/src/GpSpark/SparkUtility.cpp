#include "SparkUtility.h"
#include "SparkQuadRenderer.h"
#include <spark/Core/IO/SPK_IO_Manager.h>

namespace SPK {
namespace GP3D {

    ////////////////////////////
    /// Conversion functions ///
    ////////////////////////////

    inline gameplay::Vector3 spk2gp3d(const Vector3D& v)
    {
        return gameplay::Vector3(v.x, v.y, v.z);
    }

    inline SPK::Vector3D gp3d2spk(const gameplay::Vector3& v)
    {
        return Vector3D(v.x, v.y, v.z);
    }

}}
