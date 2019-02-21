#ifndef NIUBI_BASE_PATH_H
#define NIUBI_BASE_PATH_H

#include <egg/Export.h>
//#include <nbBase/Log.h>

//#include <list>
#include <string>
#include <vector>
#include <deque>


/*
* ���������һ��·���������
* �Ժ�Ҫ���������ϵͳ�йصĹ���ȫ���ƶ��� system�����ﴦ��
*/

namespace egg
{
    
    //!�ļ�·������
	class EGG_RUNTIME_EXPORT Path
	{
	public:
		//Path(const char* path = "");
        Path(const std::string& path = "");
		~Path();

        //for std::map key
        bool operator < (const Path & cmp) const
        {
            if(mIsAbs != cmp.mIsAbs)
            {
                return mIsAbs < cmp.mIsAbs;
            }
            return getPathMel() < cmp.getPathMel();
        } 

        inline Path& operator = (const Path& r)
        {
            mPathList = r.mPathList;
            mIsAbs = r.mIsAbs;
            return *this;
        }
        inline Path& operator = (const char* path )
        {
            set(path);
            return *this;
        }
        inline Path& operator = (const std::string& str)
        {
            set( str.c_str() );
            return *this;
        }

		//�Ƿ��Ǿ���·��
        bool isAbsolutePath()const{ return mIsAbs; }
        void setAbsolutePath(bool b){ mIsAbs = b; }

        //�Ƿ�Ϊ��Ч
        //    ������Ǿ���·�� ����·��Ϊ�� ����Ϊ����Ч
        bool isNull(){return mPathList.empty() && (!mIsAbs); }

        //·������
        unsigned int size(){return mPathList.size(); }


		//����·��
		//	����1 ·������
		//	      ����·�������� / | c: ��ʼ���������������ȶ�·��
		//        ͬʱ֧�ֻ��·������
		void set( const char* path );

        //׷����·��
        //  ����1 ���������·��
		void push(const char* path);
        void push(const Path& path);

        //����·��
        //  ����1 ���ɵĲ���
        //        ����·������������̷� ���·�������������ǰ
		void pop(unsigned int level = 1);

        std::string getFront();
        void popFront();

        //��ȡĩ������
        std::string getLastName() const;

        //��ȡĩ���ļ�������.�ָ������
        std::vector< std::string > splitLastName() const;


		//��ȡ�ƶ���ʽ��·��
		std::string getPathWindows(bool only_dir = false) const;//���windows·��
		std::string getPathUnix(bool only_dir = false) const;//���unix·��
        std::string getPathMel(bool only_dir = false,bool as_abs = false) const;//���mel·��

        //�����ǰϵͳ·��
        std::string getNativePath(bool only_dir = false) const;


        //�������·��
        //    form + return = to
        static Path relative(const char* from_abs_path, const char* to_abs_path, bool from_is_file );
        static Path relative(Path from_abs_path, Path to_abs_path ,bool from_is_file ); //form·���ǲ���һ���ļ�


	private:

		//�洢�ṹ
		std::deque< std::string > mPathList;

		//�Ƿ��Ǿ���·��
		bool mIsAbs;
	};

}//namespace nbBase

#endif//NIUBI_BASE_PATH_H

