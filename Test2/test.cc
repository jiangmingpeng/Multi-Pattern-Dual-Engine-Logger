#include <iostream>

// 单例模式
namespace SingletonMould
{
    /*1.饿汉模式*/
    // template<class T>
    class Singleton
    {
    public:
        void Print()
        {
            std::cout << "Print()" << std::endl;
        }

    public:
        Singleton(const Singleton &) = delete;
        Singleton &operator=(const Singleton &) = delete;
        // static T& GetInstance()
        static Singleton &GetInstance() // 引用放回减少拷贝
        {
            return _instance;
        }

    private:
        Singleton()
        {
            std::cout << "Singleton()" << std::endl; //
        }
        ~Singleton()
        {
        }

    private:
        static Singleton _instance;
    };
    Singleton Singleton::_instance; // 为这唯一个对象开辟空间(这个static的用法要知道--粗略的功能是 改变生命周期 作用域不发生变化)

    /*2.懒汉模式*/
    // template <class T>
    class Singleton1
    {
    public:
        void Print()
        {
            std::cout << "Print()" << std::endl;
        }

    public:
        Singleton1(const Singleton1 &) = delete;
        Singleton1 &operator=(const Singleton1 &) = delete;
        // static T& GetInstance()
        static Singleton1 &GetInstance() // 引用放回减少拷贝
        {
            static Singleton1 _instance;
            return _instance;
        }

    private:
        Singleton1()
        {
            std::cout << "Singleton1()" << std::endl; //
        }
        ~Singleton1()
        {
        }
    };
}

// int main()
// {

//     // Singleton::GetInstance().Print();//在这里其实就是直接拿到了这个类对象了 尚且只能有这一个
//     //Singleton1::GetInstance().Print();//只会在调用时候才会创建对象 属于懒加载的一部分
//     return 0;
// }

// 简单工厂模式--以生产水果为例
#include <memory>
#include <unistd.h>
namespace SingleFactoryMould
{
    class Fruit
    {
    public:
        Fruit() {}
        virtual void show() = 0;
    };

    class Apple : public Fruit
    {
    public:
        Apple() {}
        void show() override
        {
            std::cout << "This is an Apple" << std::endl;
        }
    };

    class Banana : public Fruit
    {
    public:
        Banana() {}
        void show() override
        {
            std::cout << "This is a banana" << std::endl;
        }
    };

    class FruitFactory
    {
    public:
        static std::shared_ptr<Fruit> create(const char *fruitname)
        {
            if (fruitname == "Apple")
            {
                std::shared_ptr<Fruit> fp = std::make_shared<Apple>(); // 注意这个派生类的指针或者引用是可以赋给基类的 只不过可能发生切片
                return fp;
            }
            else if (fruitname == "Banana")
            {
                std::shared_ptr<Fruit> fp = std::make_shared<Banana>();
                return fp;
            }
            else
            {
                return std::shared_ptr<Fruit>();
            }
        }
    };
}
// int main()
// {
//     FruitFactory ff;
//     ff.create("Apple")->show();
//     sleep(1);
//     ff.create("Banana")->show();
// }

// 工厂方法模式
namespace FactoryWays
{
    class Fruit
    {
    public:
        Fruit() {}
        virtual void show() = 0;
    };

    class Apple : public Fruit
    {
    public:
        Apple() {}
        void show() override
        {
            std::cout << "This is an Apple" << std::endl;
        }
    };

    class Banana : public Fruit
    {
    public:
        Banana() {}
        void show() override
        {
            std::cout << "This is a banana" << std::endl;
        }
    };

    class Factory
    {
    public:
        virtual std::shared_ptr<Fruit> create(const char *fruitname = "xxx") = 0;
    };

    class AppleFactory : public Factory
    {
    public:
        std::shared_ptr<Fruit> create(const char *fruitname = "Apple") override
        {
            return std::make_shared<Apple>();
        }
    };

    class BananaFactory : public Factory
    {
    public:
        std::shared_ptr<Fruit> create(const char *fruitname = "Banana") override
        {
            return std::make_shared<Banana>();
        }
    };
}
// int main()
// {
//     AppleFactory af;
//     af.create()->show();
//     sleep(1);
//     BananaFactory bf;
//     bf.create()->show();
//     return 0;
// }

// 抽象工厂模式
#include <string>
namespace TypeFactoryMould
{
    class Fruit
    {
    public:
        Fruit() {}
        virtual void show() = 0;
    };

    class Apple : public Fruit
    {
    public:
        Apple() {}
        void show() override
        {
            std::cout << "This is an Apple" << std::endl;
        }
    };

    class Banana : public Fruit
    {
    public:
        Banana() {}
        void show() override
        {
            std::cout << "This is a banana" << std::endl;
        }
    };

    class Animal
    {
    public:
        virtual void voice() = 0;
    };
    class Lamp : public Animal
    {
    public:
        void voice() { std::cout << "咩咩咩\n"; }
    };
    class Dog : public Animal
    {
    public:
        void voice() { std::cout << "汪汪汪\n"; }
    };

    class Factory
    {
    public:
        virtual std::shared_ptr<Fruit> getFruit(const std::string &name) = 0;
        virtual std::shared_ptr<Animal> getAnimal(const std::string &name) = 0;
    };

    class FruitFactory : public Factory
    {
    public:
        virtual std::shared_ptr<Animal> getAnimal(const std::string &name)
        {
            return std::shared_ptr<Animal>();
        }
        std::shared_ptr<Fruit> getFruit(const std::string &fruitname) override
        {
            if (fruitname == "Apple")
                return std::make_shared<Apple>();
            else if (fruitname == "Banana")
                return std::make_shared<Banana>();
            else
                return std::shared_ptr<Fruit>();
        }
    };

    class AnimalFactory : public Factory
    {
    public:
        virtual std::shared_ptr<Fruit> getFruit(const std::string &name)
        {
            return std::shared_ptr<Fruit>();
        }
        std::shared_ptr<Animal> getAnimal(const std::string &animalname) override
        {
            if (animalname == "Lamp")
                return std::make_shared<Lamp>();
            else if (animalname == "Dog")
                return std::make_shared<Dog>();
            else
                return std::shared_ptr<Animal>();
        }
    };

    // 再来改
    class TypeFactory
    {
    public:
        std::shared_ptr<Factory> create(const std::string &name)
        {
            if (name == "水果")
            {
                return std::make_shared<FruitFactory>();
            }
            else if (name == "动物")
            {
                return std::make_shared<AnimalFactory>();
            }
            else
            {
                return std::shared_ptr<Factory>();
            }
        }
    };
}
// int main()
// {
//     TypeFactory tf1;
//     tf1.create("动物")->getAnimal("Dog")->voice();
//     sleep(1);
//     tf1.create("动物")->getAnimal("Lamp")->voice();
//     TypeFactory tf2;
//     tf2.create("水果")->getFruit("Apple")->show();
//     sleep(1);
//     tf2.create("水果")->getFruit("Banana")->show();
//     return 0;
// }

// 建造者模式---类似生产线的各个部门的继承多态写法
#include <string>
namespace Constructor
{
    class Computer
    {
    public:
        using Ptr = std::shared_ptr<Computer>;
        Computer() {}
        void setBoard(const std::string &board) { _board = board; }
        void setDisplay(const std::string &display) { _display = display; }
        virtual void setOs() = 0;
        void PrintInfo() // 打印配置信息
        {
            std::string Info;
            Info = _board + _display + _os;
            std::cout << "\tBoard: " << _board << std::endl;
            std::cout << "\tDisplay: " << _display << std::endl;
            std::cout << "\tOs: " << _os << std::endl;
        }

    protected:                // 继承之后可在类内访问
        std::string _board;   // 主板
        std::string _display; // 显示器
        std::string _os;      // 操作系统
    };

    class MacBook : public Computer
    {
    public:
        using Ptr = std::shared_ptr<MacBook>;
        MacBook() {}
        void setOs() override
        {
            _os = "Mac_Os_x12.0";
        }
    };

    class Builder
    {
    public:
        using Ptr = std::shared_ptr<Builder>;
        virtual void BuildBoard(const std::string &boardname) = 0;
        virtual void BuildDisplay(const std::string &displayname) = 0;
        virtual void BuildOs() = 0;
        virtual Computer::Ptr Build() = 0; // 做好了返回电脑指针
    };

    class MacBookBuilder : public Builder
    {
    public:
        using Ptr = std::shared_ptr<MacBookBuilder>;
        MacBookBuilder() : _mb(new MacBook()) {}
        void BuildBoard(const std::string &boardname) override
        {
            _mb->setBoard(boardname);
        }
        void BuildDisplay(const std::string &displayname) override
        {
            _mb->setDisplay(displayname);
        }
        void BuildOs() override
        {
            _mb->setOs();
        }
        Computer::Ptr Build() override
        {
            return _mb;
        }

    private:
        Computer::Ptr _mb; // 用基类指针调用纯虚函数 会自动匹配到场景随对应的重写函数
    };

    class Director
    {
    public:
        using Ptr = std::shared_ptr<Director>;
        Director(Builder *builder) : _bd(builder) {}
        void Construct(const std::string &board, const std::string &display)
        {
            _bd->BuildBoard(board);
            _bd->BuildDisplay(display);
            _bd->BuildOs();
        }

    private:
        Builder::Ptr _bd;
    };
}
// int main()
// {

//     Builder *bd = new MacBookBuilder();
//     std::unique_ptr<Director> dr(new Director(bd));
//     dr->Construct("番茄主板", "菠萝显示屏");
//     Computer::Ptr computer = bd->Build();
//     computer->PrintInfo();

//     return 0;
// }

// 代理模式
namespace Agent
{
    class RentHouse
    {
    public:
        virtual void rentHouse() = 0;
    };

    class Landlord : public RentHouse
    {
    public:
        using Ptr = std::shared_ptr<Landlord>;
        void rentHouse() override { std::cout << "把房子租出去" << std::endl; }
    };

    class Intermediary : public Landlord
    {
    public:
        Intermediary() : _ld(new Landlord()) {}
        void rentHouse() override
        {
            std::cout << "进行房子宣传" << std::endl;
            std::cout << "带人看房" << std::endl;
            _ld->rentHouse();
            std::cout << "进行房子售后维修" << std::endl;
        }

    private:
        Ptr _ld;
    };
}

// int main()
// {
//     Intermediary td;
//     td.rentHouse();

//     return 0;
// }