#ifndef _FBX_UTILS_EXPORT_H
#define _FBX_UTILS_EXPORT_H

#include <vector>
#include <fstream>
//#include <vmmlib/matrix.hpp>
#include <Eigen/Eigen>
#include <fbxsdk.h>

std::vector<char> readfile(std::string filename);


//�ϲ���ǰnode��local����
Eigen::Matrix4f getLocalMatrix(FbxNode* node);

Eigen::Matrix4f getWorldMatrix(FbxNode* node);

bool hasSkin(FbxMesh* pMesh);

void getCustomNodeProperty(FbxNode* pNode);

//������������joint��bindposeλ��
void getPose(FbxScene* fbx_scene);


#endif//_FBX_UTILS_EXPORT_H

