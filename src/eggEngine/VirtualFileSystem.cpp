

#include <eggEngine/VirtualFileSystem.h>

#include "VirtualFileSystem/NativeArchive.h"
#include "VirtualFileSystem/ZipArchive.h"
// #include <nbFS/SZPArchive.h>

#if ANDROID
#include "VirtualFileSystem/AndroidAssetArchive.h"
#endif

using namespace egg;

IMPLEMENT_SINGLETON(FileSystem)

Archive* FileSystem::newArchive(const std::string ext)
{
    Archive* a = 0;

    //�ƶ��豸APPĿ¼
    //  android����asset managerʵ��
    //  IOS����applicationĿ¼
    //  ��PC�豸��ֱ����Ч
    if (ext == "asset")
    {
#if ANDROID
        a = new AndroidAssetArchive(mArg);
#endif
#if IOS
#endif
    }


    //��Դ��
    if (ext == "zip")
    {
        //zip
        a = new ZipArchive(mArg);
    }
    if (ext == "szp")
    {
        // szp
        // a = new SZPArchive();
    }

    //Ĭ�ϴ򿪱���·��
    if (a == 0)
    {
        //native
        a = new NativeArchive(mArg);
    }


    if (a)
    {
        mArchiveList.push_back(a);
    }
    return a;
}

Archive* FileSystem::getArchive(unsigned int i)
{
    if (i < mArchiveList.size())
    {
        return mArchiveList[i].get();
    }
    return 0;
}

unsigned int FileSystem::numArchive()
{
    return mArchiveList.size();
}

void FileSystem::delArchive(Archive* archive)
{
    for (unsigned int i = 0; i < mArchiveList.size(); i++)
    {
        if (mArchiveList[i].get() == archive)
        {
            mArchiveList.erase(mArchiveList.begin() + i);
            break;
        }
    }
}

