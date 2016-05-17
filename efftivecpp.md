###Effective Cpp Notes
* 书中将所有类型的变量统称为对象，而int byte char类型统称为整型类型

常见的存储区域可分为：
1、栈
由编译器在需要的时候分配，在不需要的时候自动清楚的变量的存储区。里面的变量通常是局部变量、函数参数等。
2、堆
由new分配的内存块，他们的释放编译器不去管，由我们的应用程序去控制，一般一个new就要对应一个delete。如果程序员没有释放掉，程序会一直占用内存，导致内存泄漏，在程序结束后，操作系统会自动回收。
3、自由存储区
由malloc等分配的内存块，它和堆是十分相似的，不过它是用free来释放分配的内存。
4、全局/静态存储区
全局变量和静态变量被分配到同一块内存中，在以前的C语言中，全局变量又分为初始化的和未初始化的，在C++里面没有这个区分了，他们共同占用同一块内存区。在采用段式内存管理的架构中，BSS段（bss segment）通常是指用来存放程序中未初始化的全局变量的一块内存区域。BSS是英文Block Started by Symbol的简称。BSS段属于静态存储区。
5、常量存储区
这是一块比较特殊的存储区，他们里面存放的是常量，不允许修改（当然，你要通过非正当手段也可以修改）。
6、static全局变量与普通全局变量有什么区别？
答：static全局变量和普通全局变量存储区域相同，不同的是：
static全局变量只在声明此static全局变量的文件中有效；
普通全局变量对整个源程序都有效，当此源程序包含多于一个文件的程序时，对其他文件依然有效。

7、static局部变量与普通局部变量的区别？
答：static局部变量的存储区为静态存储区，普通局部变量的存储区为栈；
static局部变量生存周期为整个源程序，但是只能在声明其的函数中调用，并且其值与上一次的结果有关；而普通局部变量的生存周期为声明其函数的周期，超过特定的范围其值会被重新初始化；
static局部变量如果未初始化其值默认为0，而普通局部变量则不确定。

####类型转换
* 尽量在类的构造方法前加上explicit，可以避免不必要的隐式类型转换。例如类定义如下
    ```
    #ifndef BASEITEM_H
    #define BASEITEM_H

    const char* const FileName = "BaseItem.h";
    const std::string FileExtName(".h");

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
            void SetName(char* val);
        protected:

        private:
            static const int MAX_LEN = 20;
            //static const float PIE = 3.14; // error, 不能在类内初始化静态成员
            static const float PIE;
            unsigned int m_nID;
            char m_szName[MAX_LEN];
    };

    #endif // BASEITEM_H

    ```

使用过程中，ShowItem(34);会调用参数为整数的方法，但如果没有这个方法，则会调用`void ShowItem(BaseItem item)`函数，将34通过隐式类型转换为BaseItem，如果将构造方法`BaseItem(int id);`加上前缀explicit，则不会调用该构造方法，因为它要求显式的调用。
    ```
    void ShowItem(BaseItem item)
    {
        cout<<"Item id: "<<item.GetID()<<endl;
    }

    void ShowItem(int data)
    {
        cout<<"Show integer: "<<data<<endl;
    }

    int main()
    {
        cout << "Hello world!" << endl;
        ShowItem(34);
        getchar();
        return 0;
    }
    ```

####拷贝构造
* 不是所有的`=`出现的地方都会调用拷贝赋值方法，当在定义一个对象时,一定会有构造方法调用，即使有=，也是调用的拷贝构造方法。
```
BaseItem item1(1);
BaseItem it2 = item1; // 定义对象it2，只调用了拷贝构造
it2 = item1;          // 对已经定义过的对象赋值操作
```
* 拷贝构造方法非常重要，因为*当值传递一个对象时，会调用类的拷贝构造方法

####函数对象
* 实现了operator()的类对象

####函数参数
* 对于基本数据类型，使用值传递要比使用引用传递更有效率

####使用const定义常量
* 使用const替换#define：
    1. 增加编译检查
    2. 宏定义只是简单替换，对于浮点数，会在代码文件中出现多次，常量只会在代码表中定义一次
    3. 调试信息中可以看到常量的定义，对于宏只能看到替换后的一个数字，没法调试

* 由于常量一般在头文件中定义，并被多个文件访问，因此对于执行类型的常量，需要对指针自身声明为常量。例如`const char* const FileName = "BaseItem.h";`，少一个const都会存在问题,因此最好使用`const std::string FileExtName(".h");`来定义常量字符串

* 类专属常量
通常需要将类的常量声明为静态的，确保该常量只有一个。对于整数类型常量，可以在类中声明的同时初始化一个值，但这个语句依旧是声明语句。对于其他类型的静态常量，则需要在类的cpp文件中进行定义。
如果要访问一个类专属常量的地址或者编译器强制要求有定义式，此时只能通过在类的实现文件中进行定义，即使是整型常量。

```
class BaseItem
{
    private:
        static const int MAX_LEN = 20;
        //static const float PIE = 3.14; // error, 不能在类内初始化静态成员
        static const float PIE;
        unsigned int m_nID;
        char m_szName[MAX_LEN];
};
// 在cpp实现文件中定义浮点数PIE
const float BaseItem::PIE = 3.14;

```

通常C++要求使用的任何东西都有定义，但是对于类中的静态整型常量，则可以不用定义，因为在常量的声明时已经有初始化值。

* 使用inline代替#define定义函数宏，一个极端的例子
    ```
    #define MAX_VALUE(a, b) ShowItem((a) > (b) ? (a) : (b))

    int a = 5, b = 0;
    MAX_VALUE(++a, b); // a 增加两次
    MAX_VALUE(++a, b+10); // 由于条件判断没有选择a，只增加一次

    // 推荐写法
    template<typename T>
    inline void MaxValue(const T& a, const T& b)
    {
        ShowItem(a > b ? a : b);
    }
    ```

####const使用
* const 出现在*左边，说明指向的数据是常量，如果出现在*右边，说明指针自身是常量
```
char name[] = "Tom";
const char* p = name;  // non-const pointer, const data
char* const poi = name; // const pointer, non-const data
const char* const data = name; // const pointer, const data
```
* const在数据类型的前面或后面效果一样
* 声明迭代器为const，就像声明指针为const一样，迭代器不能指向其他值，但是这个迭代器所指向的值是可以改变的，而const_iterator指向的值是不可改变的
    ```
    vector<int> vec;
    vec.push_back(14);
    const vector<int>::iterator iter = vec.begin();
    *iter = 15;
    iter++;  //error, 

    vector<int>::const_iterator iter = vec.begin();
    *iter = 15; // error, read-only 
    iter++;
    ```
* 尽可能多的使用const声明，包括函数参数/函数返回值，这样可以避免不必要的错误，例如错误的赋值语句在编译时就会被发现
* 编译器强制实施bitwise constness，但你编程时应使用“概念上的常量性”
* 当const 和 non-const成员函数有等价的实现时，用non-const版本调用const版本避免代码重复。
* const成员函数 （在成员函数的声明后面增加const修饰，不是在函数的返回值前增加const修饰，后者修饰的是返回值）
const成员函数可以作用于const对象，而在函数参数传递时，会使用pass by reference to const的方式来传递对象，来提高代码效率，此时就要用const成员函数来访问传入的const对象引用。
* 两个成员函数如果只是常量属性的不同，也是可以被重载的，根据调用函数的属性不同，常量对象会调用常量成员函数
* 常量成员函数不能修改成员变量的值，如果必须要修改，需要将成员变量使用mutable来修饰

* 确保对象在使用前被初始化，在很多情况下一个未初始化的对象的值是未定义（未定义是C++中一个特殊的状态)
* 总是使用构造函数的初始化列表来初始化成员变量，不要在构造函数中使用赋值语句对成员变量进行初始化，因为在初始化列表中执行的是每个成员变量的默认构造方法（或拷贝构造方法），而在构造方法中使用赋值语句进行初始化，则执行的是拷贝赋值操作，C++会先调用成员的构造方法初始化，再调用该成员的拷贝赋值对该成员进行赋值操作。例如成员变量是string类型，就会先调用string的构造方法，再调用string的拷贝赋值，导致效率降低。
* 初始化列表中，基类的总是先于派生类构造方法先执行，成员变量总是以其声明的顺序进行初始化，与它在初始化列表中的顺序无关，因此当一个成员变量的初始化依赖于另一个时，一定要在它的后面声明。
* 静态对象：生命周期从构造出来直到应用程序结束，它的析构会在main()结束时自动调用。包括全局对象/定义在namespace中的对象/在class,function,file作用域内被声明为static的对象。其中在函数体内的static对象称为local static对象。在不同的编译单元(一个cpp文件，编译成独立的obj文件)中定义的静态对象的初始化顺序是未定义的，例如在a.cpp中类Phone定义了static string name，而在b.cpp文件中定义的PhoneBook类中定义static string type,这两个对象的初始化顺序是未定义的，因此如果存在依赖关系的话，就会出现问题。可以通过使用函数将static对象转换为local static对象，通过执行静态对象所在的函数来控制初始化顺序，而该函数直接返回静态对象的引用即可。事实上，singleton模式就是一个这样处理的典型。同时通过放到函数体内，如果函数没有被执行到，也就不会触发对象的构造，一定程度上提高效率。但是这个方法要求必须在单线程中才有效，多线程下不同的线程执行该方法的次序也是未知的。如果必须要初始化静态对象，可以在同一个线程中，将有依赖关系的静态对象的包装函数依次执行，保证有序性。

* 如果自己没有定义，编译器会自动声明一个copy构造/copy assignment/一个析构/一个默认构造函数，所有这些方法都是public & inline的。
* 当基类中有virtual函数时，子类中编译器自动声明的析构函数才是virtual，否则默认也是没有virtual.
* 编译器自动生成的拷贝构造和拷贝赋值函数，只会将源对象的每一个non-static成员变量拷贝的目标对象中。
* 当类中包含引用类型成员/const成员，编译器不会生成默认的拷贝构造或拷贝赋值函数，因为引用类型的成员不能改动指向，const成员的值也不能修改。如果基类的拷贝赋值操纵为private，编译器也不会为派生类自动生成拷贝赋值函数，因为派生类不能调用基类中的私有成员函数。
* 通过在类中**声明**私有的拷贝构造和拷贝赋值函数，可以避免用户误使用编译器自动产生的这两个函数，为了避免成员函数或friend函数访问这两个私有函数，可以只是声明这两个私有函数，而不实现，这样链接时会提示链接错误。
* 通过定义UnCopyable基类来组织拷贝操作的执行，这样不用在每个子类中都去写私有的拷贝构造声明
    ```
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
    ```

* 当派生类对象经由一个基类的指针delete，而该基类的析构函数是non-virtual，其结果未定义。通常情况下，派生类的部分没有被销毁。
* 任何类只要有virtual函数，说明它是要作为基类的，析构函数就应该是virtual的
* 如果一个类不会作为基类或不会具有多态性质，则没有必要将析构函数设置为virtual，因为这样会增加额外的虚函数表信息。其中的vptr指向一个虚表，当调用virtual函数时，编译器在虚表中寻找合适的函数指针。而这个指针在32位系统中占用4个字节，而在64bit系统中指针占64bits。而且也不能和其他语言互通。因为其他语言如C中没有这个虚表指针。
* std::string/vector/list/set都是non-virtual析构的类，因此继承这些类，在delete指向子类对象的基类指针时都会出现子类资源无法释放的情况，需要避免
* 析构函数的调用次序：先调用最底层的派生类析构，在派生类的析构中调用基类的析构
* 当没有纯虚函数而又想将类定义为抽象类，则可以将析构函数定义为纯虚函数，但是必须给这个纯虚析构函数增加定义，否则它子类析构函数调用到它的析构函数时会出现未定义。

* 别让异常逃离析构函数
当vector离开作用范围调用元素的析构函数时，如果第一个元素的析构出现异常，程序会出现未定义的错误，导致后续的元素的得不到释放
例如有个数据库连接管理类用来管理数据库资源的释放，再其析构函数中可能出现数据库关闭失败的异常解决方案：
* 捕获异常，记下日志，结束程序
* 捕获异常，记下日志，让程序继续执行
* 如果客户需要对某个操作函数运行期间抛出的异常做出反应，那么类应该提供一个普通函数，而不是在析构函数中执行该操作。此例中可以给客户提供一个关闭数据库的接口，这样用户自己可以处理异常情况。
    ```
    // Database connection manager object
    class DBManager
    {
        DBManager() {};

        ~DBManager()
        {
            // 自动释放数据库资源，但有可能出现异常
            try
            {
                db.close();
            }
            catch(dbexception)
            {
                // write some log info

                // abort or ignore the exception
                std::abort();
            }
        };

    private:
        DBConnection db;
    };
    ```
* 绝不要在构造和析构函数中调用virtual函数
在执行子类的构造函数时，先执行了基类的构造函数，在基类的构造中调用了虚函数只能调用到基类的虚函数(此时运行时信息还是一个基类的对象)，而实际目的创建的是子类的对象。
* 需要注意我们通常会在不同的构造方法中调用init()方法类初始化类，而这个方法中可能调用到virtual方法，导致构造方法中调用了virtual方法却很难发现。

* 为了实现“连锁赋值”，赋值操作符必须返回一个指向操作符左侧实参的引用。
* 在=的实现中处理自我赋值，因为如果在方法内可能对对象中的成员进行操作，而操作的这些操作进而影响=右侧传入参数，导致异常。同时还可以避免低效的复制操作。
    ```
    BaseItem& BaseItem::operator=(const BaseItem& rhs)
    {
        // this是=左侧的对象指针
        cout<<"assignment ..."<<endl;
        if (this == &rhs) return *this; // handle self assignment
        //assignment operator
        // 返回指向=左侧对象的引用，从而实现连锁赋值
        return *this;
    }
    ```

* 为派生类实现拷贝构造和=函数时，注意复制其基类的成分，由于基类的成分一般是私有的，应该生让派生类的拷贝函数调用相应的基类的拷贝函数
* 不能偷懒在=中使用拷贝构造函数，反之也不行，=只能作用于一个已经初始化的对象上。含义不同，一个赋值一个是创建一个对象。如果真需要复用代码，可以再定义一个函数来在二者中复用。
    ```
    // Game继承自BaseItem，自己有一个成员是type
    Game::Game(const Game& rhs)
    :BaseItem(rhs),//调用基类的拷贝构造
    type(rhs.type) // 子类本地成员
    {
        cout<< "Game copy constructor" << endl;
    }

    Game& Game::operator=(const Game& rhs)
    {
        cout<< "Game's = function..." << endl;
        if (&rhs == this) return *this;
        BaseItem::operator=(rhs); //调用基类的赋值
        type = rhs.type;  //子类本地成员
        return *this;
    }
    ```

* 以对象管理资源
在使用对象时可能存在在执行delete语句之前函数已经return，或者continue/exception跳出，导致资源释放语句得不到执行。通过使用heap上的对象变量在函数退出时会自动执行对象析构方法，而在管理对象中实现对分配的内存资源的释放。智能指针std::auto_ptr（头文件<memory>）就是通过这个原理在其析构函数中对它指向的对象执行了delete方法（但是不会执行delete[]方法，因此不能用智能指针指向数组指针）。通常也可以参考std::auto_ptr通过自己实现一个资源管理类，定制化的实现资源的有效释放。
    ```
    void ManageObjects(int count)
    {
        BaseItem* pItem = new BaseItem();
        //....
        if(0 == count)
        {
           return ;
        }

        delete pItem; //可能不会得到执行
    }
    ```
使用智能指针std::auto_ptr的拷贝和赋值比较特殊，在进行拷贝或赋值后，来源的指针会自动被设置为null，而只有新的指针唯一指向内存资源。但是RCSP(reference-counting smart pointer)引用计数指针持续追踪有多少指针指向资源，在没有人指向资源时释放资源类似garbage collection.std::tr1::shared_ptr就是一个这样的实现(头文件是<tr1/memory>)，但是一样只是执行delete，不能删除数组资源，因为对于动态数组，C++建议使用vector而不是array
    ```
    void ManageObjects(int count)
    {
        std::auto_ptr<BaseItem> pItem(new BaseItem());
        std::auto_ptr<BaseItem> pItem2(pItem); // pItem2指向对象，pItem=null
        pItem = pItem2;  // pItem指向对象，pItem2=null
        //....
        if(0 == count)
        {
           return ;
        }
    }

    void ManageObjects(int count)
    {
        std::tr1::shared_ptr<BaseItem> pItem(new BaseItem(5));
        std::tr1::shared_ptr<BaseItem> pItem2(pItem); // pItem2指向对象，pItem也指向对象，引用计数为2
        pItem = pItem2;  // pItem2指向对象，pItem也指向对象
        pItem2->Print();
        pItem->Print();
        //....
        if(0 == count)
        {
           return ;
        }
    }
    ```
* 在资源管理类中小心处理copying行为
    ```
    class Locker
    {
    public:
        explicit Locker(Mutex* pm):mutexPtr(pm)
        {
            lock(mutexPtr); // 获得资源
        };
        ~Locker()
        {
            unlock(mutexPtr); //释放资源
        };
    private:
        Mutex* mutexPtr;
    };

    // 使用时
    Mutex pm; //定义互斥器
    {
        Locker locker(&pm)//建立一个区块定义crtical section
        // 执行互斥操作

    }// 在退出区块后，自动解除锁
    ```
* 禁止复制：Locker继承UnCopyable类
* 深拷贝资源管理类中的资源，例如标准的字符串类，每次都是深拷贝指向的字符串资源
* 转移资源的拥有权，类似auto_ptr
* 使用引用计数:借助std::tr1::shared_ptr<Mutex>实现引用计数
    ```
    class Locker
    {
    public:
        explicit Locker(Mutex* pm)
        :mutexPtr(pm, unlock)  // 初始化成员，并以unlock作为share_ptr的删除器
        {
            lock(mutexPtr.get()); // 获得资源
        };
        ~Locker()
        {
            //释放资源，自动调用成员变量shared_ptr的析构函数，
            // shared_ptr的析构函数会在引用计数为0时,自动调用删除器，这里是unlock
        };
    private:
        // shared_ptr的构造函数第二个参数是在引用计数为0时执行的删除操作
        std::tr1::shared_ptr<Mutex> mutexPtr;
    };
    ```

* 在资源管理类中提供对原始资源的访问。例如auto_ptr和shared_ptr都提供了get()方法来获取它实际的指针。作者提到了通过实现隐式转换函数来避免直接调用get()方法带来的不变，但这个方法容易造成错误的隐式转换调用。
    ```
    operator FontHandle() const //隐式类型转换函数，将当前类转换得到FontHandle类型的对象
    {
        return handle;
    }
    ```
* 成对的调用new和delete，否则可能对于[]只是释放了第一个，而对于一个对象，则是未定义的行为
* 以独立语句将newed对象置入只能指针
例如以下语句
`processData(std::tr1::shared_ptr<Widget>(new Widget), priority());`
在C++传入参数的的执行顺序是不确定的，可能是1. new Widget 2.priority() 3.std::tr1::shared_ptr<Widget>构造，如果2.出现了异常，导致智能指针还没有包含new出来的资源，会导致内存泄漏。因此最后还是使用独立的语句得到函数的参数，以及智能指针，确保资源能被智能指针正确的管理。





