
template<class T>
class Singleton
{

public:

    // Remove ctor, copy and move ctors
    Singleton() = delete;
    Singleton(const Singleton<T>& Other) = delete;
    Singleton(const Singleton<T>&& Other) = delete;
    Singleton<T>& operator=(const Singleton<T>& Other) = delete;
    Singleton<T>& operator=(const Singleton<T>&& Other) = delete;

    friend T;
    static T* GetInstance()
    {
        static T Instance;

        return &Instance;
    }
};
