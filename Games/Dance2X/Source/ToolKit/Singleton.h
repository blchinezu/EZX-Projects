//! Singleton template. Taken from: http://www.gamedev.net/reference/articles/article1954.asp
//! which in turn is taken from Game Programming Gems

#ifndef SINGLETON_H
#define SINGLETON_H

#include <cassert>

template<typename T>
class Singleton
{
   static T* _Singleton;
   public:
      Singleton()
      {
         assert(!_Singleton);
         //use a cunning trick to get the singleton pointing to the start of
         //the whole, rather than the start of the Singleton part of the object
         int Offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
         _Singleton = (T*)((int)this + Offset);
      }
      ~Singleton()
      {
         assert(_Singleton);
         _Singleton=0;
      }
      static T& GetSingleton()
      {
         assert(_Singleton);
         return *_Singleton;
      }
      static T* GetSingletonPtr()
      {
         assert(_Singleton);
         return _Singleton;
      }
};

template <typename T> T* Singleton <T>::_Singleton = 0;

#endif // SINGLETON_H

/**
Usage: Taken from http://www.ogre3d.org/wiki/index.php/Singleton

How to use it
[edit]
Making a class a singleton

Pretend we want to convert the class MyManager to a Singleton.

class MyManager
{
public:
    MyManager();
    void doSomething();
}

So we need to extend and instatiate the Ogre::Singleton template and override the singleton access methods.
Our header file will look like this:

#include <OgreSingleton.h>
class MyManager : public Ogre::Singleton<MyManager>
{
public:
    MyManager();
    void doSomething();
    static MyManager& getSingleton(void);
    static MyManager* getSingletonPtr(void);
}

You don't have to override getSingleton() and getSingletonPtr() if your Singleton is not used outside
of your EXE/DLL, but else you have to. The source file would now look like this:

#include "MyManager.h"

template<> MyManager* Singleton<MyManager>::ms_Singleton = 0;
MyManager* MyManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
MyManager& MyManager::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}

// The rest of your implementation is following.
...

[todo] Do some more explaining.
[edit]
Using a singleton

// Creating the manager. This may only be called once!
new MyManager();

// Obtaining a reference to the manager instance
MyManager& mgr = MyManager::getSingleton();

// Obtaining a pointer to the manager instance
MyManager* pMgr = MyManager::getSingletonPtr();

Note: You may call the Singleton constructor only once. Calling it more often will result in a runtime
exception. There is another semantical difference between getSingleton() and getSingletonPtr(): If the
constructor is not called beforehand getSingletonPtr() will return a NULL-Pointer and getSingleton()
will throw a runtime exception. So it is probably better to use getSingleton() most of the time, even
though it is slightly slower. But you get a clearer response when you set something up wrongly.
*/

/*
Histoty
=======
2006-07-06: Added the header guards
2006-06-23: Created file
*/
