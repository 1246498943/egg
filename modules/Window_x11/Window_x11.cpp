
#include <nbPlugins/Public.h>

/* Xlib.h is the default header that is included and has the core functionallity */
#include <X11/Xlib.h>
/* Xatom.h includes functionallity for creating new protocol messages */
#include <X11/Xatom.h>
/* keysym.h contains keysymbols which we use to resolv what keys that are being pressed */
#include <X11/keysym.h>
/* printf */
#include <stdio.h>
/* the XF86 Video Mode extension allows us to change the displaymode of the server
* this allows us to set the display to fullscreen and also read videomodes and
* other information.                                                               */
#include <X11/extensions/xf86vmode.h>

#include <iostream>

#include <X11/Xutil.h>
// #include <X11/extensions/Xcomposite.h>

#include <unistd.h>

#define ALL_INPUT_MASK (ExposureMask|KeyPressMask|StructureNotifyMask|ButtonPressMask|ButtonReleaseMask )


class  NativeWindow_x11
	:public nbPlugins::NativeWindow
{
public:

	NativeWindow_x11(){}
	virtual ~NativeWindow_x11(){}


    // Display* m_display;
    // int m_screenID;
    // Window m_XWindow;


	//��������
	// ָ��Windowed ָ��fullscreen ��������
	virtual bool createWindow(const char* title, int width, int height, int bits, bool fullscreen )
	{
		//ͬ����Ա����
		m_fullscreen = fullscreen;


        /*
            windows�µ������ǣ�
                �������ڣ�����ȫ����
                ѡ�����ظ�ʽ
                ����glrc

            x11�µ������ǣ�
                ������ʾ����
                ѡ�����ظ�ʽ
                �������ڣ�����ȫ����
                ����glrc
        */

        //������ʾ����
        Display* display = XOpenDisplay(0);
        if (display == NULL)
        {
            std::cerr << "Could not open the display" << std::endl;
            return false;
        }
        m_display = (unsigned int)display;

        //��ȡʵ�����ӵ���Ļid
        int m_screenID = DefaultScreen(display);





        //glx��ʼ����Ҫ��ͬ�� visual_info
        XVisualInfo* visual_info = 0;
        XVisualInfo visual_defaule; //����Ĭ�ϴ���

        //ѡ�����ظ�ʽ
        if(1)
        {
            m_gc_plug->choosePixelFormat(
                this
                ,0,0//gl no use
                ,32,8			//ɫ�� alpha һֱRGB �ܷѽ�
                ,1				//���ۻ��� �޷���
                ,16				//��ô���ö��ǿ��� ����float�����
                ,1				//����0������Ч
                ,1				//��Զ�ǿ�����
                ,4				//������Ч
                ,true			//˫������Ч
			);

            //�õ�ѡ����
            visual_info =  (XVisualInfo*) m_gc_plug->m_pf;
        }

        //���glx��ʼ��ʧ��
        else
        {
            XVisualInfo *visual_list;
            XVisualInfo visual_template;
            int nxvisuals;
            nxvisuals = 0;
            visual_template.screen = m_screenID;

            //�õ�visual���
            visual_list = XGetVisualInfo (display, VisualScreenMask, &visual_template, &nxvisuals);
            for (int i = 0; i < nxvisuals; ++i)
            {
                printf("  %3d: visual 0x%lx class %d (%s) depth %d\n",
                     i,
                     visual_list[i].visualid,
                     visual_list[i].c_class,
                     visual_list[i].c_class == TrueColor ? "TrueColor" : "unknown",
                     visual_list[i].depth);
            }

            //ƥ��һ����ѽ��
            if (!XMatchVisualInfo(display, XDefaultScreen(display), 24, TrueColor, &visual_defaule))
            {
                fprintf(stderr, "no such visual\n");
                //return 1;
            }

            visual_info = &visual_defaule;
        }

        printf("Matched visual 0x%lx class %d (%s) depth %d\n",
            visual_info->visualid,
            visual_info->c_class,
            visual_info->c_class == TrueColor ? "TrueColor" : "unknown",
            visual_info->depth);

        //����ɫ�ʱ�
        Colormap colormap = XCreateColormap(display, XDefaultRootWindow(display), visual_info->visual, AllocNone);



#if 0
        //��ѯ��Ƶģʽ�汾��Ϊ�˸���ȫ�����ܣ�
        int vmMajor, vmMinor;
        XF86VidModeQueryVersion(display, &vmMajor, &vmMinor);
        printf("XF86 VideoMode extension version %d.%d\n", vmMajor, vmMinor);

        //��ѯ������Ƶģʽ
        XF86VidModeModeInfo **modes;
        int modeNum;
        int vmMajor, vmMinor;
        XF86VidModeGetAllModeLines(display, screen, &modeNum, &modes);

        //��ѯ����ģʽ/�������ģʽ
        XF86VidModeModeInfo desktopMode = *modes[0]; ;
        int bestMode = 0;
        for (i = 0; i < modeNum; i++)
        {
            if ((modes[i]->hdisplay == width) && (modes[i]->vdisplay == height))
            {
                bestMode = i;
            }
        }
#endif

        //����ȫ��
		// if (m_fullscreen && m_gc_plug->isWindowsAuxFull())// Attempt Fullscreen Mode?
        // {
        // }


        //��������
        Window win;
        XSetWindowAttributes attrs;
        attrs.colormap = colormap;
        attrs.background_pixel = 0;
        attrs.border_pixel = 0;
        win = XCreateWindow(display, XDefaultRootWindow(display)
                        , 0, 0, width, height
                        , 0, visual_info->depth, InputOutput
                        , visual_info->visual, CWBackPixel | CWColormap | CWBorderPixel
                        , &attrs);


        //XSync(dpy, True);
        XMapWindow   (display, win);
        XStoreName   (display, win, title);
        XSelectInput (display, win, ALL_INPUT_MASK);

        //��¼windows id
        m_winID = (unsigned int) win;


        //����glrc
        m_gc_plug->createGraphaContext(this);
		m_renderer_plug->init();


		return true;
	}

	virtual bool destroyWindow()
	{

		//�����豸
		// m_gc_plug->destroyGraphaContext(this);

		return true;
	}


	virtual bool isShow()
	{
		return true;
	}

	virtual void sleep(unsigned int ms)
	{
		usleep(ms*1000);
	}

	std::string getDataPath()
	{
		return "../data/";
	}

    virtual bool isSupportMultiTouch()
    {
        return false;
    }

	virtual bool updateWindow()
	{
        Display* display = (Display*)m_display;
        Window win = (Window)m_winID;

        bool s = true;
        while(s)
        {
            XEvent event = {0};
            // printf("XNextEvent 1\n");
            //XNextEvent(display, &event);
            s = XCheckWindowEvent( display, win , ALL_INPUT_MASK, &event);
            // printf("XNextEvent 2\n");
            switch(event.type)
            {
                case ConfigureNotify:
                {
                    int width = event.xconfigure.width;
                    int height = event.xconfigure.height;
                    //printf("resize %d %d\n",width,height);

                    this->m_event_callback->onSize(width,height);
                    break;
                }

                case Expose:
                {
                    // XSetForeground(display, gc, WhitePixel(display, screen));
                    // XFillRectangle(display, win, gc, 0, 0, width, height);
                    // XSetForeground(display, gc, BlackPixel(display, screen));
                    // XDrawString(display, win, gc, width/2, height/2, "XWindow", 7);
                    break;
                }

                case KeyPress:
                {
                    // if(event.xkey.keycode == XKeysymToKeycode(display, XK_Escape))
                    // {
                        // XFreeGC(display, gc);
                        // XCloseDisplay(display);
                        // return 0;
                    // }

                    break;
                }
                case ButtonPress:
                {
                    int mx = event.xkey.x;
                    int my = event.xkey.y;
                    unsigned int code = event.xkey.keycode;
                    //printf("ButtonPress %d %d %d\n",code,mx,my);
                    this->m_event_callback->onMouse(code,1,mx,my);
                    break;
                }
                case ButtonRelease:
                {
                    int mx = event.xkey.x;
                    int my = event.xkey.y;
                    unsigned int code = event.xkey.keycode;
                    //printf("ButtonRelease %d %d\n",mx,my);
                    this->m_event_callback->onMouse(code,0,mx,my);
                    break;
                }

                default: break;

            }

        }


		return true;
	}

protected:
private:
};



//ע��
NIUBI_REGISTER_PLUGINS_FUNCTION_BEGIN( pm ,NativeWindow_x11)
NIUBI_REGISTER_PLUGINS(pm,NativeWindow_x11)
NIUBI_REGISTER_PLUGINS_FUNCTION_END
