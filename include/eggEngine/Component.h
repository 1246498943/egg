#ifndef NIUBI_SG_COMPONENTBASE_H
#define NIUBI_SG_COMPONENTBASE_H

//#include <nbBase/Public.h>
#include <string>

#include <egg/Math.h>
#include <egg/Object.h>
#include <egg/Node.h>
//#include <eggEngine/View.h>

#include <eggEngine/Resource.h>
#include <eggEngine/RenderVisitor.h>
#include <eggEngine/InputVisitor.h>
#include <eggEngine/Matrix.h>

namespace egg
{

    /*
    * ������ֽڵ� ����������Ϊ �����Կ��ƴ�������
    */

    class EGG_ENGINE_EXPORT ComponentBase
        :public Node
    {
        friend class SceneNode;

    public:
        ComponentBase(Node* node =0,const std::string& name = "component")
            :Node(name),mAttachNode(node){}
        virtual ~ComponentBase(){}

        //����˵��
        //virtual std::string getTypeName(){return "component";}
        virtual std::string ObjectClassName() override {return "component";}

        //
        virtual void onAttach() {}
        virtual void onDetach() {}

        //�ּ�����Ϣ�ӿ�
        virtual void onInput(nv_input* nv) {} //������Ϣ����
        virtual void onUpdate(nv_update* nv) {} //�������
        virtual void onCull(nv_cull* nv, Eigen::Matrix4f& vm) {} //ɸѡ����ͼ������Ⱦͼ
        virtual void onRender(nv_render* nv, Eigen::Matrix4f& mv) {} //����ӿڳ���Camera�������Ⲣû��ʵ��ͨ;
        virtual void onGUI(nv_gui* nv) {} //��Ⱦ��ɺ�ר�Ŵ���GUI�Ļ��ƹ���


        //��Ϣ�ּ�
        //   ��Ϊ�����һ����ѭ�����ı����߼���������Щ��Ϣ��Ҫ����Ԥ�������Ա���ּ���
        virtual void traverse(NodeVisitor& nv);

        Node* getNode(){return mAttachNode.get().get();}

    private:
        egg::observer_ptr<Node> mAttachNode;

        Eigen::Matrix4f calculateModelView(NodeVisitor& nv);
    };



    //�������Ӧ���Ǹ���Դ ��ʱռһ������
    //class NIUBI_SG_EXPORT Line
    //    :public ComponentBase
    //{
    //public:
    //    Line(){}
    //    virtual ~Line(){}

    //    virtual std::string ObjectClassName() override {return "line";}

    //    virtual void onUpdate(View::nv_update* nv) override {}
    //    virtual void onCull(View::nv_cull* nv) override {}
    //    virtual void onRender(View::nv_render* nv ,vmml::mat4f& mv) override {}

    //};


}//namespace egg


#endif//NIUBI_SG_COMPONENTBASE_H
