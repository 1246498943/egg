
#include <eggEngine/Window.h>
#include <eggEngine/GraphicsContext.h>

using namespace egg;

//IMPLEMENT_FACTORY(Window);
IMPLEMENT_SINGLETON(WindowFactory)

void Window::frame(double dt)
{
    //�����ǰ�����л�������Ϣ����Ϊ��û�б������������Ϣֱ�����
    //this->cleanWindowEventQueue();

    //���´����ռ���Ϣ
    this->updateWindow();

    //��������
    if (isShow())
    {
        //������Ϣ
        if (mGraphicsContext)mGraphicsContext->frame();
        //mWindowEventCallback->onFrame(dt);
        //mWindowEventCallback->onDraw(dt);
    }

}

