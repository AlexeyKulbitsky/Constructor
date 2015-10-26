#ifndef MPTR_H
#define MPTR_H


template<class T>
class MPtr
{
public:
    MPtr();
    MPtr(const MPtr<T>& p);
    ~MPtr();
    MPtr(T* p);
    T* operator->() const;
    operator T*() const;
    MPtr<T>& operator=(const MPtr<T>& p);
protected:
    struct RealPtr
    {
        T* pointer;
        unsigned int count;

        RealPtr(T* p = 0);
        ~RealPtr();
    };

    RealPtr* pointer;
private:
};

#endif // MPTR_H
