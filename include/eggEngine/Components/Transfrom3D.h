#ifndef _EGG_Component_Transfrom3D_H_
#define _EGG_Component_Transfrom3D_H_

#include <eggEngine/Component.h>

#include <eggEngine/Components/IconMesh.h>


namespace egg
{

    //3D�ռ���
    //      ����һ��matrix4x4����
    //      ������Ҫ��ʾһ��icon
    class EGG_ENGINE_EXPORT Transfrom3D
        :public ComponentBase
    {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        Transfrom3D(const Eigen::Matrix4f& m = Eigen::Matrix4f::Identity())
        {
#if 0
            std::vector<float> position_v4;
            std::vector<float> color_v4;

            //0 red
            position_v4.push_back(0.0);
            position_v4.push_back(0.0);
            position_v4.push_back(0.0);
            position_v4.push_back(1.0);
            color_v4.push_back(1.0);
            color_v4.push_back(0.0);
            color_v4.push_back(0.0);
            color_v4.push_back(1.0);
            //1 red
            position_v4.push_back(1.0);
            position_v4.push_back(0.0);
            position_v4.push_back(0.0);
            position_v4.push_back(1.0);
            color_v4.push_back(1.0);
            color_v4.push_back(0.0);
            color_v4.push_back(0.0);
            color_v4.push_back(1.0);
            //0 green
            position_v4.push_back(0.0);
            position_v4.push_back(0.0);
            position_v4.push_back(0.0);
            position_v4.push_back(1.0);
            color_v4.push_back(0.0);
            color_v4.push_back(1.0);
            color_v4.push_back(0.0);
            color_v4.push_back(1.0);
            //1 green
            position_v4.push_back(0.0);
            position_v4.push_back(1.0);
            position_v4.push_back(0.0);
            position_v4.push_back(1.0);
            color_v4.push_back(0.0);
            color_v4.push_back(1.0);
            color_v4.push_back(0.0);
            color_v4.push_back(1.0);
            //0 blue
            position_v4.push_back(0.0);
            position_v4.push_back(0.0);
            position_v4.push_back(0.0);
            position_v4.push_back(1.0);
            color_v4.push_back(0.0);
            color_v4.push_back(0.0);
            color_v4.push_back(1.0);
            color_v4.push_back(1.0);
            //1 blue
            position_v4.push_back(0.0);
            position_v4.push_back(0.0);
            position_v4.push_back(1.0);
            position_v4.push_back(1.0);
            color_v4.push_back(0.0);
            color_v4.push_back(0.0);
            color_v4.push_back(1.0);
            color_v4.push_back(1.0);

            icon.build(position_v4, color_v4);
#endif 
            localMatrix = m;
            //localMatrix.setIdentity();
            worldViewMatrix.setIdentity();
        }
        virtual ~Transfrom3D() {}

        //virtual std::string getTypeName(){return "transfrom3d";}
        virtual std::string ObjectClassName() override { return "transfrom3d"; }

        virtual void onUpdate(nv_update* nv) override
        {
            //LOGI("onUpdate");

            //���յ����ڵ��modelview �����Լ���mv����
            //renderViewMatrix = viewMatrix * nv->parentViewMatrix;
            //nv->parentViewMatrix = renderViewMatrix;
        }
        virtual void onCull(nv_cull* nv, Eigen::Matrix4f& vm) override
        {
            //LOGI("onCull");
        }
        virtual void onRender(nv_render* nv, Eigen::Matrix4f& vm) override
        {
            //LOGI("onRender");

            //ʹ���Լ���¼��mv������Ⱦ
            //proj����ʹ��mv��
            //icon.render(nv->render, nv->projactiveMatrix, worldViewMatrix);
        }
        virtual void onInput(nv_input* nv) override {}

        Eigen::Matrix4f getLocalMatrix() { return localMatrix; }
        void setLocalMatrix(const Eigen::Matrix4f& lm) { localMatrix = lm; }
        Eigen::Matrix4f getWorldViewMatrix() { return worldViewMatrix; }
        void setWorldViewMatrix(const Eigen::Matrix4f& wm) { worldViewMatrix = wm; }

        virtual void save(Path filename, std::ostream& os, Serializer* ser) const override
        {
            ComponentBase::save(filename, os, ser);

            const float* m = localMatrix.data();
            for (int i = 0; i < 16; i++)
            {
                ser->writeNumber(os, m[i]);
            }
        }

        virtual void load(Path filename, std::istream& is, Serializer* ser, ObjectManager* om, Referenced* rmg, bool load_ext) override
        {
            ComponentBase::load(filename, is, ser, om, rmg, load_ext);

            float m[16];
            for (int i = 0; i < 16; i++)
            {
                ser->readNumber(is, m[i]);
            }

            //����ֱ�Ӹ�matrix��ֵһ������ ����ʹ��setǿ�����һ������
            //localMatrix = m;
            //localMatrix.set(&m[0], &m[15], false);
            for (size_t i = 0; i < 16; i++)
            {
                localMatrix << m[i];
            }
        }

        //void getPosition(float& x,float& y,float& z);
        //void setPosition(float x,float y,float z);
        //void getRotate(float& x,float& y,float& z);
        //void setRotate(float x,float y,float z);
        //void getScale(float& x,float& y,float& z);
        //void setScale(float x,float y,float z);

    protected:

        Eigen::Matrix4f localMatrix;        //local ����
        Eigen::Matrix4f worldViewMatrix;    //ȫ��MV����(�������û��޸�)

        //IconMesh icon; //������ʾ����
    };
}//namespace egg

#endif //_EGG_Component_Transfrom3D_H_
