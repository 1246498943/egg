
#include <egg/Path.h>

#include <string>
#include <algorithm>

namespace egg
{

	//Path::Path(const char* path)
	//	:mIsAbs(false)
	//{
	//	set(path);
	//}
    Path::Path(const std::string& path)
    {
        set(path.c_str());
    }

	Path::~Path()
	{

	}

	void Path::set( const char* path )
	{
		mPathList.clear();
		mIsAbs = false;
		push(path);
	}

    void Path::push(const Path& path)
    {
        for (unsigned int i = 0; i < path.mPathList.size(); i++)
        {
            mPathList.push_back(path.mPathList[i]);
        }
    }

	void Path::push(const char* path)
	{
        if (strlen(path) == 0)
        {
            return;
        }


		std::string input_path(path);

		//�жϾ���·��
		int begin_index = 0;//��ʼѭ��·����λ��

		//���û��·��
		if( mPathList.size() == 0)
		{
			//windows
			if( input_path[1] == ':' 
				&& 
				((input_path[0] >= 'a' && input_path[0] <= 'z') || (input_path[0] >= 'A' && input_path[0] <= 'Z'))
				)
			{
				mIsAbs = true;
				begin_index = 2;
				std::string pf = " ";
				pf[0] = input_path[0];
				mPathList.push_back(pf);
			}

			//unix
			if( input_path[0] == '/' )
			{
				mIsAbs = true;
			}

			//mel
			if( input_path[0] == '|' )
			{
				mIsAbs = true;
			}
		}

		std::string dir;//��ʱ���
		for( unsigned int i = begin_index ; i < input_path.size() ; i++ )
		{
			//���������·�������� ��Ϊ�ָ��
			if( input_path[i] == '\\' 
				|| input_path[i] == '/'
				|| input_path[i] == '|')
			{
				//���·��
				if(dir.size() > 0)
				{
					mPathList.push_back(dir);
					dir.clear();
				}
			}
			else //��Ч����
			{
				//����Ǹ�Ŀ¼
				if( input_path[i] == '.' && input_path[i+1] == '.' && 
					(input_path[i+2] == '\\' || input_path[i+2] == '/' || input_path[i+2] == '|')
					)
				{
					i+=2;
					if( mPathList.size() > 0  && (*mPathList.rbegin()) != ".." ) //���..ǰ�治��.. Ҳ���ǵ�һ��
					{
						mPathList.pop_back(); //ɾ�����һ��Ŀ¼
					}
                    else if( mPathList.size() == 0 )
                    {
                        //���ϲ����Ѿ������
                    }
					else //׷�ӱ�� ..
					{
						dir.push_back('.');
						dir.push_back('.');
						mPathList.push_back(dir);
						dir.clear();
					}
					continue;
				}

				//����Ǳ���Ŀ¼
				else if( input_path[i] == '.' && 
					(input_path[i+1] == '\\' || input_path[i+1] == '/' || input_path[i+1] == '|')
					)
				{
					i+=1;
					continue; //ʲôҲ����
				}

				//���ʲô������
				else
				{
					dir.push_back(input_path[i]);
				}

			}
		}//for


		//���һ���ֶ�
		if(dir.size() > 0)
		{
			mPathList.push_back(dir);
			dir.clear();
		}

	}


	void Path::pop(unsigned int level)
	{
		if(mPathList.size() >= level)
		{
			for(unsigned int i=0;i<level;i++)
			{
				mPathList.pop_back();
			}
		}
	}

    std::string Path::getFront()
    {
        return *mPathList.begin();
    }
    void Path::popFront()
    {
        mPathList.pop_front();
    }
    std::string Path::getLastName() const
    {
        return mPathList.back();
    }

    //ע�⣺���ַ���Ϊ��ʱ��Ҳ�᷵��һ�����ַ���  
    void split( const std::string& s, std::string& delim,std::vector< std::string >* ret )  
    {  
        size_t last = 0;  
        size_t index = s.find_first_of(delim,last);  
        while (index != std::string::npos)  
        {  
            ret->push_back(s.substr(last,index-last));  
            last = index + 1;  
            index = s.find_first_of(delim,last);  
        }  
        if (index - last > 0)  
        {  
            ret->push_back(s.substr(last,index - last));  
        }  
    }

    std::vector< std::string > Path::splitLastName() const
    {
        std::vector< std::string > ret;
        std::string delim = ".";
        split(getLastName(), delim , &ret);

        if(ret.size() == 1 && ret[0].size() == 0)
        {
            ret.clear();
        }
        return ret;
    }


    std::string Path::getNativePath(bool only_dir) const
    {
#if _WIN32
        return getPathWindows(only_dir);
#else
        return getPathUnix(only_dir);
#endif
    }

	std::string Path::getPathWindows(bool only_dir) const
	{
		std::string ret;

		for( auto it = mPathList.begin()
			; it != mPathList.end() 
			; it++ )
		{
			if( *it == *mPathList.rbegin() && only_dir)
			{
				break;
			}

			if(it == mPathList.begin())
			{
				if(mIsAbs)
				{
					ret += *it;
					ret += ":";
				}
				else if( *it == ".." )
				{
					ret += *it;
				}
				else // .
				{
					ret += ".\\";
					ret += *it;
				}
			}
			else
			{
				ret += "\\";
				ret += *it;
			}
		}

		return ret;
	}


	std::string Path::getPathUnix(bool only_dir) const
	{
		std::string ret;

        if(mPathList.size() == 0 && mIsAbs )
        {
            return "/";
        }

		for( auto it = mPathList.begin()
			; it != mPathList.end() 
			; it++ )
		{
			if( *it == *mPathList.rbegin() && only_dir)
			{
				break;
			}

			if(it == mPathList.begin())
			{
				if(mIsAbs)
				{
					ret += "/";
					ret += *it;
				}
				else if( *it == ".." )
				{
					ret += *it;
				}
				else // .
				{
					ret += "./";
					ret += *it;
				}
			}
			else
			{
				ret += "/";
				ret += *it;
			}
		}

		return ret;
	}


    std::string Path::getPathMel(bool only_dir, bool as_abs) const
	{
		std::string ret;

        for( auto it = mPathList.begin()
			; it != mPathList.end() 
			; it++ )
		{
			if( *it == *mPathList.rbegin() && only_dir)
			{
				break;
			}

			if(it == mPathList.begin())
			{
                if (mIsAbs || as_abs)
				{
					ret += "|";
					ret += *it;
				}
				else if( *it == ".." )
				{
					ret += *it;
				}
				else // .
				{
					ret += ".|";
					ret += *it;
				}
			}
			else
			{
				ret += "|";
				ret += *it;
			}
		}

        if(mPathList.size() == 0 && this->mIsAbs)
        {
            ret += "|";
        }

        if(mPathList.size() == 1 && only_dir && this->mIsAbs)
        {
            ret += "|";
        }

		return ret;
	}

    Path Path::relative(const char* from_abs_path, const char* to_abs_path, bool from_is_file)
    {
        Path from(from_abs_path);
        Path to(to_abs_path);
        return relative(from,to, from_is_file);
    }
    Path Path::relative(Path from_abs_path, Path to_abs_path, bool from_is_file)
    {
        //����ֵ
        Path p;

        //�����Ǿ���·�����ܼ������ֵ
        if( !from_abs_path.isAbsolutePath() || !to_abs_path.isAbsolutePath() )
        {
            return p;
        }

        //�ҵ���ͬ�ĸ��ڵ�
        int level = -1;
        unsigned int size = std::min(from_abs_path.mPathList.size(),to_abs_path.mPathList.size());
        for (unsigned int i = 0; i < size; i++)
        {
            if( from_abs_path.mPathList[i] == to_abs_path.mPathList[i])
            {
                level = i;
            }
            else
            {
                break;
            }
        }

        //������ڹ�ͬ·��
        if(level >= 0)
        {
            //���ϵ�
            int end = from_abs_path.mPathList.size() - 1;
            if (from_is_file)
            {
                end = from_abs_path.mPathList.size() - 2;
            }
            for (unsigned int i = level; i < end; i++)
            {
                p.push("..");
            }
        }
        else
        {
            //���ϵ�
            for (unsigned int i = 0; i < from_abs_path.mPathList.size() - 1; i++)
            {
                p.push("..");
            }
        }

        //���µ�
        for (unsigned int i = level + 1; i < to_abs_path.mPathList.size() ; i++)
        {
             p.push( to_abs_path.mPathList[i].c_str() );
        }

        //����
        return p;
    }



}//namespace nbBase


