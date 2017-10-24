
#ifndef CELL_HPP
#define CELL_HPP

#include "Defines.hpp"

template<typename T>
class Cell{
public:
    int x;
    int y;
    Attribute<T> attribute;
    // Cell<T> neighbors[NEIGHS];
    int x_neighbors[NEIGHBORS];
    int y_neighbors[NEIGHBORS];
    int count_neighbors;

    Cell(){ }
    Cell(const int &x, const int &y, const Attribute<T> &attrib){
        this->SetX(x);
        this->SetY(y);
        this->SetAttribute(attrib);
        this->count_neighbors = 0;
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
    void AddNeighbor(const int &x_neighbor, const int &y_neighbor){
        this->x_neighbors[count_neighbors] = x_neighbor;
        this->y_neighbors[count_neighbors] = y_neighbor;
        this->count_neighbors = this->count_neighbors + 1;
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
    // void RemoveNeighbor(const int &x_neighbor, const int &y_neighbor){
    //     this->x_neighbors[count_neighbors] = x_neighbor;
    //     this->y_neighbors[count_neighbors] = y_neighbor;
    //     this->count_neighbors = this->count_neighbors + 1;
    // }
};

template<typename T>
MPI_Datatype Make_MPI_Cell(){
    MPI_Datatype MPI_Attribute = Make_MPI_Atribute<T>();

    MPI_Datatype MPI_Cell;

    const int nitens = 6;
    MPI_Datatype blocktypes[3] = {MPI_INT, MPI_INT, MPI_Attribute, MPI_INT, MPI_INT, MPI_INT};
    int blocklengths[6] = {1, 1, 1, NEIGHBORS, NEIGHBORS, 1};
    MPI_Aint offsets[6] = {offsetof(Cell<T>, x), offsetof(Cell<T>, y), offsetof(Cell<T>, attribute),
        offsetof(Cell<T>, x_neighbors), offsetof(Cell<T>, y_neighbors), offsetof(Cell<T>, count_neighbors)};

    MPI_Type_create_struct(nitens, blocklengths, offsets, blocktypes, &MPI_Cell);
    MPI_Type_commit(&MPI_Cell);

    return MPI_Cell;
}

#endif
