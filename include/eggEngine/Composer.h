#ifndef _EGG_COMPOSER_H_
#define _EGG_COMPOSER_H_

#include <eggEngine/Export.h>

#include <eggEngine/Resource.h>
#include <eggEngine/RenderPass.h>
#include <eggEngine/SceneNode.h>


namespace egg
{
    /*
    *  �ϳ�������
    *
    *  Ĭ�Ͻ����ϳ����ǿյģ���ȫû���κ����á�
    *  ��cull�����л��Զ���Ϊ����һ��ǰ����Ⱦ��
    *  SceneNode��Camera����Ĭ�������ָ��RenderWindow��Ĭ�����á�
    *
    *  SceneNode��Ӧ��slot���ø�RenderWindow��һ�¡�
    *  Camera�����ø�RenderWindow��һ�¡�
    *  �������������¼SceneNode��Camera�Ķ�Ӧ���ơ�
    */
    class EGG_ENGINE_EXPORT Composer
        :public Resource
    {
    public:
        Composer();
        virtual ~Composer();


        virtual void save(Path filename, std::ostream& os, Serializer* ser) const override;
        virtual void load(Path filename, std::istream& is, Serializer* ser
            , ObjectManager* om, Referenced* rmg, bool load_ext) override;


        std::map<std::string, ref_ptr< RenderPass > > getPasses() { return mPasses; }
        std::map<std::string, ref_ptr< Texture > > getTargets() { return mTargets; }
        //std::map<std::string, ref_ptr< SceneNode > >getSceneNodes() { return mSceneNodes; }

        void addStandardPass(std::string pass_name);
        void addImagePass(std::string pass_name);

        void addRengerTarget(std::string target_name);

        void setInputSceneNode(std::string pass_name, std::string node_path);
        void setInputCamera(std::string pass_name, std::string node_path);

        void setRengerTarget(std::string pass_name, std::string target_name);


    private:

        std::map<std::string, ref_ptr< RenderPass > > mPasses;
        std::map<std::string, ref_ptr< Texture > > mTargets;
        //std::map<std::string, ref_ptr< SceneNode > > mSceneNodes;

        std::map<std::string, std::string > mInputCamera;
        std::map<std::string, std::string > mInputSceneNodeSlot;

    };

}//namespace egg

#endif // _EGG_COMPOSER_H_
