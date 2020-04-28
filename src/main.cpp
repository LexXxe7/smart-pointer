#include"pointer.h"

int main() {
    smart_ptr<int> p=new int(14);
    p=new int(99);
    p=new int(100);
}
