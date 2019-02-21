
#include <eggEngine/RenderWindow.h>

#include <eggEngine/Components/Transfrom3D.h>


using namespace egg;



RenderWindow::RenderWindow(Window* win, GraphicsContext* gc, Renderer* rd)
    :mWindow(win), mGraphicsContext(gc), mRenderer(rd)
{
    init();
}

RenderWindow::~RenderWindow()
{
}

bool RenderWindow::init()
{
    //create root node
    mRoot = new SceneNode();
    mRoot->addComponent(new egg::Camera());
    mRoot->addComponent(new egg::CameraHandler());

    //create default camera
    //mDefaultCamera = new SceneNode();
    //mDefaultCamera->addComponent(new egg::Camera());

    //create default composer
    mComposer = new Composer();
    mComposer->addStandardPass("default");

    return true;
}

bool RenderWindow::shutdown()
{

    return true;
}

bool RenderWindow::startInputThread()
{
    return true;
}

bool RenderWindow::startRenderThread()
{
    return true;
}

bool RenderWindow::runStep(double dt_second)
{
    if (mWindow.valid())
    {
        if (mThreadMode == 0)
        {
            processInput(dt_second);
            processRender(dt_second);
        }

        if (mThreadMode == 1)
        {
            //����߳�û�����������߳�
            if (true)
            {

            }
            if (true)
            {

            }
        }

        return mWindow->isShow();
    }
    return false;
}


void RenderWindow::processInput(double dt_second)
{
    //������Ϣ 
    //  1. �ռ�ȫ����Ϣ������Ϣ����
    mWindow->frame(dt_second);

    //  2. �Գ������һ�α�����ת�ӵ�compoents��onInput�ӿ�
    std::queue<WindowEvent> weq = mWindow->getWindowEventQueue();
    while (!weq.empty())
    {
        //ÿ��WindowEvent����һ�α���
        auto we = weq.front();

        ref_ptr<nv_input> nv = new nv_input();
        nv->mWindowEvent = we;

        std::map<std::string, ref_ptr< RenderPass > > rps = mComposer->getPasses();
        for (auto it = rps.begin(); it != rps.end(); it++)
        {
            //֪ͨ����������ӿ�
            egg::Camera* camera = it->second->getCamera();
            if (camera == 0)
            {
                //��һ�θ��µ�ʱ�� camera�Ĺ�������Ч�� ����ʹ��mDefaultCamera
                //camera = mDefaultCamera->getComponent<Camera>();

                //ֱ�Ӵ�ROOTȡ�����
                camera = mRoot->getComponent<Camera>();
            }
            camera->onInput(nv.get());


            if (it->second->getRenderScene())
            {
                //��һ�θ��µ�ʱ��  pass��SceneNode�п�����û�е� �ȴ���Ⱦ���ʺ��ֲ�
                it->second->getRenderScene()->accept(*nv);
            }
        }

        weq.pop();
    }

    // 3. ������Ϣ
    mWindow->cleanWindowEventQueue();
}

void RenderWindow::processRender(double dt_second)
{

    //STEP 1. ����ͳһʱ��

    //STEP 2. ���³���(����ȫ������ͼ)
    ref_ptr<nv_update> nv = new nv_update();
    nv->dt = dt_second;
    //nv->currentViewMatrix = this->viewMatrix;//����Ӧ���ǵ�λ����
    mRoot->accept(*nv);


    //STEP 3. CULL����(����ָ����pass��camera�����ĳ���ͼ)
    //����ȫ�����õ�cameraǿ��ִ��onCull()
    std::map<std::string, ref_ptr< RenderPass > > passes = mComposer->getPasses();
    for (auto it = passes.begin(); it != passes.end(); it++)
    {
        RenderPass* pass = it->second.get();
        StandardRenderPass* spass = dynamic_cast<StandardRenderPass*>(pass);
        if (spass)
        {
            //����Ĭ��ֵ����
            egg::Camera* camera = spass->getCamera();
            if (camera == 0)
            {
                //spass->setCamera(mDefaultCamera->getComponent<Camera>());
                spass->setCamera(mRoot->getComponent<Camera>()); //ʹ��Ĭ���������Ϊ����
            }
            egg::SceneNode* sn = spass->getRenderScene();
            if (sn == 0)
            {
                spass->setRenderScene(mRoot.get()); //ʹ��Ĭ�ϵĳ�����Ϊ��Ⱦ����
            }

            //ÿ��cull֮ǰ���ؽ��������
            spass->resetRenderNode();

            //����ִ��camera��onCull����
            //   ��ǰ��mRenderNode���Ƕ�Ӧcamera����Ⱦ״̬
            //   ������¼camera��pass��rendernode
            ref_ptr<nv_cull> cull = new nv_cull();
            cull->mSceneNode = spass->getRenderScene(); //��������
            cull->mRenderNode = spass->getRenderNode(); //��Ⱦͼ���

            //����Ĭ�ϵ�mv����
            //  �����������������ϵ
            //  scenenode��¼����������������ĵ����ꡣ��������ľ��������ʵ�ʾ��󡣶�������������Ĺ۲����
            //  ����ӵ�renderleaf��ʱ����� cam->vm * sn->vm 
            Eigen::Matrix4f world_matrix;
            world_matrix.setIdentity();

            //��������������ָ���������ͶӰ����
            spass->getCamera()->onCull(cull.get(), world_matrix);


            //�����������Ӧ�ĳ���ͼ
            //  ����������Ⱦͼ
            //spass->getRenderScene()->accept(*cull);
        }
    }


    //STEP 4. ִ����Ⱦ(������Ⱦͼ)
    //std::map<std::string, ref_ptr< RenderPass > > passes = mComposer->getPasses();
    for (auto it = passes.begin(); it != passes.end(); it++)
    {
        RenderPass* pass = it->second.get();

        ref_ptr<nv_render> nv = new nv_render();
        nv->render = mRenderer.get();
        //nv->context = mGraphicsContext->m_gcID;
        nv->gc = mGraphicsContext.get();

        //�趨�������ϵ ��������
        Eigen::Matrix4f n;
        pass->getCamera()->onRender(nv.get(), n);//����ĵڶ�������û���κ�����

        //������Ⱦ��
        pass->getRenderNode()->accept(*nv);
    }

    //STEP 5.ִ��GUI��Ⱦ
    for (auto it = passes.begin(); it != passes.end(); it++)
    {
        RenderPass* pass = it->second.get();

        ref_ptr<nv_gui> nv = new nv_gui();
        nv->renderer = mRenderer.get();
        nv->gc = mGraphicsContext.get();
        nv->window = mWindow.get();
        nv->viewport = pass->getCamera()->getViewPort();

        //mRoot->accept(*nv);
        pass->getRenderScene()->accept(*nv);
    }

    //STEP 6.����������
    mGraphicsContext->swapBuffers();
}

#if 0
void RenderWindowEventCallback::onKey(int keycode, int keystate, int x, int y)
{
}

void RenderWindowEventCallback::onMouse(int type, int moustbutton, int moustbuttonstate, int x, int y)
{
}

void RenderWindowEventCallback::onTouch(unsigned int message, unsigned int cursor_id, float x, float y)
{
}

void RenderWindowEventCallback::onSize(int w, int h)
{
}

//void RenderWindowEventCallback::onFrame(float dt)
//{
//}
//
//void RenderWindowEventCallback::onDraw(float dt)
//{
//}
#endif

