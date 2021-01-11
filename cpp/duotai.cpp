/*C++学习笔记  多态*/

#include <iostream>
#include <stdlib.h>
using namespace std;
 
class CA
{
public:
    void f()
    {
        cout << "CA f()" << endl;
    }
    virtual void ff()
    {
        cout << "CA ff()" << endl;
        f();
    }
};
 
class CB : public CA
{
public :
    virtual void f()
    {
        cout << "CB f()" << endl;
    }
    void ff()
    {
        cout << "CB ff()" << endl;
        f();
        CA::ff();
    }
};
class CC : public CB
{
public:
    virtual void f()
    {
        cout << "C f()" << endl;
    }
};
 
int main()
{
    CB b;
    CA *ap = &b;
    CC c;
    CB &br = c;
    CB *bp = &c;
 
    ap->f();        // CA f()
    cout << endl;
  
    b.f();          // CB f()
    cout << endl;
 
    br.f();         // C f()
    cout << endl;
 
    bp->f();
    cout << endl;
 
    ap->ff();
    cout << endl;
 
    bp->ff();
    cout << endl;
 
    return 0;
}
