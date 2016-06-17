//
// This file is part of the stargazer library.
//
// Copyright 2016 Claudio Bandera <claudio.bandera@kit.edu (Karlsruhe Institute of Technology)
//
// The stargazer library is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The stargazer library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once


// Ceres includes
#include "../CoordinateTransformations.h"
#include "../StargazerTypes.h"
#include "ceres/ceres.h"

namespace stargazer {

struct LM2ImgReprojectionFunctor {

    double u_observed, v_observed;
    double x_marker, y_marker;

    LM2ImgReprojectionFunctor(double u_observed, double v_observed, double x_marker, double y_marker)
            : u_observed(u_observed), v_observed(v_observed), x_marker(x_marker), y_marker(y_marker) {
    }

    template <typename T>
    bool operator()(const T* const lm_pose, const T* const camera_pose, const T* const camera_intrinsics,
                    T* residuals) const {

        // Transform landmark point to camera
        T u_marker = T(0.0);
        T v_marker = T(0.0);

        T x_marker_tmp = T(x_marker);
        T y_marker_tmp = T(y_marker);

        transformLM2Img<T>(&x_marker_tmp, &y_marker_tmp, lm_pose, camera_pose, camera_intrinsics, &u_marker, &v_marker);

        // Compute residual
        residuals[0] = u_marker - T(u_observed);
        residuals[1] = v_marker - T(v_observed);

        return true;
    }

    // Factory to hide the construction of the CostFunction object from
    // the client code.
    static ceres::CostFunction* Create(const double u_observed, const double v_observed, const double x_marker,
                                       const double y_marker) {
        return (new ceres::AutoDiffCostFunction<LM2ImgReprojectionFunctor, 2, 6, 6, 6>(
            new LM2ImgReprojectionFunctor(u_observed, v_observed, x_marker, y_marker)));
    }
};

struct World2ImgReprojectionFunctor {

    double u_observed, v_observed;
    double x_marker, y_marker, z_marker;

    World2ImgReprojectionFunctor(double u_observed, double v_observed, double x_marker, double y_marker,
                                 double z_marker)
            : u_observed(u_observed), v_observed(v_observed), x_marker(x_marker), y_marker(y_marker),
              z_marker(z_marker) {
    }

    template <typename T>
    bool operator()(const T* const camera_pose, const T* const camera_intrinsics, T* residuals) const {

        // Transform landmark point to camera
        T u_marker = T(0.0);
        T v_marker = T(0.0);

        T x_marker_tmp = T(x_marker);
        T y_marker_tmp = T(y_marker);
        T z_marker_tmp = T(z_marker);
        transformWorld2Img<T>(&x_marker_tmp, &y_marker_tmp, &z_marker_tmp, camera_pose, camera_intrinsics, &u_marker,
                              &v_marker);

        // Compute residual
        residuals[0] = u_marker - T(u_observed);
        residuals[1] = v_marker - T(v_observed);

        return true;
    }

    // Factory to hide the construction of the CostFunction object from
    // the client code.
    static ceres::CostFunction* Create(const double u_observed, const double v_observed, const double x_marker,
                                       const double y_marker, const double z_marker) {
        return (new ceres::AutoDiffCostFunction<World2ImgReprojectionFunctor, 2, (int)POSE::N_PARAMS, (int)INTRINSICS::N_PARAMS>(
            new World2ImgReprojectionFunctor(u_observed, v_observed, x_marker, y_marker, z_marker)));
    }
};

} // namespace stargazer