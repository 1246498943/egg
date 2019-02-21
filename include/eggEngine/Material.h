#ifndef NIUBI_SG_MATERIAL_H
#define NIUBI_SG_MATERIAL_H

#include <eggEngine/Export.h>

#include <map>

//#include <eggBase/Public.h>

#include <eggEngine/Resource.h>
//#include <eggEngine/Program.h>
//#include <eggEngine/Texture.h>
#include <eggEngine/Pass.h>


/*
* Material
*   һ����Դ��¼�ļ�
*   ��Ҫ���һ��effect��ʵ��������
*   ��Ҫ������tech�¹���pass  ÿ��pass������Ӧ����Ⱦ״̬��uniform��ֵ
*   ���ھ���shader�Ķ��� ���ṩһ��IO����
*/




#define NB_DEFAULT_THCH_NAME "default"


namespace egg
{

    /*
     *   �������ʺ�Ĭ������һ������
     *       tech "default"
     *       pass 0
     *       shader���� NB_SHADER_TYPE_ONLY_VERTEX ֻ�ж�������
     *       (Ĭ��ֻ�Ǳ�֤�˼��������ܱ���Ⱦ���� ���ڹ۸���� ����֤)
     */
    class EGG_ENGINE_EXPORT Material
        :public Resource
    {
    public:

        //Tech����
        //typedef std::vector<Pass> Tech;
        class Tech
            :public Object
        {
        public:
            Tech() {}
            virtual ~Tech() {}

            std::vector< egg::ref_ptr< Pass > > mPasses;
        };


        Material();
        virtual ~Material();

        //virtual std::string getExt(){return "material";}
        virtual std::string ObjectClassName() override { return "material"; }

        //��ǰtech
        std::string getCurrentTech() { return mCurrentTech; }
        void setCurrentTech(std::string tech) { mCurrentTech = tech; }

        //tech����
        std::vector<std::string > getTechNames();
        Tech* getOrCreateTech(std::string tech_name = NB_DEFAULT_THCH_NAME);
        Tech* getTech(std::string tech_name = NB_DEFAULT_THCH_NAME);
        void removeTech(std::string tech_name = NB_DEFAULT_THCH_NAME);
        void removeTech(Tech* t);


        //�ռ��Ѿ����ص���Դ����
        virtual void collectionResourceObject(std::map< egg::Path, egg::ref_ptr< Resource > >& result) override;

        //�ռ���δ���ص���Դ·��
        virtual void collectionResourceLink(std::vector< egg::ref_ptr< ResourceLink > >& result) override;

        virtual void save(egg::Path filename, std::ostream& os, Serializer* ser) const override;
        virtual void load(egg::Path filename, std::istream& is, Serializer* ser, ObjectManager* om
            , egg::Referenced* rmg, bool load_ext) override;

    private:

        //��ǰtech
        std::string mCurrentTech;

        //Tech
        std::map<std::string, egg::ref_ptr<Tech> > mTechs;

        //����VBO����ӳ���ϵ
        //std::map<std::string , unsigned int > mAttrbuteIDs;

    };

}//namespace nbSG

#endif//NIUBI_SG_MATERIAL_H
