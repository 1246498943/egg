
#include <eggEngine/VirtualFileSystem.h>

TEST(eggEngine, VirtualFileSystem)
{


}

//��ֲ�����Ĳ���
//���Խ�������
void test_fs_native(const char* archive_filename, const char* archive_ext)
{
    //�ļ�ϵͳ
    egg::FileSystem fs;

    //ע��һ����
    egg::Archive* a = fs.newArchive(archive_ext);

    //�򿪰�
    a->open(archive_filename);
    if (a->isOpen())
    {
        printf("archive is open\n");
    }
    else
    {
        printf("archive is close\n");
    }


    //�½�Ŀ¼
    a->newEntryAsDir("ddr");
    a->newEntryAsDir("ddr2");
    a->newEntryAsDir("a|b|c|d|e");

    //�½��ļ�
    a->setEntryAsFile("qq.txt", "1122334455", 10, true);
    a->setEntryAsFile("qq2.txt", "1122334455", 10, true);
    a->setEntryAsFile("a|b|c|d|e|qq3.txt", "1122334455", 10, true);

    //ɾ��Ŀ¼
    //a->delEntry("ddr");
    a->delEntry("ddr2");

    //ɾ���ļ�
    //a->delEntry("qq.txt");
    a->delEntry("qq2.txt");


    //�򿪶���
    egg::Entry* ent = a->getEntry("aa.txt");
    if (ent)
    {
        //���ļ���Ϣ
        if (ent->isFile())
        {
            printf("is file\n");
        }
        if (ent->isDir())
        {
            printf("is dir\n");
        }
        printf("name %s\n", ent->getName().c_str());
        printf("len  %d\n", ent->getLen());

        //���ļ�����
        std::string str;
        str.resize(ent->getLen() + 1);
        str[ent->getLen()] = '\0';
        ent->read(&str[0]);
        printf("%s\n", str.c_str());
    }
    else
    {
        printf("file not exist\n");
    }



    //�ر�
    a->close();

}

//��ֲ�����Ĳ���
//���Բ�ѯ����
void test_fs_native2(const char* archive_filename, const char* archive_ext)
{
    //�ļ�ϵͳ
    egg::FileSystem fs;

    //ע��һ����
    egg::Archive* a = fs.newArchive(archive_ext);

    //�򿪰�
    a->open(archive_filename);
    if (a->isOpen())
    {
        printf("archive is open\n");
    }
    else
    {
        printf("archive is close\n");
    }

    //��ӡ�ļ��б�
    std::vector<std::string> l = a->listFile("ddr", true);
    for (unsigned int i = 0; i < l.size(); i++)
    {
        printf("%s\n", l[i].c_str());
    }

}
