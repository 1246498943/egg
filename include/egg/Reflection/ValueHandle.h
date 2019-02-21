#ifndef _CLR_ValueHandle_H_
#define _CLR_ValueHandle_H_

#include <egg/Export.h>

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <sstream>

#include <egg/ref_ptr.h>
#include <egg/Object.h>

namespace egg
{
    template<typename T>
    std::string numToString(T num)
    {
        std::ostringstream oss;
        oss << num;
        return oss.str();
    }


    enum ValueHandleValueType
    {
        //ֵ����
        VHVT_Value

        //���ö���
        , VHVT_Ref

        //ö��
        , VHVT_Enum
    };

    enum ValueHandleSetType
    {
        //��ֵ
        VHST_Single

        //���ж���
        , VHST_Vector

        //�����Ķ���
        , VHST_Map

        //�������Ķ���(������������set��֧��)
        //, VHST_Set
    };

    class EGG_RUNTIME_EXPORT ValueHandle
        :public Referenced
    {
    public:
        ValueHandle(std::string sourceName = "", std::string attrName = ""
            , ValueHandleValueType value_type = VHVT_Value, ValueHandleSetType set_type = VHST_Single
        )
            :mValueType(value_type)
            , mSetType(set_type)
            , mSourceType(sourceName)
            , mName(attrName)
            , mCodec("ascii")
        {
        }
        virtual ~ValueHandle()
        {
        }

        ValueHandleValueType getValueType() { return mValueType; }
        ValueHandleSetType getSetType() { return mSetType; }
        std::string getName() { return mName; }

        //��������
        const std::string getDataType() { return mSourceType; }
        const std::string getDataCodec() { return mCodec; }
        void setDataCodec(const std::string& str) { mCodec = str; }

        //ȡֵ����
        std::function<std::string(Object*)> getDataString;
        std::function<void(Object*, std::string)> setDataString;

        //���ö������
        //CLROBJECT* getObject(CLROBJECT* object) { return 0; }
        //void setObject(CLROBJECT* object, CLROBJECT* attr_object) {}
        std::function<Object*(Object*)> getObject;
        std::function<void(Object*, Object*)> setObject;



        //���ж��� �������� ͨ��
        //int getDimension() { return 0; }
        //int getDimensionSize(int d) { return 0; }
        //void remove() {}
        //void clear() {}
        std::function<int(Object* object)> getDimension;
        std::function<int(Object* object, int dim)> getDimensionSize;
        std::function<void(Object* object, int dim)> remove;
        std::function<void(Object* object)> clear;


        //���ж���
        //void setIndex(int dim, int index) {}
        //void push(int dim, std::string str) {}
        std::function<void(Object* object, int dim, int index)> setIndex;
        std::function<void(Object* object, int dim, std::string str)> pushDataString;
        std::function<void(Object* object, int dim, Object* attr_object)> pushObject;
        //std::function<void(CLROBJECT* object, int dim, int index)> removeIndex;


        //��������
        //std::vector<std::string> getKeys(int d) { return std::vector<std::string>(); }
        //void setKey(int dim, std::string key) {}
        //void insert(int dim, std::string key, std::string str) {}
        std::function<std::vector<std::string>(Object* object, int dim)> getKeys;
        std::function<void(Object* object, int dim, std::string key)> setKey;
        std::function<void(Object* object, int dim, std::string key, std::string str)> insertDataString;
        std::function<void(Object* object, int dim, std::string key, Object* attr_object)> insertObject;
        //std::function<void(CLROBJECT* object, int dim, std::string key)> removeKey;


        //�������Ķ���(������������set��֧��)
        //�ϼ�����
        //void begin() {}
        //void end() {}
        //void next() {}
        //std::function<void(void)> begin;
        //std::function<void(void)> end;
        //std::function<void(void)> next;


    //protected:



    private:
        ValueHandleValueType mValueType;
        ValueHandleSetType mSetType;
        std::string mSourceType;
        std::string mName;
        std::string mCodec;

    protected:
    public:
        std::vector<int> mIndex;
        std::vector<std::string> mKeys;
    };


}//namespace egg

#endif // !_CLR_ValueHandle_H_
