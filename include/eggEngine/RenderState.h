#ifndef _EGG_RenderState_H_
#define _EGG_RenderState_H_

#include <eggEngine/Export.h>

#include <vector>
#include <string>
#include <map>
#include <stack>

#include <egg/Object.h>
//#include <eggEngine/Pass.h>


namespace egg
{

    class GraphicsContext;
    class Renderer;
    class Pass;

    //��Ⱦ״̬����
    class EGG_ENGINE_EXPORT RenderState
        :public Object
    {
    public:

        RenderState() {}
        virtual ~RenderState() {}

        //������Ҫ��ջ����
        void setModelViewMatrix(float* m44) { memcpy(&modelViewMatrix[0], &m44[0], 16 * sizeof(float)); }
        void setProjectionMatrix(float* m44) { memcpy(&projectionMatrix[0], &m44[0], 16 * sizeof(float)); }

        //ʹ��pass��������ǰ��Ⱦ״̬
        egg::Pass* top();
        //void set(egg::Material::Pass* pass);
        void push(egg::Pass* pass);
        void pop();


        //�󶨵�ǰ��Ⱦ״̬
        virtual void bindRenderPass(GraphicsContext* gc, Renderer* renderer) = 0;
        virtual void unbindRenderPass() = 0;

        //�������¿ռ�λ��
        virtual void applyMatrix(egg::GraphicsContext* gc, egg::Renderer* renderer) = 0;

    protected:

        //matrix
        float modelViewMatrix[16];
        float projectionMatrix[16];

    private:
        //
        std::stack< ref_ptr<egg::Pass> > mStack;

    };


}//namespace egg


#endif // _EGG_RenderState_H_
