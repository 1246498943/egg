
#include <thread>
#include <chrono>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include <Eigen/Eigen>
#include <eggEngine/Matrix.h>

int main(int argc, char* argv[])
{
    //

    Eigen::Vector3f v0; //�����ǲ����ʼ��xyz��
    Eigen::Vector3f v1(1,2,3);
    Eigen::Vector3f v2;
    v2 << 3 , 2 , 1;


    //

    Eigen::Matrix4f m0; //�����ǲ����ʼ����

    Eigen::Matrix4f m1;
    m1.setIdentity();

    //���ָ�ֵ��ʽ�ǰ���������ʽ�����Ǵ洢��������ġ�
    Eigen::Matrix4f m2;
    m2 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16;

    std::cout << m2(0, 0) << " ";
    std::cout << m2(0, 1) << " ";
    std::cout << m2(0, 2) << " ";
    std::cout << m2(0, 3) << " ";

    //������ӿڿ�������������������У������ڴ������С���֪��Ϊʲô��


    //
    Eigen::Matrix4f m33;
    m33.setIdentity();

    m33(0, 0) = 0;
    m33(0, 1) = 1;
    m33(0, 2) = 2;
    m33(0, 3) = 3;
    m33(1, 0) = 4;
    m33(1, 1) = 5;
    m33(1, 2) = 6;
    m33(1, 3) = 7;
    m33(2, 0) = 8;
    m33(2, 1) = 9;
    m33(2, 2) = 10;
    m33(2, 3) = 11;
    m33(3, 0) = 12;
    m33(3, 1) = 13;
    m33(3, 2) = 14;
    m33(3, 3) = 15;

    //���������������
    std::cout << "\n";
    std::cout << m33;
    std::cout << "\n";

    //������洢����
    float* d = m33.data();
    for (size_t i = 0; i < 16; i++)
    {
        std::cout << d[i] << " ";
    }



    Eigen::Vector4f v8(1, 0, 0, 1);
    //Eigen::Vector4f v9 = v8 * m1; //�����ֱ�ӱ������
    Eigen::Vector4f v10 = m1 * m1 * v8;

    //�ӽ���������˳���� m * v


    //

    Eigen::AngleAxisf aa0;
    Eigen::AngleAxisf aa1(1.0,Eigen::Vector3f(1,2,3));

    Eigen::Quaternionf qq0;
    Eigen::Quaternionf qq1(1,2,3,4); //ע��˳���� w xyz


    //����ת��
    aa0 = qq1;
    qq0 = aa1;

    Eigen::Matrix3f m3; //��ת��Ϣֻ�ܸ�ֵ��һ��3x3����
    m3 = qq0;
    m3 = aa1;

    qq0 = m3;
    aa1 = m3;


    //

    Eigen::Affine3f af;
    af.setIdentity();
    af.translate(Eigen::Vector3f(1, 1, 1));
    af.rotate(Eigen::AngleAxisf(90.0 / 180.0*M_PI, Eigen::Vector3f(0, 0, 1)));
    af.scale(0.5);
 
    //���������ӿڶ���ǰ��
    //���������˳���� TRS

    Eigen::Matrix4f m_tx;
    m_tx = af.matrix();

    //�����ӡ��˳�������������ʽ
    std::cout << "m_tx\n";
    std::cout << m_tx;
    std::cout << "\n";

    //������ӡ����������Ľ��
    float* dd = m_tx.data();
    for (size_t i = 0; i < 16; i++)
    {
        std::cout << dd[i] << " ";
    }



    //ֱ��ʹ��matrix����ռ�任
    Eigen::Matrix4f m_tx2 =
        Eigen::translate(1.0f, 1.0f, 1.0f)
        * Eigen::rotate(float(90.0 / 180.0*M_PI), 0.0f, 0.0f, 1.0f)
        * Eigen::scale(0.5f, 0.5f, 0.5f)
        ;

    std::cout << "m_tx2\n";
    std::cout << m_tx2;
    std::cout << "\n";
    
    //�����Ǽ�������ǴӺ���ǰ��˳�� Ҳ���� M2 * M1 * M0 * V


    //ת������
    Eigen::Affine3f af1(m_tx);
    Eigen::Matrix4f m_tx1(af.matrix());


    //�������߲�ֵ



    //ͶӰ�������



    //lookat����



    //���������洢
    std::vector<Eigen::Vector3f> list;

    list.push_back(Eigen::Vector3f(1, 2, 3));
    list.push_back(Eigen::Vector3f(4, 5, 6));
    list.push_back(Eigen::Vector3f(7, 8, 9));

    float* ddd = list[0].data();
    for (size_t i = 0; i < 9; i++)
    {
        std::cout << ddd[i] << " ";
    }
    std::cout << "\n";

    //
    std::vector<Eigen::Matrix4f> list2;

    Eigen::Matrix4f m_0;
    m_0 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16;
    Eigen::Matrix4f m_1;
    m_1 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16;

    list2.push_back(m_0);
    list2.push_back(m_1);

    float* ddd2 = list2[0].data();
    for (size_t i = 0; i < 32; i++)
    {
        std::cout << ddd2[i] << " ";
    }
    std::cout << "\n";


    return 0;
}

