#ifndef _EGG_PLATFORM_H_
#define _EGG_PLATFORM_H_

#include <egg/Export.h>

#include <egg/Singleton.h>

#include <string>
#include <vector>

namespace egg
{
    class EGG_RUNTIME_EXPORT Platform
    {
    public:
        Platform();
        ~Platform();

        //manager(Platform)
        DECLARE_SINGLETON(Platform)

        //Ŀ¼����ȫ���ƶ���system dir��

        //�����༶�ļ�·��
        int createDirectory(const char* path);

        //·���Ƿ����
        //  ����ֵ 0������ 1�ļ� 2Ŀ¼
        int isExist(const char* path);

        //��ѯ�ļ�����
        unsigned int Length(const char* path);

        //ɾ���ļ�������Ŀ¼
        void removeFile(const char* path, bool force_delete_non_enpty_dir = false);

        //�о�Ŀ¼�е�·��
        //  dir �б�����λ��
        //  path dir�ڵ�·��
        //  ergodic �ݹ�������Ŀ¼
        std::vector<std::string> listFile(const char* dir, const char* path, bool ergodic = false);


        //��Щȫ���ƶ���system dir������
        std::string getApplicationDir();
        std::string getApplicationUrl();
        std::string getAppStorageDir();
        std::string getAppStorageUrl();
        std::string getCacheDir();
        std::string getCacheUrl();
        std::string getDesktopDir();
        std::string getDesktopUrl();
        std::string getDocumentsDir();
        std::string getDocumentsUrl();

    private:

    };

} //namespace egg


#endif // !_EGG_PLATFORM_H_
