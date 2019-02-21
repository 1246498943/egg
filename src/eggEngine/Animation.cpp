
#include <eggEngine/Animation.h>

using namespace egg;

void Animation::save(Path filename, std::ostream& os, Serializer* ser) const
{
    Resource::save(filename, os, ser);

    //д������mIndex
    ser->writeNumber(os, mIndex.size()); //ͨ����
    for (auto it = mIndex.begin(); it != mIndex.end(); it++)
    {
        ser->writeString(os, it->first);   //ͨ������

        ser->writeNumber(os, it->second);  //ID
    }

    //д������mData
    ser->writeNumber(os, mData.size()); //ͨ����
    for (auto it = mData.begin(); it != mData.end(); it++)
    {
        ser->writeNumber(os, (*it).size()); //ͨ���ľ�������

        for (unsigned int i = 0; i < (*it).size(); i++)
        {
            for ( int mi = 0; mi < 16; mi++) //д�����
            {
                ser->writeNumber(os, ((*it)[i])(mi)); //��ǰ����
            }
        }
    }

}

void Animation::load(Path filename, std::istream& is, Serializer* ser
    , ObjectManager* om, Referenced* rmg, bool load_ext)
{
    Resource::load(filename, is, ser, om, rmg, load_ext);

    //������mIndex
    int index_count = 0;
    ser->readNumber(is, index_count); //ͨ����
    for (int i = 0; i < index_count; i++)
    {
        std::string index_name;
        ser->readString(is, index_name); //ͨ������

        int index_id = 0;
        ser->readNumber(is, index_id);  //ID

        mIndex[index_name] = index_id;
    }

    //������mData
    int data_count = 0;
    ser->readNumber(is, data_count); //ͨ����
    for (int i = 0; i < data_count; i++)
    {
        int data_len = 0;
        ser->readNumber(is, data_len); //��ǰͨ��Ԫ������

        mData.push_back(std::vector<Eigen::Matrix4f>(data_len));
        for (int j = 0; j < data_len; j++)
        {
            for (unsigned int mi = 0; mi < 16; mi++) //��ȡ����
            {
                ser->readNumber(is, mData[i][j](mi)); //��ǰ����
            }
        }
    }

}


unsigned int Animation::getFrameNum()
{
    unsigned int ret = 0;
    for (unsigned int i = 0; i < mData.size(); i++)
    {
        if (ret < mData[i].size()) ret = mData[i].size();
    }
    return ret;// / 16;
}

Eigen::Matrix4f Animation::interpolation(std::string name, float frame)
{
    unsigned int a = (unsigned int)(frame);       //��ֵA��
    unsigned int b = (unsigned int)(frame) + 1;   //��ֵB��
    float i = frame - a; //��ֵ����

    //printf("    %d %d\n",a,b);
    //printf("    %f\n",i);

    auto it = mIndex.find(name);
    if (it != mIndex.end())
    {
#if 0
        if (it->second < mData.size() && b * 16 < mData[it->second].size())
        {

            vmml::mat4f mat_a;
            vmml::mat4f mat_b;
            mat_a.set(&mData[it->second][a * 16], &mData[it->second][a * 16 + 15], false);
            mat_b.set(&mData[it->second][b * 16], &mData[it->second][b * 16 + 15], false);

            //���Բ�ֵ
            vmml::mat4f mat_i = mat_a + (mat_b - mat_a) * i;
            //vmml::mat4f mat_i = mat_a;
            return mat_i;
        }
#else

        if (it->second < mData.size() && b < mData[it->second].size())
        {
            return mData[it->second][a]; //Ŀǰ�Ȳ�ʵ�ֲ�ֵ
        }
#endif
        
    }

    //��Ч����
    return Eigen::Matrix4f::Identity();
}


