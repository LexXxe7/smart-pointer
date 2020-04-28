#include<iostream>
#include<list>
#include<typeinfo>
#include<cstdlib>
#include"details.h"
#include"iterator.h"

template<class T, int size=0>
class smart_ptr {
    public:
        // Define an iterator type for smart_ptr<T>
        typedef iter<T> iterator;
        
        // Empty constructor
        // NOTE: templates aren't able to have prototypes with default arguments
        // This is why the constructor is designed like this:
        smart_ptr() {
            smart_ptr(0);
        }
        smart_ptr(T*);
        smart_ptr(const smart_ptr&);  // Copy constructor
        ~smart_ptr();                 // Destructor
        
        // Overload assignment of pointer to smart_ptr
        T* operator=(T*);
        
        // Overload assignment of smart_ptr to smart_ptr
        smart_ptr& operator=(smart_ptr&);
        
        // Return a reference to the object pointed to by this smart_ptr
        T& operator*() {
            return *addr;
        }
        
        // Return the address being pointed to
        T* operator->() {
            return addr;
        }
        
        // Return a reference to the object at the index specified by i
        T& operator[](int i) {
            return addr[i];
        }
        
        // Conversion function to T*
        operator T*() {
            return addr;
        }
        
        // Collect garbage - returns true if at least one object was freed
        static bool collect();
        
        // Return an iter to the start of the allocated memory
        iter<T> begin() {
            int _size;
            
            if (isArray)
                _size=arraySize;
            else
                _size=1;
            
            return iter<T>(addr, addr, addr+_size);
        }
        
        // Return an iter to one past the end of the allocated memory
        iter<T> end() {
            int _size;
            
            if (isArray)
                _size=arraySize;
            else
                _size=1;
            
            return iter<T>(addr+_size, addr, addr+_size);
        }
        
        // Return the size of refContainer
        static int refContainerSize() {
            return refContainer.size();
        }
        
        // A utility function that displays refContainer
        static void showlist();
        
        // Clear refContainer when program exits
        static void shutdown();
    
    private:
        // refContainer maintains the garbage collection list
        static std::list<details<T> > refContainer;
        
        // Return an iterator to smart_ptr details in refContainer
        typename std::list<details<T> >::iterator findPtrInfo(T* ptr);
        
        // addr points to the allocated memory to which this smart_ptr currently points
        T* addr;
        
        // isArray is true if this smart_ptr points to an allocated array, it is false otherwise
        bool isArray;
        
        // If this smart_ptr is pointing to an allocated array, then arraySize contains its size
        unsigned int arraySize;
        
        static bool first;  // true when first smart_ptr is created
};

// STATIC INITIALIZATION
// Creates storage for the static variables
template<class T, int size>
std::list<details<T> > smart_ptr<T, size>::refContainer;

template<class T, int size>
bool smart_ptr<T, size>::first=true;

// Constructor for both - initialized and uninitialized - objects (-> see class interface)
template<class T, int size>
smart_ptr<T, size>::smart_ptr(T* t) {
    // Register shutdown() as an exit function
    if (first)
        atexit(shutdown);
    first=false;
    
    typename std::list<details<T> >::iterator p;
    p=findPtrInfo(t);
    
    if (p!=refContainer.end())
        p->refcount++;
    else {
        details<T> gcObj(t, size);
        refContainer.push_front(gcObj);
     }
     
     addr=t;
     arraySize=size;
     if (size>0)
         isArray=true;
     else
         isArray=false;
}

// Copy constructor
template<class T, int size>
smart_ptr<T, size>::smart_ptr(const smart_ptr &obj) {
    typename std::list<details<T> >::iterator p;
    
    p=findPtrInfo(obj.addr);
    p->refcount++;
    
    addr=obj.addr;
    arraySize=obj.arraySize;
    if (arraySize>0)
        isArray=true;
    else
        isArray=false;
}

// Destructor
template<class T, int size>
smart_ptr<T, size>::~smart_ptr() {
    typename std::list<details<T> >::iterator p;
    
    p=findPtrInfo(addr);
    if (p->refcount)
        p->refcount--;
    
    collect();
}

// Collect garbage - returns true if at least one object was freed
template<class T, int size>
bool smart_ptr<T, size>::collect() {
    bool memfreed=false;
    typename std::list<details<T> >::iterator p;
    
    do {
        for (p=refContainer.begin(); p!=refContainer.end(); p++) {
            if (p->refcount>0)
                continue;
            
            memfreed=true;
            refContainer.remove(*p);
            
            if (p->memPtr) {
                if (p->isArray)
                    delete [] p->memPtr;
                else
                    delete p->memPtr;
            }
            
            break;
        }
    } while (p!=refContainer.end());
    
    return memfreed;
}

// Overload assignment of pointer to smart_ptr
template<class T, int size>
T* smart_ptr<T, size>::operator=(T* t) {
    typename std::list<details<T> >::iterator p;
    
    p=findPtrInfo(addr);
    p->refcount--;
    
    p=findPtrInfo(t);
    if (p!=refContainer.end())
        p->refcount++;
    else {
        details<T> gcObj(t, size);
        refContainer.push_front(gcObj);
     }
     
     addr=t;
     
     return t;
}

// Overload assignment of smart_ptr to smart_ptr
template<class T, int size>
smart_ptr<T, size>& smart_ptr<T, size>::operator=(smart_ptr &rv) {
    typename std::list<details<T> >::iterator p;
    
    p=findPtrInfo(addr);
    p->refcount--;
    
    p=findPtrInfo(rv.addr);
    p->refcount++;
    
    addr=rv.addr;
    
    return rv;
}

// A utility function that displays refContainer
template<class T, int size>
void smart_ptr<T, size>::showlist() {
    typename std::list<details<T> >::iterator p;
    
    std::cout << "refContainer<" << typeid(T).name() << ", " << size << ">:\n";
    std::cout << "memPtr refcount value\n";
    
    if (refContainer.begin()==refContainer.end())
        std::cout << "Container is empty!\n\n";
    
    for (p=refContainer.begin(); p!=refContainer.end(); p++) {
        std::cout << '[' << (void*)p->memPtr << ']'
                  << ' ' << p->refcount << ' ';
        
        if (p->memPtr)
            std::cout << ' ' << *p->memPtr;
        else
            std::cout << "---";
        
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
}

// Find a pointer in refContainer
template<class T, int size>
typename std::list<details<T> >::iterator
smart_ptr<T, size>::findPtrInfo(T* ptr) {
    typename std::list<details<T> >::iterator p;
    
    // Find ptr in refContainer
    for (p=refContainer.begin(); p!=refContainer.end(); p++)
        if (p->memPtr==ptr)
            return p;
    
    return p;
}

// Clear refContainer when program exits
template<class T, int size>
void smart_ptr<T, size>::shutdown() {
    if (refContainerSize()==0)
        return;  // list is empty
    
    typename std::list<details<T> >::iterator p;
    for (p=refContainer.begin(); p!=refContainer.end(); p++)
        p->refcount=0;  // Set all reference counts to zero
    
    collect();
}