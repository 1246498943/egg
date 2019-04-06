
#include <eggEngine/Level.h>

using namespace egg;

void Level::save(Path filename, std::ostream& os, Serializer* ser) const
{
    Resource::save(filename, os, ser);

}

void Level::load(Path filename, std::istream& is, Serializer* ser
    , ObjectManager* om, Referenced* rmg, bool load_ext)
{
    Resource::load(filename, is, ser, om, rmg, load_ext);

}



Entity* Level::createEntity(const std::string& type_name, unsigned int id)
{
    //���Ϊ0 ���Զ�����һ��ID
    if (id == 0)
    {
        mLastID++;
        id = mLastID;
    }

    //�ж�������id�Ѿ����ھͲ����ظ�������
    Entity* ent = getEntity(id);
    if (ent)
    {
        std::cout << "entity id is exsit" << std::endl;
        return 0;
    }

    //���id���ڼ�¼���ֵ �ͼ�¼Ϊ���ֵ
    if (id > 0 && id > mLastID)
    {
        mLastID = id;
    }

    //����entity����
    Entity* ent_new = EntityFactory::getInstance()->createEntity(type_name);
    if (ent_new)
    {
        //��¼���б���
        ent_new->setUID(id);
        mEntityList[id] = ent_new;
        ent_new->onCreate();
        return ent_new;
    }
    else
    {
        std::cout << "entity factory is not exsit" << std::endl;
    }

    return 0;
}

void Level::destroyEntity(unsigned int id)
{
    auto it = mEntityList.find(id);
    if (it != mEntityList.end())
    {
        it->second->onDestroy();
        mEntityList.erase(it);
    }
}

void Level::spawnEntity(unsigned int id
    , Eigen::Vector3f pos, Eigen::Vector3f rot, Eigen::Vector3f scale)
{
    auto it = mEntityList.find(id);
    if (it != mEntityList.end())
    {
        it->second->onSpawn(mRoot, pos, rot, scale);
    }
}

void Level::unspawnEntity(unsigned int id)
{
    auto it = mEntityList.find(id);
    if (it != mEntityList.end())
    {
        it->second->onUnSpawn(mRoot);
    }
}

Entity* Level::getEntity(unsigned int id)
{
    auto it = mEntityList.find(id);
    if (it != mEntityList.end())
    {
        return it->second.get();
    }
    return 0;
}

