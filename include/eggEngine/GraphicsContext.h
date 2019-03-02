#ifndef GraphicsContext_h__
#define GraphicsContext_h__

//#include <egg/Public.h>
#include <eggEngine/Export.h>

#include <egg/Object.h>
#include <egg/Singleton.h>
#include <egg/Factory.h>
#include <egg/DataType.h>

namespace egg
{
	class Window;

	class EGG_ENGINE_EXPORT GraphicsContext
        :public Object
	{

	public:

		GraphicsContext()
            :mCreated(false),m_globle(0),mNeedReszie(false),mNativeWindow(0)
            , m_GI(0)
            , m_DC(0)
            , m_gcID(0)
            , m_pf(0)
        {}
		virtual ~GraphicsContext(){}

        //DECLARE_FACTORY(GraphicsContext);

	public:
    
		//ѡ�����ظ�ʽ
		virtual bool choosePixelFormat(
            Window* win
			,int width, int height	//����WH  dx��Ҫ gl����Ҫ
			,int ColorBits			//24 32
			,int AlphaBits			//0
			,int AccumBits			//0
			,int DepthBits			//24Bit Z-Buffer (Depth Buffer)  
			,int StencilBits		//0
			,int AuxBuffers			//0
			,int samples			//���������  0�ر� 2 4 8 16
			,bool doublebuffer		//˫����
			) = 0;

		//�����豸����
		virtual bool createGraphaContext(Window* win) = 0;
		virtual bool destroyGraphaContext(Window* win) = 0;
		virtual bool isCreated(){return mCreated;}

		void setGloble(void* g){m_globle = g; }
		void* m_globle; //ȫ�ֶ���������

		//����������
		virtual void swapBuffers() = 0;

		//ȫ���豸����
		//opengl����  dx��LPDIRECT3D9
        HANDLE_TYPE m_GI;

		//�����豸ID
		//opengl��dc  dx����
        HANDLE_TYPE m_DC;

		//ͼ���豸��ʶ�� 
		//���ݽӿڻ�����ͬ��Ҫǿ��ת���ɲ�ͬ����  
		//gl����HGLRC  dx����LPDIRECT3DDEVICE9
        HANDLE_TYPE m_gcID;
        
        //��ǰѡ������ظ�ʽ Ĭ��Ϊ0
        // windowsΪPixelFormat
        // x11Ϊvisualָ��
        HANDLE_TYPE m_pf;


		//��ǰͼ�λ����Ƿ���Ҫ���ڸ���ȫ��  Ŀǰ��֪��ֻ��dx����Ҫ
		virtual bool isWindowsAuxFull() = 0;

		//����android���µ�
		void needResize(){mNeedReszie = true;}
		virtual void frame() = 0;

        //
        virtual bool makeCurrent() = 0;

        //���ô�ֱͬ��
        virtual bool setVSync(bool b) = 0;

	protected:

		bool mCreated;
		bool mNeedReszie;
        Window* mNativeWindow;
	};


    class EGG_ENGINE_EXPORT GraphicsContextFactory
        :public Factory<GraphicsContext>
    {
    public:
        GraphicsContextFactory() {}
        ~GraphicsContextFactory() {}
        DECLARE_SINGLETON(GraphicsContextFactory)
    };

}

#endif // GraphicsContext_h__

