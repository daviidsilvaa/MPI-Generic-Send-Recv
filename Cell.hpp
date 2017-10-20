
#ifndef CELL_HPP
#define CELL_HPP

template<typename T>
class Cell{
public:
    int x;
    int y;
    Attribute<T> attribute;
    // Cell<T> *neighbors;

    Cell(){ }
    Cell(const int &x, const int &y, const Attribute<T> &attrib, int &size){
        this->SetX(x);
        this->SetY(y);
        this->SetAttribute(attrib);
    }

    ~Cell(){ }

    void SetX(const int &x){
        this->x = x;
    }
    void SetY(const int &y){
        this->y = y;
    }
    void SetAttribute(const Attribute<T> &attrib){
        this->attribute = attrib;
    }

    int GetX(){
        return this->x;
    }
    int GetY(){
        return this->y;
    }
    Attribute<T> GetAttribute(){
        return this->attribute;
    }
};

#endif
