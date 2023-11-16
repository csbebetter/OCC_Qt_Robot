#include "R_quaternion.h"

Eigen::Matrix3d quaternion2R(Eigen::Quaterniond& quat)
{
    Eigen::Matrix3d R;//声明一个Eigen类的3*3的旋转矩阵
    //四元数转为旋转矩阵--先归一化再转为旋转矩阵
    R = quat.normalized().toRotationMatrix();
    return R;
}

Eigen::Quaterniond R2quaternion(Eigen::Matrix3d& R)
{
    Eigen::Quaterniond quat;
    quat = Eigen::Quaterniond(R);//旋转矩阵转为四元数
    quat.normalize();//转为四元数之后，需要进行归一化
    return quat;
}
