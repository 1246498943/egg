#ifndef NIUBI_SG_RESOURCEMANAGER_H
#define NIUBI_SG_RESOURCEMANAGER_H

#include <eggEngine/Export.h>

#include <map>

//#include <eggBase/Public.h>
#include <egg/Log.h>
#include <egg/SystemArg.h>

#include <eggEngine/Resource.h>
#include <eggEngine/VirtualFileSystem.h>


namespace egg
{

    /*
    * ��Դ������
    *
    * ��Դ���ͣ�
    *       mesh(skeleton bindpose weight) geometry(subgeometry)
    *       image texture
    *       shader program material
    *       scenenode
    */

    /*
    * �ڴ���Դ������
    *   1 �����Դ�ڴ����
    *   2 ��Ϊ��Դ����ʹ��
    */
#define NB_RESOURCEGROUP_DEFAULT_NAME "default"
    class EGG_ENGINE_EXPORT ResourceGroup
    {
    public:
        ResourceGroup(Resource* resource = 0);

        void add(Resource* resource = 0);

        //�������п��ܴ��ڵ���Դ
        void searchAll();

        void cleanupLink();

        void clear();

        std::map< egg::Path, egg::ref_ptr< Resource > > mResourceObjectMap;
        std::vector< egg::ref_ptr< ResourceLink > > mResourceLinkList;
    };

    /*
        resource manager
        �����ļ����ڴ�����ת������
    */
    class EGG_ENGINE_EXPORT ResourceManager
        :public egg::Referenced
    {
    public:

        ResourceManager();
        virtual ~ResourceManager();

        //manager(ResourceManager)
        DECLARE_SINGLETON(ResourceManager)

        void init(egg::niubi_entry_arg& arg, bool binary);

        //ͳһasset�ļ�·�� ֧�ָ��ǹ�ϵ
        class UniformAssetFile
        {
        public:
            egg::Path uniform_path;   //��·��
            unsigned char type;         //�ļ����� 0�ļ� 1Ŀ¼
            unsigned int archive_index; //����һ������index
            unsigned int repeat_count;  //��������
        };

        //������
        void registerArchive(const std::string& type, const std::string& path, bool write);
        void unRegisterArchive(const std::string& path);
        void updateUniformAssetPath();
        std::map< egg::Path, UniformAssetFile > getUniformAssetFileMap() { return mUniformAssetFileMap; }


        //���ڶ�д�ļ�
        //    pathΪ���ھ���·������
        void writeFile(const egg::Path& path, const char* data, unsigned int len);
        void readFile(const egg::Path& path, std::vector<char>& data, unsigned int& len);
        void setWriteArchiveIndex(unsigned int index) { mWriteArchiveIndex = index; }
        unsigned int getWriteArchiveIndex() { return mWriteArchiveIndex; }


        //��д������Դ ��������������
        Resource* readResource(const egg::Path& uniform_path, bool load_ext, const std::string& resource_group_name = NB_RESOURCEGROUP_DEFAULT_NAME);
        Resource* readResource(const egg::Path& uniform_path, bool load_ext, ResourceGroup* resource_group);
        bool writeResource(Resource* resource /*, const std::string& uniform_path*/);


        //��д��Դ�� ����������������
        //ResourceGroup* readResourceGroup(const std::string& uniform_path);
        Resource* readResourceGroup(const egg::Path& uniform_path, ResourceGroup* result, bool load_ext);
        bool writeResourceGroup(ResourceGroup* resource_group);


        //��ѯ��Դ��
        ResourceGroup* getResourceGroup(const std::string& name = NB_RESOURCEGROUP_DEFAULT_NAME);

        //��ȡ���������
        //ObjectManager* getObjectManager() { return mObjectManager.get(); }

    private:

        //�ⲿ�洢������Դ����
        //egg::FileSystem mFileSystem;
        unsigned int mWriteArchiveIndex;
        std::map< egg::Path, UniformAssetFile > mUniformAssetFileMap;

        //�ڴ����
        std::map< std::string, ResourceGroup > mResourceGroupMap;

        //�������л�
        //egg::ref_ptr< ObjectManager > mObjectManager;
        bool use_binary;
        egg::Serializer* ser;

    };

}//namespace egg

#endif//NIUBI_SG_RESOURCEMANAGER_H
