#ifndef NIUBI_FS_FILESYSTEM_EXPORT_H
#define NIUBI_FS_FILESYSTEM_EXPORT_H

//#include <nbBase/Public.h>
#include <eggEngine/Export.h>

#include <egg/Log.h>
#include <egg/SystemArg.h>
#include <egg/Path.h>
#include <egg/ref_ptr.h>
#include <egg/Singleton.h>

#include <string>
#include <vector>


/*
*
*����·��
*    ���Ǳ��ز���ϵͳ��·����ʽ windows c:\aaaa\bbb  linux /usr/local
*��·��
*    ����mel����ʽ  |aaa|bbb
*
*���뷽ʽ
*    Ŀǰʹ�ñ���ANSI������ΪĬ��֧�� ��string�ķ���ȷ������˵
*
*/

namespace egg
{
    ///�ļ�����
    ///
    ///���е�һ���ļ�����Ŀ¼
    class EGG_ENGINE_EXPORT Entry
        :public Referenced
    {
    public:
        Entry(){}
        virtual ~Entry(){}

        virtual std::string getName() = 0;
        virtual bool isFile() = 0;
        virtual bool isDir() = 0;
        virtual unsigned int getLen() = 0;
        virtual void read(char* data) = 0;
    };

    ///������
    ///
    ///����:����Ŀ¼,zip��,sqlite+QuickLZ
    class EGG_ENGINE_EXPORT Archive
        :public Referenced
    {
    public:
        Archive(egg::niubi_entry_arg& arg)
            :mArg(arg){}
        virtual ~Archive(){}

        egg::niubi_entry_arg mArg;//ϵͳ��ڲ�������

        //virtual std::string getExt() = 0; //֧�ֵĺ�׺����

        //�򿪰�
        //   path �����ļ�·��        asset����ʱ�˲�����Ч
        //   mode 0ֻ�� 1���� 2�༭   asset����ʱ�˲���ֻ��Ϊֻ��
        virtual void open(const std::string path, int mode = 0) = 0;

        //�رհ�
        virtual void close() = 0;

        //��ѯ�Ƿ��
        virtual bool isOpen() = 0;


        //��ȡ����
        //  path ��·��
        virtual Entry* getEntry(const std::string path) = 0;

        //���Ƕ���
        //  path ��·��
        //  data д������
        //  len д���ֽڳ���
        virtual void setEntryAsFile(const std::string path, const char* data, unsigned int len , bool _override) = 0;

        //�½�Ŀ¼
        //  path ��·��
        virtual bool newEntryAsDir(const std::string path) = 0;

        //ɾ������
        //  path ��·��
        virtual void delEntry(const std::string path) = 0;

        //�оٰ���·��
        //  path ��·��
        //  ergodic �ݹ���Ŀ¼
        virtual std::vector<std::string> listFile(const char* path, bool ergodic = false) = 0;

    };

    ///�ļ�ϵͳ
    ///
    ///  ʹ��ͳһ·������������Ӳ���ϵ��ļ�\n
    ///  ֧�ֶ������ͬ�����ǲ���\n
    ///  ֧�ֶԱȰ����������������°�\n
    ///  �Ժ���Կ�����������Զ���ļ�\n
    class EGG_ENGINE_EXPORT FileSystem
        :public Referenced
    {
    public:
        FileSystem(){}
        ~FileSystem(){}

        DECLARE_SINGLETON(FileSystem)

        void setArg(egg::niubi_entry_arg& arg){ mArg = arg; }

        //������ ����·����׺�ж�
        //  ����Ϊ"asset"ʱ��PCģʽ��ֻ�ܷ��ر���·�� ���Ϊandroid�õ�assetmanager·�� ���ΪIOS�õ�application·��
        Archive* newArchive(const std::string ext);

        //��ȡ��
        Archive* getArchive(unsigned int i);

        //��ǰ������
        unsigned int numArchive();

        //ɾ����
        void delArchive(Archive* archive);

    private:

        std::vector< ref_ptr<Archive> > mArchiveList;

        egg::niubi_entry_arg mArg;//ϵͳ��ڲ�������
    };

}//namespace nbFS

#endif//NIUBI_FS_FILESYSTEM_EXPORT_H
