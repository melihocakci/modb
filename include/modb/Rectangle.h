#ifndef RECTANGLE_H
#define RECTANGLE_H

namespace modb {
    class Rectangle {
        public:

            float m_width;
            float m_height;
        
            Rectangle() = default;
            Rectangle(Rectangle &other) = default;


            ~Rectangle() = default;

            template <class Archieve>
            inline void serialize(Archieve& ar, unsigned int) {
                ar& m_width;
                ar& m_height;
            }
    };

}


#endif