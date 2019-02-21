
#ifndef NIUBI_BASE_FILE_DIR_UTIL_H
#define NIUBI_BASE_FILE_DIR_UTIL_H

#include <egg/Export.h>

//#include <direct.h>  
#include <string>
#include <vector>

#include <iostream>  
#include <stdlib.h>  
#include <stdio.h>  
//#include <string.h>  
#ifdef linux  
#include <unistd.h>  
#include <dirent.h>  
#endif  
#ifdef WIN32  
#include <direct.h>  
#include <io.h>  
#endif  

#if 0
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
using namespace boost::filesystem;
#endif

#include <egg/Path.h>
#include <egg/Codes.h>


//��ѯ��ǰĿ¼
EGG_RUNTIME_EXPORT std::string getCurrentDirectory();

//�����༶�˵� ���������Ŀ¼ ./ ../��ͷ
EGG_RUNTIME_EXPORT int mkdir_r(const std::string& path);

//�ݹ�Ŀ¼�����ļ��б� UTF-8
EGG_RUNTIME_EXPORT std::vector<std::string> getFilesInDir(std::string cate_dir);


//�ļ��Ƿ����
EGG_RUNTIME_EXPORT bool isFileExists(const std::string& filename);

//Ŀ¼�Ƿ����
EGG_RUNTIME_EXPORT bool isDirExists(const std::string& dir);

#endif //NIUBI_BASE_FILE_DIR_UTIL_H
