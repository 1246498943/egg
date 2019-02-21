
#ifndef _EGG_ENGINE_XML_H_
#define _EGG_ENGINE_XML_H_

#include <vector>

#include <rapidxml.hpp>

//����node
typedef rapidxml::xml_node<> EGG_XML_NODE;
typedef rapidxml::xml_attribute<> EGG_XML_ATTRIBUTE;

//����document
class EGG_XML_DOCUMENT
    :public rapidxml::xml_document<>
    //, public osg::Referenced
{
public:
    EGG_XML_DOCUMENT() {}
    virtual ~EGG_XML_DOCUMENT() { /*clearAll();*/ }

    //��ȫɾ��
    //void clearAll()
    //{
    //    this->clear();
    //    buffer.clear();
    //}

    //std::vector<char> buffer;

protected:
private:
};

//�Աȶ���������ǲ���arg1
inline bool compareElementName(EGG_XML_NODE *xml_element, const char* name)
{
    return (strcmp(xml_element->name(), name) == 0);
}

//���ض��������ֵ
inline const char* getElementProperties(EGG_XML_NODE *xml_element, const char* name)
{
    for (EGG_XML_ATTRIBUTE *attr = xml_element->first_attribute(name)
        ; attr
        ; attr = attr->next_attribute(name))
    {
        return (attr->value());
    }
    //return (xml_element->first_attribute(name)->value());

    return "";
}

//��ѯһ�����������ֵ�ǲ���arg2
inline bool compareAttributeName(EGG_XML_NODE *xml_element, const char* attr_name, const char* v_name)
{
    return (strcmp(getElementProperties(xml_element, attr_name), v_name) == 0);
}

//��������
inline const char* getContent(EGG_XML_NODE *xml_element)
{
    EGG_XML_NODE *child = xml_element->first_node();
    if (child->type() == rapidxml::node_data)
    {
        return (child->value());
    }
    if (child->type() == rapidxml::node_cdata)
    {
        return (child->value());//���ﻹ��Ҫ���Կ����
    }

    return "";
}

#endif // !_EGG_ENGINE_XML_H_
