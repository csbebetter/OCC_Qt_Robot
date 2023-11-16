#include "R_quaternion.h"

Eigen::Matrix3d quaternion2R(Eigen::Quaterniond& quat)
{
    Eigen::Matrix3d R;//����һ��Eigen���3*3����ת����
    //��Ԫ��תΪ��ת����--�ȹ�һ����תΪ��ת����
    R = quat.normalized().toRotationMatrix();
    return R;
}

Eigen::Quaterniond R2quaternion(Eigen::Matrix3d& R)
{
    Eigen::Quaterniond quat;
    quat = Eigen::Quaterniond(R);//��ת����תΪ��Ԫ��
    quat.normalize();//תΪ��Ԫ��֮����Ҫ���й�һ��
    return quat;
}
