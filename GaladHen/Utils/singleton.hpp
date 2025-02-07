
template<class T>
class Singleton
{

public:

    // Remove copy and move ctors
    Singleton(const Singleton<T>& Other) = delete;
    Singleton(Singleton<T>&& Other) = delete;
    Singleton<T>& operator=(Singleton<T>& Other) = delete;
    Singleton<T>& operator=(Singleton<T>&& Other) = delete;

    friend T;
    static T* GetInstance()
    {
        static T Instance;

        return &Instance;
    }

protected:

    Singleton() {}
};
