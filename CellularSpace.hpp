
#ifndef CELLULARSPACE_HPP
#define CELLULARSPACE_HPP

template<class T>
class CellularSpace{
public:
    int width;
    int height;
    Cell<T> *memoria;

    static CellularSpace* create(const int &width, const int &height){
        CellularSpace<T> *cs = new CellularSpace<T>;
        cs->width = width;
        cs->height = height;
        cs->memoria = new Cell<T>[width*height];

        return cs;
    }

    ~CellularSpace(){
        delete memoria;
    }

    void SetCell(Cell<T> cell, const int &i, const int &j){
        this->memoria[i*this->width + j] = cell;
    }

    Cell<T> GetCell(const int &i, const int &j){
		return memoria[i*this->width + j];
	}
};

#endif
