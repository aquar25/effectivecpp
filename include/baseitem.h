#ifndef BASEITEM_H
#define BASEITEM_H

#include <vector>
#include "utility.h"

const char* const FileName = "BaseItem.h";
const std::string FileExtName(".h");

class UnCopyable
{
protected:
    UnCopyable() {}  //允许派生类构造和析构
    ~UnCopyable() {}
private:
    UnCopyable(const UnCopyable&); //只是声明一个私有的拷贝构造，避免子类中可以拷贝
    UnCopyable& operator=(const UnCopyable&);
};

class UniqueClass : private UnCopyable
{
    // 子类中不需要再声明私有的拷贝构造或拷贝赋值函数，
    // 因为编译器自动生成的版本会调用基类的对应私有函数而导致编译失败
};

class BaseItem
{
    public:
        BaseItem();
        //explicit BaseItem(int id);
        BaseItem(int id);
        virtual ~BaseItem();
        BaseItem(const BaseItem& other);
        BaseItem& operator=(const BaseItem& other);

        const unsigned int& GetID() const { return m_nID; }
        unsigned int& GetID()
        {
            return const_cast<unsigned int&>(// 将返回值转为non-const
                static_cast<const BaseItem&>(*this).GetID() // 将引用this转换为const类型，才能调用const成员函数
            );
        }
        void SetID(unsigned int val) { m_nID = val; }
        char* GetName() { return m_szName; }
        void SetName(const char* val);

        void Print();


    protected:

    private:
        static const int MAX_LEN = 20;
        //static const float PIE = 3.14; // error, 不能在类内初始化静态成员
        static const float PIE;
        unsigned int m_nID;
        char m_szName[MAX_LEN];
};

class Game: public BaseItem
{
public:
    Game();
    ~Game();

    Game(const Game& rhs);
    Game& operator=(const Game& rhs);
private:
    int type;
};


class Container
{
private:
    class Transformer
    {

    public:
        Transformer();
        ~Transformer();

        void transform(BaseItem* item);
    };

public:
    Container();
    ~Container();

    void addItem(BaseItem* item);
    void doTransform();

private:
    DISALLOW_COPY_AND_ASSIGN(Container);

private:
    std::vector<BaseItem* > m_vItems;
    Transformer m_transformer;
};

#endif // BASEITEM_H
