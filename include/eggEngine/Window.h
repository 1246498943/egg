#ifndef _EGG_LAUNCHER_H_
#define _EGG_LAUNCHER_H_

#include <eggEngine/Export.h>

#include <string>
#include <queue>
#include <queue>

#include <egg/Object.h>
#include <egg/Singleton.h>
#include <egg/Factory.h>
#include <egg/DataType.h>


namespace egg
{

	class GraphicsContext;
    class Renderer;
#if 0
    class Window;
    

    class EGG_ENGINE_EXPORT WindowEventCallback
        :public Object
    {
    public:

        WindowEventCallback() {}
        virtual ~WindowEventCallback() {}

        virtual void onKey(int keycode, int keystate, int x, int y) = 0;

        //���������Ϣ
        //  type        0 move 1 click
        //  moustbutton 1 ��� 2 �Ҽ� 3 �м�
        //  moustbuttonstate 0 up 1 down
        //  x y ����ֵ ���Ͻ�ԭ��
        virtual void onMouse(int type, int moustbutton, int moustbuttonstate, int x, int y) = 0;

        //��������Ϣ
        //  message     0 null 1 add cursor 2 move cursor 3 delete cursor 
        //  cursor_id   ��0��ʼ 
        //  x y         ���Ͻǰٷֱ�
        virtual void onTouch(unsigned int message, unsigned int cursor_id, float x, float y) = 0;

        virtual void onSize(int w, int h) = 0;

        //virtual void onFrame(float dt) = 0;
        //virtual void onDraw(float dt) = 0;

        Window* mWindow;

    protected:
    private:
    };
#endif

    enum WindowEventType
    {
        WET_NULL
        , WET_MOUSE
        , WET_KEY
        , WET_TOUCH
        , WET_SIZE
    };

    enum KeyState
    {
        KS_NULL
        , KS_UP
        , KS_DOWN
    };

    enum MouseType
    {
        MT_Null
        , MT_Move
        , MT_Click
    };

    enum MouseButton
    {
        MB_Null
        , MB_Left
        , MB_Middle
        , MB_Right
    };

    enum KeyCode //����code
    {
        KC_Null
        ,KC_1
    };

    enum TouchType
    {
        TT_Null
        , TT_AddCursor
        , TT_MoveCursor
        , TT_DeleteCursor
    };


    //������Ϣ�¼�
    class EGG_ENGINE_EXPORT WindowEvent
        //:public Object
    {
    public:

        WindowEvent() {}
        virtual ~WindowEvent() {}

        WindowEventType mWindowEventType = WET_NULL;

        KeyCode mKeyCode = KC_Null;
        KeyState mKeyState = KS_NULL; //key and mouse
        
        MouseType mMouseType = MT_Null;
        MouseButton mMouseButton = MB_Null;

        TouchType mTouchType = TT_Null;
        unsigned int mCursorID = 0;

        float x = 0;
        float y = 0;
        float z = 0;
        float w = 0;
    };

#if 0
    class EGG_ENGINE_EXPORT WindowEventQueue
        :public Object
    {
    public:

        WindowEventQueue() {}
        virtual ~WindowEventQueue() {}

        //��������
        virtual void onKey(KeyCode keycode, KeyState keystate, int x, int y)
        {
            WindowEvent we;
            we.mWindowEventType = WET_KEY;
            we.x = x;
            we.y = y;
            we.mKeyCode = keycode;
            we.mKeyState = keystate;
            mWindowEventQueue.push(we);
        }

        //���������Ϣ
        //  type        0 move 1 click
        //  moustbutton 1 ��� 2 �Ҽ� 3 �м�
        //  moustbuttonstate 0 up 1 down
        //  x y ����ֵ ���Ͻ�ԭ��
        virtual void onMouse(MouseType type
            , MouseButton moustbutton, KeyState moustbuttonstate, int x, int y)
        {
            WindowEvent we;
            we.mWindowEventType = WET_MOUSE;
            we.x = x;
            we.y = y;
            we.mMouseType = type;
            we.mMouseButton = moustbutton;
            we.mKeyState = moustbuttonstate;
            mWindowEventQueue.push(we);
        }

        //��������Ϣ
        //  message     0 null 1 add cursor 2 move cursor 3 delete cursor 
        //  cursor_id   ��0��ʼ 
        //  x y         ���Ͻǰٷֱ�
        virtual void onTouch(TouchType message, unsigned int cursor_id, float x, float y)
        {
            WindowEvent we;
            we.mWindowEventType = WET_TOUCH;
            we.x = x;
            we.y = y;
            we.mTouchType = message;
            we.mCursorID = cursor_id;
            mWindowEventQueue.push(we);
        }


        //����λ�õ���
        //   ���Ͻ�ԭ�� ���ص�λ
        virtual void onSize(int x, int y, int width, int height)
        {
            WindowEvent we;
            we.mWindowEventType = WET_SIZE;
            we.x = x;
            we.y = y;
            we.z = width;
            we.w = height;
            mWindowEventQueue.push(we);
        }

        std::queue<WindowEvent>& getWindowEventQueue()
        {
            return mWindowEventQueue;
        }

    private:

        std::queue<WindowEvent> mWindowEventQueue;
    };
#endif


    class EGG_ENGINE_EXPORT Window
		:public Object
	{

	public:

        Window()
            :mDone(false), mGraphicsContext(0)
            //, mWindowEventCallback(0)
            //, mGloble(0)
            ,mDisplay(0){}
		virtual ~Window(){}

        //DECLARE_FACTORY(Window);

		//void setEventCallback(WindowEventCallback* cb)
		//{
  //          mWindowEventCallback = cb;
  //          mWindowEventCallback->mWindow = this;
		//}

	public:

		void frame(double dt);

		virtual bool isShow() = 0;

		//virtual void sleep(unsigned int ms) = 0;
		//virtual std::string getDataPath() = 0;

	public:

        //��������
        // ָ��Windowed ָ��fullscreen ��������
        virtual bool createWindow(const char* title, int width, int height, int bits, bool fullscreen
            , GraphicsContext* gc = 0, Renderer* renderer = 0) = 0;
        virtual bool destroyWindow() = 0;

    protected:

		//void setGraphicsContext(GraphicsContext* gc){mGraphicsContext = gc;}
		GraphicsContext* mGraphicsContext;//gc����������

		//void setRenderer(Renderer* renderer){ mRenderer = renderer; }
		//Renderer* mRenderer; //renderer����������

		//void setGloble(void* g){ mGloble = g; }
		//void* mGloble; //ȫ�ֶ���������

	protected:

		//����ϵͳ����ĸ��½ӿ�
		virtual bool updateWindow() = 0;

	public:

		//��ѭ�����
		bool mDone;

		//������ID ò��ֻ��windows���������
        HANDLE_TYPE mInstance;
        
        //��ʾ���������� x11ר�� ��¼Display*����
        HANDLE_TYPE mDisplay;

		//�����ڱ�ʶ�� ����ϵͳ��ͬ��Ҫǿ��ת���ɲ�ͬ����
        // windows������һ����� ����uint
        // x11������һ�� ulong
        HANDLE_TYPE mWinID;

		//��ռȫ�����
		bool mFullscreen;

		//��Ϣ�ص�
        //WindowEventCallback* mWindowEventCallback;

	protected:


        //�������
    public:
        //��������
        virtual void onKey(KeyCode keycode, KeyState keystate, int x, int y)
        {
            WindowEvent we;
            we.mWindowEventType = WET_KEY;
            we.x = x;
            we.y = y;
            we.mKeyCode = keycode;
            we.mKeyState = keystate;
            mWindowEventQueue.push(we);
        }

        //���������Ϣ
        //  type        0 move 1 click
        //  moustbutton 1 ��� 2 �Ҽ� 3 �м�
        //  moustbuttonstate 0 up 1 down
        //  x y ����ֵ ���Ͻ�ԭ��
        virtual void onMouse(MouseType type
            , MouseButton moustbutton, KeyState moustbuttonstate, int x, int y)
        {
            WindowEvent we;
            we.mWindowEventType = WET_MOUSE;
            we.x = x;
            we.y = y;
            we.mMouseType = type;
            we.mMouseButton = moustbutton;
            we.mKeyState = moustbuttonstate;
            mWindowEventQueue.push(we);
        }

        //��������Ϣ
        //  message     0 null 1 add cursor 2 move cursor 3 delete cursor 
        //  cursor_id   ��0��ʼ 
        //  x y         ���Ͻǰٷֱ�
        virtual void onTouch(TouchType message, unsigned int cursor_id, float x, float y)
        {
            WindowEvent we;
            we.mWindowEventType = WET_TOUCH;
            we.x = x;
            we.y = y;
            we.mTouchType = message;
            we.mCursorID = cursor_id;
            mWindowEventQueue.push(we);
        }


        //����λ�õ���
        //   ���Ͻ�ԭ�� ���ص�λ
        virtual void onSize(int x, int y, int width, int height)
        {
            WindowEvent we;
            we.mWindowEventType = WET_SIZE;
            we.x = x;
            we.y = y;
            we.z = width;
            we.w = height;
            mWindowEventQueue.push(we);
        }

        std::queue<WindowEvent>& getWindowEventQueue()
        {
            return mWindowEventQueue;
        }

        void cleanWindowEventQueue()
        {
            std::queue<WindowEvent> n;
            return mWindowEventQueue.swap(n);
        }

	private:

        std::queue<WindowEvent> mWindowEventQueue;
	};


    class EGG_ENGINE_EXPORT WindowFactory
        :public Factory<Window>
    {
    public:
        WindowFactory() {}
        ~WindowFactory() {}
        DECLARE_SINGLETON(WindowFactory)
    };


//#include <egg/GraphicsContext.h>

    //void Window::frame(float dt)
    //{
    //    //���´���
    //    this->updateWindow();

    //    if (isShow())
    //    {
    //        //������Ϣ
    //        if (m_gc_plug)m_gc_plug->frame();
    //        m_event_callback->onFrame(dt);
    //        m_event_callback->onDraw(dt);
    //    }
    //}



}//namespace egg


#endif // _EGG_LAUNCHER_H_


