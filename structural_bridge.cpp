// Bridge Pattern
// ---------------
// Decouple an abstraction from its implementation so that the two can vary 
// independently.
//
// Applications
// ------------
// 1. You want to avoid a permanent binding between an abstraction and its 
// implementation. This might be the case, for example, when the implementation
// must be selected or switched at run-time.
// 2. Both the abstractions and their implementations should be extensible by
// subclassing. In this case, the Bridge pattern lets you combine the different
// abstractions and implementations and extend them independently.
// 3. Changes in the implementation of an abstraction should have no impact on
// clients; that is, their code should not have to be recompiled.
// 4. (C++) You want to hide the implementation of an abstraction completely
// from clients. In C++ the representation of a class is visible in the class 
// interface.
// 5. You want to share an implementation among multiple objects (perhaps using
// reference counting), and this fact should be hidden from the client.

#include <iostream>
using namespace std;

// Suppose the Window interface below contains the necessary methods to draw a
// basic window on the screen. This interface will be extended by subclasses to
// create more specific window like application window, dialog window, icon
// window, and so on. On the other hand, we want the window to be shown in both
// Windows and Linux OS.

// If we put the implementations and abstractions in the same place, for each
// type of window, there will be two classes, one for Linux and one for 
// Windows. But if we decouple implementations in another class and supply its
// reference to the abstract classes, there will be two classes for Linux &
// Windows implementations plus one class for each type of window.

// Here is an abstract class for implementation WindowImp (our IMPLEMENTOR)
// that will be  implemented by WindowsWindowImp and LinuxWindowImp (our
// CONCRETE IMPLEMENTOR) for Windows and Linux platforms respectively.
class WindowImp {
public:
    virtual void setExtent() = 0;
    virtual void setOrigin() = 0;
    virtual void top() = 0;
    virtual void bottom() = 0;
    virtual void drawRect() = 0;
    virtual void drawText() = 0;
    virtual void drawBitmap() = 0;
    // ...and many more drawing functions independent of Window abstraction
protected:
    WindowImp();
};

class WindowsWindowImp: public WindowImp {
public:
    // Platform specific implementations of WindowImp interface
private:
    // Windows specific fields and functions
};

class LinuxWindowImp: public WindowImp {
public:
    // Platform specific implementations of WindowImp interface
private:
    // Linux specific fields and functions
};

// Here is an abstract Window class (our ABSTRACTION) that will be implemented
// and extended by different types of windows.
// Notice that we have delegated the choosing of appropriate IMPLEMENTOR to the
// WindowSystemFactory, a factory method, that returns the appropriate concrete
// implementor based on some logic.
// We could instantiate an appropriate implementor in the constructor if Window
// knew about all concrete implementors, using some logic. For example,
// depending on use case, we can instantiate a linked list or array based
// data structure.
// Another way is to choose a default implementor and choose another later
// depending on use cases. It is possible since abstraction & implementation is
// decoupled. For example, initially we can create a array based data
// structure, but change it to a list based when number of elements is above
// a threshold.
class Window {
public:
    Window();
    // Calls are forwarded to the implementation
    virtual void setExtent();
    virtual void setOrigin();
    virtual void raise();
    virtual void lower();
    // ...and many more
protected:
    WindowImp* getWindowImp() {
        if (imp = 0) {
            imp = WindowSystemFactory()::instance()->makeWindowImp();
            // WindowSystemFactory is a singleton, abstract factory method that
            // returns the appropriate WindowImp based on the current platform.
        }
        return imp;
    }
private:
    WindowImp* imp;
};

// An application window (our REFINED ABSTRACTION) will show contents in the
// window.
class ApplicationWindow: public Window {
public:
    ApplicationWindow() = default;
    // ...methods of Window interface
    virtual void showContent();
};

// Calls the IMPLEMENTOR to draw contents on specific platforms.
void ApplicationWindow::showContent() {
    getWindowImp()->drawText();
}

// Similarly an icon window (our REFINED ABSTRACTION) will show icons in the
// window.
class IconWindow: public Window {
public:
    IconWindow() = default;
    // ...methods of Window interface
    virtual void showContent();
};

// Calls the IMPLEMENTOR to draw contents on specific platforms.
void IconWindow::showContent() {
    getWindowImp()->drawBitmap();
}

// In this way, we can grow and change both our IMPLEMENTOR & ABSTRACTION
// independently of each other. At the same time, we are hiding implementation
// details from the client.

// However, there is only one Implementor, for example, we only want a Linux
// based window, there is no need of a bridge pattern. But it could be needed
// if you don't want any change in implementation to disturb existing code 
// base. In other word, if the existing codebase should not be recompiled,
// just relinked, we have to use bridge pattern.