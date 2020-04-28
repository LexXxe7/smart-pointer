class outOfRangeExc {
    // Add functionality if needed by your application
};

/* An iterator-like class for cycling through arrays that are pointed
to by smart pointers. iter pointers ** DO NOT ** participate in or
affect garbage collection. Thus, an iter pointing to some object
does not prevent that object from being recycled.
*/

template<class T>
class iter {
    public:
        iter() {
            ptr=end=begin=0;
            length=0;
        }
        iter(T* p, T* first, T* last) {
            ptr   =p;
            end   =last;
            begin =first;
            length=last-first;
        }
        
        // Return length of sequence to which this iter points
        unsigned int size() const {return length;}
        
        // Return value pointed to by ptr (do not allow out-of-bounds access)
        T& operator*() {
            if ((ptr>=end) || (ptr<begin))
                throw outOfRangeExc();
            
            return *ptr;
        }
        
        // Return address contained in ptr (do not allow out-of-bounds access)
        T* operator->() {
            if ((ptr>=end) || (ptr<begin))
                throw outOfRangeExc();
            
            return ptr;
        }
        
        // Prefix ++
        iter operator++() {
            ptr++;
            
            return *this;
        }
        
        // Prefix --
        iter operator--() {
            ptr--;
            
            return *this;
        }
        
        // Postfix ++
        iter operator++(int notused) {
            T* tmp=ptr;
            ptr++;
            
            return iter<T>(tmp, begin, end);
        }
        
        // Postfix --
        iter operator--(int notused) {
            T* tmp=ptr;
            ptr--;
            
            return iter<T>(tmp, begin, end);
        }
        
        // Return a reference to the object at the specified index (do not allow out-of-bounds access)
        T& operator[](int i) {
            if ((i<0) || (i>=(end-begin)))
                throw outOfRangeExc();
            
            return ptr[i];
        }
        
        // Define the relational operators
        bool operator==(iter obj2) {return (ptr==obj2.ptr);}
        bool operator!=(iter obj2) {return (ptr!=obj2.ptr);}
        bool operator<=(iter obj2) {return (ptr<=obj2.ptr);}
        bool operator>=(iter obj2) {return (ptr>=obj2.ptr);}
        bool operator<(iter obj2) {return (ptr<obj2.ptr);}
        bool operator>(iter obj2) {return (ptr>obj2.ptr);}
        
        // Add an integer to an iter
        iter operator+(int n) {
            ptr+=n;
            
            return *this;
        }
        
        // Subtract an integer from an iter
        iter operator-(int n) {
            ptr-=n;
            
            return *this;
        }
        
        // Return number of elements between two iters
        int operator-(iter<T> &itr2) {
            return ptr-itr2.ptr;
        }
    
    private:
        T* ptr;    // Current pointer value
        T* end;    // Points to element one past end
        T* begin;  // Points to start of allocated array
        
        unsigned int length;  // Length of sequence
};