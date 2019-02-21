#ifndef _EGG_Coord_H_
#define _EGG_Coord_H_

#include<egg/Export.h>
#include<egg/Node.h>
#include<egg/NodeVisitor.h>

namespace egg
{

    //�������Coordinate
    //	ʹ��ʹ�����ֵΪԭ�㣬����ֵΪƫ�����������һ�������������Ϣ
    class Coord
    {
    public:
        Coord(float absolute = 0.0f, float relative = 0.0f)
            :mAbsolute(absolute), mRelative(relative) {}

        float getAbs() { return mAbsolute; }

        float getRel() { return mRelative; }

        float getCoord(float ref)
        {
            return ref * mRelative + mAbsolute;
        }

    private:

        float mAbsolute;
        float mRelative;
    };

#if 0
    class Coord2DNode
        :public egg::Node
    {
    public:

        Coord2DNode(const std::string& name = "node")
            :egg::Node(name) {}
        virtual ~Coord2DNode() {}

        //��ȡ��ǰ������������Ҿ���ֵ
        void getMeAbsLTRB(float& l, float& t, float& r, float& b, float& cx, float& cy);

        //��ʾ����
        void reshape();

    protected:

        //����ֵ
        Coord mX;
        Coord mY;
        Coord mW;
        Coord mH;
        Coord mCX;
        Coord mCY;
        float mRotate;

        //��ǰ��LTRB  ����ڸ��ڵ�ľ�������
        float mRectShape[4];  //���ڵ��LTRB�����ڵ����ĵ�ƫ��
        float mRectPivots[2]; //���ڵ�����ĵ㵽���ڵ����ĵ�ƫ��

    private:

        //��ȡ���ڵ���������Ҿ���ֵ
        void getParentAbsXYWH(float& x, float& y, float& w, float& h, float& cx, float& cy);

        //����
        void reShape(float x, float y, float w, float h);
        float mLastReShape[4];//���һ������

    };
#endif


    class Coord2D
        //:public egg::Node
    {
    public:

        Coord2D()
        {
        }
        virtual ~Coord2D() {}

        //����ֵ
        Coord mX;
        Coord mY;
        Coord mW;
        Coord mH;
        Coord mCX;
        Coord mCY;
        float mRotate;



    };


    class Coord2DInterface
    {
    public:
        Coord2DInterface()
        {
            mRectShape[0] = 0;
            mRectShape[1] = 0;
            mRectShape[2] = 0;
            mRectShape[3] = 0;

            mRectPivots[0] = 0;
            mRectPivots[1] = 0;
            mRectPivots[2] = 0;
            mRectPivots[3] = 0;

            mLastReShape[0] = 0;
            mLastReShape[1] = 0;
            mLastReShape[2] = 0;
            mLastReShape[3] = 0;
        }
        virtual ~Coord2DInterface() {}

        Coord2D& getCoord2D() { return mCoord2D; }

        virtual void onShape(float l, float t, float r, float b, float rot) = 0;

    protected:

        Coord2D mCoord2D;

    public:
        //��ǰ��LTRB  ����ڸ��ڵ�ľ�������

        //���ڵ��LTRB�����ڵ����ĵ�ƫ��
        float mRectShape[4];

        //���ڵ�����ĵ㵽���ڵ����ĵ�ƫ��
        float mRectPivots[2];

        //���һ������
        float mLastReShape[4];

    };

    class Bound
    {
    public:
        Bound(float left = 0, float top = 0, float right = 0, float botton = 0)
            :Left(left), Right(right), Top(top), Botton(botton)
        {}
        float Left;
        float Right;
        float Top;
        float Botton;
    };

    class Coord2DVisitor
        :public egg::NodeVisitor
    {
    public:
        Coord2DVisitor() {}
        virtual ~Coord2DVisitor() {}

        Coord2DInterface* getParent()
        {
            if (np.path.size() > 1)
            {
                Coord2DInterface* parent = dynamic_cast<Coord2DInterface*>(np.path[np.path.size() - 1]);
                if (parent)
                {
                    return parent;
                }
            }
            return 0;
        }

        //����ת��Ϊ�߽�
        Bound Coord2Bound(
            Coord2D x, Coord2D y
            , Coord2D w, Coord2D h
            , Coord2D cx, Coord2D cy
        ) const
        {
            Bound bound;

            return bound;
        }

        //�߽�ת��Ϊ����
        void Coord2Bound(Bound& bound
            , Coord2D& x, Coord2D& y
            , Coord2D& w, Coord2D& h
            , Coord2D& cx, Coord2D& cy
        ) const
        {
        }

        virtual void apply(egg::Node& node)
        {
            Coord2DInterface* coord2d = dynamic_cast<Coord2DInterface*>(&node);
            if (coord2d)
            {
                //testnode->num++;
                Coord2DInterface* parent = getParent();
                if (parent)
                {
                    //�����ȡ���ڵ�� LTRB ����

                    //���㵱ǰ������

                    
                }
                else
                {
                    //û�и��ڵ��ֱ�Ӽ����Լ�������

                    float new_w = coord2d->getCoord2D().mW.getCoord(0);
                    float new_h = coord2d->getCoord2D().mH.getCoord(0);
                    float new_cx = coord2d->getCoord2D().mCX.getCoord(0);
                    float new_cy = coord2d->getCoord2D().mCY.getCoord(0);
                    float new_x = coord2d->getCoord2D().mX.getCoord(0);
                    float new_y = coord2d->getCoord2D().mY.getCoord(0);



                }


                //������Ϊ�Ѿ���������Լ�������


                //ת������Ϊ LTRB

                //���͸��麯��


            }
        }
    };



}

#endif //_EGG_Coord_H_

