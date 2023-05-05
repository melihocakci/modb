#ifndef SHAPE_H
#define SHAPE_H

class Shape
{

    public:
        Shape()=default;
        Shape(&Shape shape)=default;
        ~Shape()=default;

        AddObject();
        


    private:
        std::string ShapeString;
}

#endif
