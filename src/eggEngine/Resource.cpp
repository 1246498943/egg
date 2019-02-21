
#include <eggEngine/Resource.h>

using namespace egg;


void Resource::save(egg::Path filename , std::ostream& os , Serializer* ser ) const
{
    Object::save(filename,os,ser);

    //��Դ·��ֻ������Դ�� û�к�׺
    std::string name = uniform_path.getLastName();
    ser->writeString( os , name );
}

void Resource::load(egg::Path filename , std::istream& is , Serializer* ser 
    , ObjectManager* om, egg::Referenced* rmg, bool load_ext)
{
    Object::load(filename,is,ser,om,rmg,load_ext);

    //��ǰ�ļ�Ŀ¼+��¼����=��Դ·�� û�к�׺
    std::string name;
    ser->readString( is , name );
    uniform_path.set(filename.getPathMel(true,true).c_str()); //��ǰ�ļ�Ŀ¼
    uniform_path.push(name.c_str());
}

//����ͳһ·�� �����β����class��׺�Ͳ�����ȥ
void Resource::setUniformPath(const std::string& path)
{
    setUniformPath(egg::Path(path));
}

void Resource::setUniformPath(egg::Path path)
{
    //����·��
    uniform_path = path;

    //���ĩβ�Ƿ�����ȷ�ĺ�׺
    std::vector< std::string > sl = path.splitLastName();
    std::string lastname = path.getLastName();
    if (sl.size() <= 1)  //�޺�׺ �����ļ����Ƹ���׺��ͬ�����
    {
        uniform_path.pop();
        uniform_path.push((lastname + "." + this->ObjectClassName()).c_str());
    }
    else //�ļ������к�׺
    {
        if ((*sl.rbegin()) != this->ObjectClassName()) //ĩβ�ַ�����class��׺
        {
            uniform_path.pop();
            uniform_path.push((lastname + "." + this->ObjectClassName()).c_str());
        }
    }
}