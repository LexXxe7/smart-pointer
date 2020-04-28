template<class T>
class details {
    public:
        unsigned int refcount;   // Current reference count
        T* memPtr;               // Pointer to allocated memory
        bool isArray;            // True if pointing to array
        unsigned int arraySize;
        
        details(T*, unsigned int);
};

template<class T>
details<T>::details(T* t, unsigned int size) {
    memPtr  =t;
    refcount=1;
    if (size>0) {
        isArray  =true;
        arraySize=size;
    }
    else
        isArray=false;
}

template<class T>
bool operator==(const details<T> &obj1, const details<T> &obj2) {
    return (obj1.memPtr==obj2.memPtr);
}