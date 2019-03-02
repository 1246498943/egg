
#include "NativeArchive.h"

#include <egg/Path.h>
#include <egg/Platform.h>


using namespace egg;


NativeEntry::NativeEntry()
{
}

NativeEntry::~NativeEntry()
{
}

std::string NativeEntry::getName()
{
    egg::Path p(path.c_str());
    return p.getLastName();
}

bool NativeEntry::isFile()
{
    egg::Path p(dir.c_str());
    p.push(path.c_str());
    if (Platform::getInstance()->isExist(p.getNativePath().c_str()) == 1) return true;
    return false;
}

bool NativeEntry::isDir()
{
    egg::Path p(dir.c_str());
    p.push(path.c_str());
    if (Platform::getInstance()->isExist(p.getNativePath().c_str()) == 2) return true;
    return false;
}

unsigned int NativeEntry::getLen()
{
    egg::Path p(dir.c_str());
    p.push(path.c_str());
    return Platform::getInstance()->Length(p.getNativePath().c_str());
}

void NativeEntry::read(char* data)
{
    egg::Path p(dir.c_str());
    p.push(path.c_str());

    //���ļ� ���ļ����󴫵ݸ�entry
    std::ifstream ifs(p.getNativePath(), std::ios::binary | std::ios::in);
    if (ifs.good())
    {
        ifs.read(data, getLen());
    }
    ifs.close();
}


NativeArchive::NativeArchive(egg::niubi_entry_arg& arg)
    :Archive(arg)
    , dir("")
{
}

NativeArchive::~NativeArchive()
{
}

//std::string NativeArchive::getExt()
//{
//    return "";
//}

void NativeArchive::open(const std::string path, int mode)
{
    //��֤·���Ƿ����
    if (Platform::getInstance()->isExist(path.c_str()) != 2)
    {
        //�������Ŀ¼���Ǵ���
        if(mode == 0)return;

        //����·��
        Platform::getInstance()->createDirectory( path.c_str() );
    }
    dir = egg::Path(path).getNativePath();
}

void NativeArchive::close()
{
    dir = "";
}

bool NativeArchive::isOpen()
{
    return !dir.empty();
}

Entry* NativeArchive::getEntry(const std::string path)
{
    //��֤·���Ƿ����
    egg::Path p = getAbsPath( path );
    int state = Platform::getInstance()->isExist(p.getNativePath().c_str());
    if (state == 0)
    {
        return 0;
    }

    //�������
    NativeEntry* ent = new NativeEntry();

    //��ѯ�ļ�״̬������
    ent->dir = dir; //����·��
    ent->path = path;    //��·��

    return ent;
}

void NativeArchive::setEntryAsFile(const std::string path, const char* data, unsigned int len, bool _override)
{
    //��ѯ�ļ��Ƿ���� ��������� �ʹ��� ������ھ͸���

    egg::Path p2 = path;
    if( p2.size() > 1 )//���·�����ȳ���1 ��˵������Ŀ¼����
    {
        p2.pop(); //�õ�Ŀ¼

        //Ŀ¼�����ڴ���֮
        Platform::getInstance()->createDirectory(getAbsPath( p2.getPathMel() ).getNativePath().c_str());
    }

    //����·��
    egg::Path p = getAbsPath( path );

    //�ļ�������
    if (Platform::getInstance()->isExist(p.getNativePath().c_str()) == 0
        || _override )
    {
        //���ļ� ���ļ����󴫵ݸ�entry
        std::ofstream ofs;
        ofs.open(p.getNativePath(), std::ofstream::out|std::ios::binary);
        if (ofs.good())
        {
            ofs.write(data, len);
        }
        ofs.flush(); //����ǿ����ɻ����� ��Ȼ����������ļ������
        ofs.close();
    }
}

bool NativeArchive::newEntryAsDir(const std::string path)
{
    //���Խ���һ��Ŀ¼
    egg::Path p = getAbsPath( path );

    //Ŀ¼�Ѿ����� ֱ�ӷ���
    if (Platform::getInstance()->isExist(p.getNativePath().c_str()) == 2) return true;

    //Ŀ¼�����ڴ���֮
    Platform::getInstance()->createDirectory(p.getNativePath().c_str());

    return true;
}

void NativeArchive::delEntry(const std::string path)
{
    //ɾ��Ŀ¼�����ļ�����
    egg::Path p = getAbsPath( path );
    Platform::getInstance()->removeFile(p.getNativePath().c_str());
}


std::vector<std::string> NativeArchive::listFile(const char* path, bool ergodic)
{
    //�����·���Ǹ�����·����ת�������·��
    egg::Path rp(path);
    rp.setAbsolutePath(false);
    std::string str = rp.getPathMel();
    //if (rp.isAbsolutePath())
    //{
    //    str = "." + rp.getPathMel();
    //}
    //else
    //{
    //    str = rp.getPathMel();
    //}

    std::vector<std::string> l = Platform::getInstance()->listFile(dir.c_str(), str.c_str(), ergodic);
    std::vector<std::string> ret;

    //�ѱ���·��ת������·����ʽ
    for (unsigned int i = 0; i < l.size(); i++)
    {
        egg::Path p(l[i].c_str());
        ret.push_back(p.getPathMel(false, true));
    }

    return ret;
}


//����ھ���·��  �õ�����ϵͳ�ľ���·��
egg::Path NativeArchive::getAbsPath( const std::string& path )
{
    //����·��
    egg::Path p( dir ); //��ǰĿ¼
    egg::Path p2(path);         //����·��
    p2.setAbsolutePath(false);     //ǿ��ת�������·�� Ϊ������·���ϲ�����ȷ
    p.push( p2 );

    return p;
}
