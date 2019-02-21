#ifndef NIUBI_SG_SCENE_H
#define NIUBI_SG_SCENE_H

#include <eggRuntime/ref_ptr.h>
#include <eggEngine/SceneNode.h>
#include <eggEngine/Resource.h>

namespace egg
{

    /*
    * ������Դ
    *    scene�������root�ڵ� ����Ҳ��resource
    */

    class EGG_ENGINE_EXPORT Scene
        :public Resource
        //,public SceneNode
    {
    public:
        Scene() { mRoot = new SceneNode(); }
        virtual ~Scene() {}

        //virtual std::string getExt(){return "scene";}
        virtual std::string ObjectClassName() override { return "scene"; }

        virtual void save(Path filename, std::ostream& os, Serializer* ser) const override
        {
            Resource::save(filename, os, ser);
            mRoot->save(filename, os, ser);
        }

        virtual void load(Path filename, std::istream& is, Serializer* ser, ObjectManager* om, Referenced* rmg, bool load_ext) override
        {
            Resource::load(filename, is, ser, om, rmg, load_ext);
            mRoot->load(filename, is, ser, om, rmg, load_ext);
        }

        //�ռ��Ѿ����ص���Դ����
        virtual void collectionResourceObject(std::map< Path, ref_ptr< Resource > >& result) override
        {
            result[uniform_path] = this;

            ref_ptr< nv_collectionResourceObject > nv = new nv_collectionResourceObject(result);
            mRoot->accept(*nv);
        }

        //�ռ���δ���ص���Դ·��
        virtual void collectionResourceLink(std::vector< ref_ptr< ResourceLink > >& result) override
        {
            //std::vector< std::string > r;
            ref_ptr< nv_collectionResourceLink > nv = new nv_collectionResourceLink(result);
            mRoot->accept(*nv);
        }


        SceneNode* getRoot() { return mRoot.get(); }

    private:

        ref_ptr< SceneNode > mRoot;
    };

}//namespace egg

#endif//NIUBI_SG_SCENE_H
