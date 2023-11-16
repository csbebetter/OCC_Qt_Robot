#pragma once
#include <Eigen/Dense>
#include <cmath>


Eigen::Matrix3d quaternion2R(Eigen::Quaterniond& quat);
Eigen::Quaterniond R2quaternion(Eigen::Matrix3d& R);
