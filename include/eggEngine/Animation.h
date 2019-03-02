#ifndef NIUBI_SG_ANIMATION_H
#define NIUBI_SG_ANIMATION_H

#include <eggEngine/Resource.h>

#include <egg/Math.h>
#include <eggEngine/Matrix.h>

namespace egg
{

    /*
    * Animation
    */

    class EGG_ENGINE_EXPORT Animation
        :public Resource
    {
    public:

        Animation() {}
        virtual ~Animation() {}

        virtual std::string ObjectClassName() override { return "animation"; }

        //��д����
        virtual void save(Path filename, std::ostream& os, Serializer* ser) const override;
        virtual void load(Path filename, std::istream& is, Serializer* ser, ObjectManager* om, Referenced* rmg, bool load_ext) override;

        //��ѯ��������
        //unsigned int getSize(){ return mIndex.size(); }
        std::map< std::string, unsigned int >& getIndex() { return mIndex; }
        std::vector< std::vector< Eigen::Matrix4f > > &   getData() { return mData; }

        //��ȡ���ܵ�frame����
        //  ���յ���Ӧ���ǵȳ��ģ�����Ҳ�����������
        unsigned int getFrameNum();

        //��ѯ��ֵ���
        Eigen::Matrix4f interpolation(std::string name, float frame);

    private:

        //���ƶ�ӦID��
        //   ID�Ǵ�0��ʼ˳��ı�
        std::map< std::string, unsigned int > mIndex;

        //ID��Ӧ�Ķ�������
        //   ��ʱ�ȴ洢������Ϣ mat44 �Ժ��ٿ���ѹ����SRT����
        std::vector< std::vector< Eigen::Matrix4f > > mData;
    };


}//namespace egg

#endif//NIUBI_SG_ANIMATION_H
