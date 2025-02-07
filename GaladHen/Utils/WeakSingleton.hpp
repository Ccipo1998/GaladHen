
#pragma once

template<class T>
class WeakSingleton
{

public:

    WeakSingleton()
    {
        // Instance created on first ctor call
        if (!InstanceRef)
            InstanceRef = static_cast<T*>(this);
    }

    WeakSingleton(const WeakSingleton<T>& Other) = delete;
    WeakSingleton(WeakSingleton<T>&& Other) = delete;
    WeakSingleton<T>& operator=(WeakSingleton<T>& Other) = delete;
    WeakSingleton<T>& operator=(WeakSingleton<T>&& Other) = delete;

    friend T;
    static T* GetInstance()
    {
        return InstanceRef;
    }

protected:

    // Ref to instance
    static T* InstanceRef;
};

template <class T>
T* WeakSingleton<T>::InstanceRef = nullptr;
