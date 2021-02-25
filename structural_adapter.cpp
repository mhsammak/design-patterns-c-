// Adapter Pattern
// ---------------
// Converts the interface of a class into another interface clients expect.
// Adapter lets classes work together that couldn't otherwise because of 
// incompatible interfaces.
//
// Applications
// ------------
// 1. You want to use an existing class, and its interface does not match the
// one you need.
// 2. you want to create a reusable class that cooperates with unrelated or 
// unforeseen classes, that is, classes that don't necessarily have compatible
// interfaces.
// 3. (object adapter only) you need to use several existing subclasses, but
// it's unpractical to adapt their interface by subclassing every one. An 
// object adapter can adapt the interface of its parent class.

#include <iostream>
using namespace std;

// We will be dealing with a drawing program that can take coordinates
// of points in a screen through a Point class, draw different shapes
// through a Shape class, and manipulate the shapes through a Manipulator
// class.

// A Point class to contain the coordinates of a point in a screen.
class Point {
public:
    double x;
    double y;

    Point(double xCord, double yCord) {
        x = xCord;
        y = yCord;
    }
};

// An abstract class to manipulate Shapes in the screen. For example
// dragging, resizing, etc. Extended by subclasses to manipulate
// different kinds of Shapes like LinesShapes, SquareShapes, etc.
class Manipulator {
public:
    Manipulator();
    virtual void manipulate();
};

// An example LineShape manipulator.
class LineManipulator: public Manipulator {
public:
    LineManipulator();
    virtual void manipulate();
};

void LineManipulator::manipulate() {
    cout << "Manipulating Lines" << endl;
}

// Suppose we have an abstract Shape class (our TARGET) that can draw different
// shapes like line in LineShape subclass, square in SquareShape subclass, etc. 
// For simplicity, the interface has a method drawBoundingBox that takes the 
// coordinates of top left and bottom right points to draw the bounding box.
// The getManipulator returns a specific manipulator for specific shape.
class Shape {
public:
    Shape();
    virtual void drawBoundingBox(Point& topLeft, Point& bottomRight);
    virtual Manipulator* getManipulator();
};

// An example LineShape, concrete implementation of the Shape interface.
class LineShape: public Shape {
public:
    LineShape();
    virtual void drawBoundingBox(Point& topLeft, Point& bottomRight);
    virtual void drawShape();
    virtual Manipulator* getManipulator();
};

void LineShape::drawBoundingBox(Point& topLeft, Point& bottomRight) {
    cout << "Drawing bounding box for lines" << endl;
}

void LineShape::drawShape() {
    cout << "Drawing Line" << endl;
}

Manipulator* LineShape::getManipulator() {
    return new LineManipulator();
}

// Extending the Shape interface to draw lines, squares, polygons, etc. are
// easy. But now you are asked to to create a subclass that can draw texts.
// You know this is tedious task. But you came to know that there is a class
// TextView (our Adaptee) already written by someone to do the exact thing.
// Only problem is that it has a different interface. It takes the origin 
// coordinate and also height and width to draw bounding box. It also uses
// different method to draw the text.
class TextView {
public:
    TextView();
    void origin(double x, double y);
    void area(double height, double width);
    void write();
};

// You then write an adapter class TextShape that makes these two different
// interfaces work together. We can let TextShape inherit Shape's interface & 
// TextView's implementation (class-based) to make things work. To do so, we
// have to inherit Shape publicly, and TextView privately. Then we can tweak
// the Shape interface so that it can utilize TextView's implementation as
// it is. It has the advantage that only one object is created and there is no
// complexities of pointers to any TextView objects as it directly inherits it.

// TextView does not provide any option for manipulation. On the other hand
// the Shape interface does. So, we need to define a TextManipulator for the
// TextShape adapter.
class TextManipulator: public Manipulator {
public:
    TextManipulator();
    virtual void manipulate();
};

void TextManipulator::manipulate() {
    cout << "Manipulating text" << endl;
}

// The drawBoundingBox computes the origin coordinates from the given topLeft
// & bottomRight arguments and call the TextView's origin method with that.
// Similarly, it computes the height & width and call the TextView's area 
// method. The drawShape then calls TextView's write method. Now a client use
// the regular Shape interface to draw texts.
class TextShape: public Shape, private TextView {
public:
    TextShape();
    virtual void drawBoundingBox(Point& topLeft, Point& bottomRight);
    virtual void drawShape();
    virtual Manipulator* getManipulator();
};

void TextShape::drawBoundingBox(Point& topLeft, Point& bottomRight) {
    origin((bottomRight.x - topLeft.x) / 2, (topLeft.y - bottomRight.y) / 2);
    area(bottomRight.x - topLeft.x, topLeft.y - bottomRight.y);
}

void TextShape::drawShape() {
    write();
}

Manipulator* TextShape::getManipulator() {
    return new TextManipulator();
}

// But what happens if we want to adapt not only TextView class but also its
// all possible subclasses. It won't work because we are adapting a concrete
// adaptee class (TextView) implementation, not an interface. Also, it has
// some issues like it lets modify adaptee behaviour in some ways as it is a
// subclass of the adaptee and can override methods. Another way to adapt
// TextView is to supply it to TextShape as a pointer rather than inheriting 
// it (object-based).
class TextShape2 {
public:
    TextShape2(TextView*);
    virtual void drawBoundingBox(Point& topLeft, Point& bottomRight);
    virtual void drawShape();
    virtual Manipulator* getManipulator();
private:
    TextView* textView;
};

TextShape2::TextShape2(TextView* tv) {
    textView = tv;
}

void TextShape2::drawBoundingBox(Point& topLeft, Point& bottomRight) {
    textView->origin((bottomRight.x - topLeft.x) / 2, (topLeft.y - bottomRight.y) / 2);
    textView->area(bottomRight.x - topLeft.x, topLeft.y - bottomRight.y);
}

void TextShape2::drawShape() {
    textView->write();
}

Manipulator* TextShape2::getManipulator() {
    return new TextManipulator();
}

// Now, we can use not only a specific concrete TextView but also its subclasses
// if any.