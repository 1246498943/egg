#ifndef _TEST_BASE_CLASS_H_
#define _TEST_BASE_CLASS_H_

#include <set>
#include <vector>
#include <map>


enum OBJECTTYPE
{
    OC_AAAA = 0
    , OC_BBBB
    , OC_CCCC
};

class BBBB;

class AAAA
    :public egg::Object
{
public:
    AAAA() 
    {
        type = OC_AAAA;
    
        //ref = new AAAA();
    }
    ~AAAA() {}

    //ֵ����
    int id = 0;
    std::string name;

    //ö������
    OBJECTTYPE type;

    //��������
    std::vector<int> nums_vector;
    std::map<int, int> nums_map;
    std::vector<AAAA*> children;
    //std::map<std::string, CLRPTR<AAAA> > library;
    std::map<std::string,BBBB*> library;


    //����������
    std::set<int> nums_set;

    //������������
    AAAA* ref = 0;


    //����
    void exec()
    {
    }

private:

};

class BBBB
    :public AAAA
{
public:
    BBBB() { type = OC_BBBB; }
    ~BBBB() {}

    std::string nameB;


    void exec()
    {
    }
private:

};

class CCCC
    :public BBBB
{
public:
    CCCC() { type = OC_CCCC; }
    ~CCCC() {}

    std::string nameC;


private:
    void exec()
    {
    }
};


#endif // !_TEST_BASE_CLASS_H_

