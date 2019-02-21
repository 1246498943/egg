#ifndef _EGG_Component_Camera_H_
#define _EGG_Component_Camera_H_

#include <eggEngine/Component.h>
#include <eggEngine/RenderNode.h>
#include <eggEngine/SceneNode.h>
#include <eggEngine/Matrix.h>

namespace egg
{

    class EGG_ENGINE_EXPORT Camera
        :public ComponentBase
    {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        Camera() 
        {
            mViewMatrix.setIdentity();
            mProjactiveMatrix.setIdentity();

            //vmml::frustumf f;
            //f.set_perspective(30, 1.0, 0.01, 1000.0);
            //mProjactiveMatrix = f.compute_matrix();
            mProjactiveMatrix = Eigen::perspective(30.0f, 1.0f, 0.1f, 100.0f);

            //mViewMatrix = vmml::mat4f(vmml::vec3f(0, 0, 0), vmml::vec3f(0, 0, 0), vmml::vec3f(0, 0, 0));
            mViewMatrix.setIdentity();


            mClearColor = Eigen::Vector4f(0.1f, 0.2f, 0.3f, 1.0f);
            mViewPort = Eigen::Vector4f(0.0f, 0.0f, 512.0f, 512.0f);
        }
        virtual ~Camera() {}

        virtual std::string ObjectClassName() override { return "camera"; }

        virtual void onUpdate(nv_update* nv) override {}
        virtual void onCull(nv_cull* nv, Eigen::Matrix4f& vm) override
        {
            //�����������Ⱦ״̬
            if (mPass.valid())
            {
                //������������Ⱦ״̬���þ����Ϊ��ȫ����������Ⱦ״̬
                nv->mRenderNode->setPass(mPass.get());
                nv->ignore_pass = true;
            }
            else
            {
                nv->ignore_pass = false;
            }

            //�ӵ�ǰ�����ȡ����ȷ��ͶӰ����
            //  ��Ϊһ��cull visitor���ܻ������������������������ǿ�����õ�ǰ��ͶӰ����
            nv->projactiveMatrix = this->mProjactiveMatrix;
            nv->cameraViewMatrix = this->mViewMatrix;

            //����ֱ�ӱ�����������ѭ�� ����Ҫ��������
            //nv->mSceneNode->accept(*nv);
            for (size_t i = 0; i < nv->mSceneNode->getNumChild(); i++)
            {
                nv->mSceneNode->getChild(i)->accept(*nv);
            }
        }
        virtual void onRender(nv_render* nv, Eigen::Matrix4f& mv) override
        {
            //set viewport
            nv->render->setViewport(
                //0, 0, 500, 500
                mViewPort[0], mViewPort[1], mViewPort[2], mViewPort[3]
            );

            //clear frame buffer
            nv->render->clear(Renderer::CLEAR_MASK::CM_COLOR
                | Renderer::CLEAR_MASK::CM_ZBUFFER
                | Renderer::CLEAR_MASK::CM_STENCIL
                //, 0.1f, 0.2f, 0.3f, 1.0f
                ,mClearColor[0], mClearColor[1], mClearColor[2], mClearColor[3]
                , 1.0f, 0.0f);

        }
        virtual void onInput(nv_input* nv) override 
        {
            if (nv->mWindowEvent.mWindowEventType == egg::WET_SIZE)
            {
                //printf("resize = %f , %f\n",nv->mWindowEvent.z, nv->mWindowEvent.w);

                mViewPort = Eigen::Vector4f(0, 0, nv->mWindowEvent.z, nv->mWindowEvent.w);
                if (nv->mWindowEvent.z > 0 && nv->mWindowEvent.w > 0)
                {
                    mProjactiveMatrix = Eigen::perspective(30.0f, nv->mWindowEvent.z / nv->mWindowEvent.w, 0.1f, 100.0f);
                }
            }
        }

        void setPass(egg::Pass* pass) { mPass = pass; }
        egg::Pass* getPass() { return mPass.get(); }

        void setViewPort(const Eigen::Vector4f& viewport) { mViewPort = viewport; }
        Eigen::Vector4f getViewPort() { return mViewPort; }

        void setProjactiveMatrix(const Eigen::Matrix4f& projactivematrix) { mProjactiveMatrix = projactivematrix; }
        Eigen::Matrix4f getProjactiveMatrix() { return mProjactiveMatrix; }

        void setViewMatrix(const Eigen::Matrix4f& viewmatrix) { mViewMatrix = viewmatrix; }
        Eigen::Matrix4f getViewMatrix() { return mViewMatrix; }

    private:

        Eigen::Vector4f mClearColor;
        Eigen::Vector4f mViewPort;
        Eigen::Matrix4f mProjactiveMatrix; //ͶӰ����
        Eigen::Matrix4f mViewMatrix;       //������������������� Ҳ����lookat�Ľ��
        ref_ptr<egg::Pass> mPass;
    };



    class EGG_ENGINE_EXPORT CameraHandler
        :public ComponentBase
    {
    public:
        CameraHandler() {}
        virtual ~CameraHandler() {}

        virtual std::string ObjectClassName() override { return "camerahandler"; }

        virtual void onInput(nv_input* nv) override;

        //���״̬
        float mouse_x = 0;
        float mouse_y = 0;
        float mouse_last_x = 0;
        float mouse_last_y = 0;
        float mouse_move_x = 0;
        float mouse_move_y = 0;

        //����״̬
        bool mouse_pressed[3] = { false, false, false };

        //������λ����״̬
        float fuyang = 0;
        float fangwei = 0;
        float juli = 10;

    };



}//namespace egg

#endif //_EGG_Component_Camera_H_
