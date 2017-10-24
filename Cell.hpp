
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
    int neighbors[NEIGHBORS + NEIGHBORS];
    int count_neighbors;

    Cell(){ }

    Cell(const int &x, const int &y, const Attribute<T> &attrib){
        this->SetX(x);
        this->SetY(y);
        this->SetAttribute(attrib);
        this->count_neighbors = 0;
    }

    Cell(const Cell<T> &cell){
        this->SetX(cell.x);
        this->SetY(cell.y);
        this->SetAttribute(cell.attribute);
        this->count_neighbors = cell.count_neighbors;
        for(int i = 0; i < NEIGHBORS; i++)
            this->neighbors[i] = cell.neighbors[i];
    }

    ~Cell(){ }

    Cell<T>& operator=(const Cell<T> &cell){
        if(this != &cell){
            this->SetX(cell.x);
            this->SetY(cell.y);
            this->SetAttribute(cell.attribute);
            this->count_neighbors = cell.count_neighbors;
            for(int i = 0; i < NEIGHBORS; i++)
                this->neighbors[i] = cell.neighbors[i];
        }
        return *this;
    }

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

    Cell<T> SetNeighbor(Cell<T> cell_received){
        Cell<T> cell = Cell<T>(cell_received);
        int neighbors[NEIGHBORS + NEIGHBORS];
        int count_neighbors;

        if(cell.x == 0){
            if(cell.y == 0){
                count_neighbors = 3;
                neighbors[0] = cell.x;      neighbors[NEIGHBORS + 0] = cell.y + 1;
                neighbors[1] = cell.x + 1;  neighbors[NEIGHBORS + 1] = cell.y + 1;
                neighbors[2] = cell.x + 1;  neighbors[NEIGHBORS + 2] = cell.y;
                return cell;
            }if(cell.y == DIMY-1){
                count_neighbors = 3;
                neighbors[0] = cell.x;      neighbors[NEIGHBORS + 0] = cell.y - 1;
                neighbors[1] = cell.x + 1;  neighbors[NEIGHBORS + 1] = cell.y - 1;
                neighbors[2] = cell.x + 1;  neighbors[NEIGHBORS + 2] = cell.y;
                return cell;
            }
            if(cell.y != DIMY-1 && cell.y != 0){
                count_neighbors = 5;
                neighbors[0] = cell.x;      neighbors[NEIGHBORS + 0] = cell.y + 1;
                neighbors[1] = cell.x + 1;  neighbors[NEIGHBORS + 1] = cell.y + 1;
                neighbors[2] = cell.x + 1;  neighbors[NEIGHBORS + 2] = cell.y;
                neighbors[3] = cell.x + 1;  neighbors[NEIGHBORS + 3] = cell.y - 1;
                neighbors[4] = cell.x;      neighbors[NEIGHBORS + 4] = cell.y - 1;
                return cell;
            }
        }
        if(cell.x == DIMX-1){
            if(cell.y == 0){
                count_neighbors = 3;
                neighbors[0] = cell.x - 1;  neighbors[NEIGHBORS + 0] = cell.y;
                neighbors[1] = cell.x - 1;  neighbors[NEIGHBORS + 1] = cell.y - 1;
                neighbors[2] = cell.x;      neighbors[NEIGHBORS + 2] = cell.y + 1;
                return cell;
            }
            if(cell.y == DIMY-1){
                count_neighbors = 3;
                neighbors[0] = cell.x - 1;  neighbors[NEIGHBORS + 0] = cell.y;
                neighbors[1] = cell.x - 1;  neighbors[NEIGHBORS + 1] = cell.y - 1;
                neighbors[2] = cell.x;      neighbors[NEIGHBORS + 2] = cell.y - 1;
                return cell;
            }
            if(cell.y != DIMY-1 && cell.y != 0){
                count_neighbors = 5;
                neighbors[0] = cell.x;      neighbors[NEIGHBORS + 0] = cell.y + 1;
                neighbors[1] = cell.x - 1;  neighbors[NEIGHBORS + 1] = cell.y + 1;
                neighbors[2] = cell.x - 1;  neighbors[NEIGHBORS + 2] = cell.y;
                neighbors[3] = cell.x - 1;  neighbors[NEIGHBORS + 3] = cell.y - 1;
                neighbors[4] = cell.x;      neighbors[NEIGHBORS + 4] = cell.y - 1;
                return cell;
            }
            if(cell.y == 0){
                if((cell.x != 0) && (cell.x != DIMX-1)){
                    count_neighbors = 5;
                    neighbors[0] = cell.x - 1;  neighbors[NEIGHBORS + 0] = cell.y;
                    neighbors[1] = cell.x - 1;  neighbors[NEIGHBORS + 1] = cell.y + 1;
                    neighbors[2] = cell.x;      neighbors[NEIGHBORS + 2] = cell.y + 1;
                    neighbors[3] = cell.x + 1;  neighbors[NEIGHBORS + 3] = cell.y + 1;
                    neighbors[4] = cell.x + 1;  neighbors[NEIGHBORS + 4] = cell.y;
                    return cell;
                }
            }
            if(cell.y == DIMY-1){
                if((cell.x != 0) && (cell.x != DIMX-1)){
                    count_neighbors = 5;
                    neighbors[0] = cell.x - 1;  neighbors[NEIGHBORS + 0] = cell.y;
                    neighbors[1] = cell.x - 1;  neighbors[NEIGHBORS + 1] = cell.y - 1;
                    neighbors[2] = cell.x;      neighbors[NEIGHBORS + 2] = cell.y - 1;
                    neighbors[3] = cell.x + 1;  neighbors[NEIGHBORS + 3] = cell.y - 1;
                    neighbors[4] = cell.x + 1;  neighbors[NEIGHBORS + 4] = cell.y;
                    return cell;
                }
            }
            if(cell.x > 0 && cell.x < DIMX-1 && cell.y > 0 && cell.y < DIMY-1){
                count_neighbors = 8;
                neighbors[0] = cell.x - 1;  neighbors[NEIGHBORS + 0] = cell.y;
                neighbors[1] = cell.x - 1;  neighbors[NEIGHBORS + 1] = cell.y - 1;
                neighbors[2] = cell.x;      neighbors[NEIGHBORS + 2] = cell.y - 1;
                neighbors[3] = cell.x + 1;  neighbors[NEIGHBORS + 3] = cell.y - 1;
                neighbors[4] = cell.x + 1;  neighbors[NEIGHBORS + 4] = cell.y;
                neighbors[5] = cell.x + 1;  neighbors[NEIGHBORS + 5] = cell.y + 1;
                neighbors[6] = cell.x;      neighbors[NEIGHBORS + 6] = cell.y - 1;
                neighbors[7] = cell.x - 1;  neighbors[NEIGHBORS + 7] = cell.y + 1;
                return cell;
            }
        }
    }
};

template<typename T>
MPI_Datatype Make_MPI_Cell(){
    MPI_Datatype MPI_Attribute = Make_MPI_Atribute<T>();

    MPI_Datatype MPI_Cell;

    const int nitens = 5;
    MPI_Datatype blocktypes[5] = {MPI_INT, MPI_INT, MPI_Attribute, MPI_INT, MPI_INT};
    int blocklengths[5] = {1, 1, 1, (NEIGHBORS + NEIGHBORS), 1};
    MPI_Aint offsets[5] = {offsetof(Cell<T>, x), offsetof(Cell<T>, y), offsetof(Cell<T>, attribute),
        offsetof(Cell<T>, neighbors), offsetof(Cell<T>, count_neighbors)};

    MPI_Type_create_struct(nitens, blocklengths, offsets, blocktypes, &MPI_Cell);
    MPI_Type_commit(&MPI_Cell);

    return MPI_Cell;
}

#endif

// void AddNeighbor(const int &x_neighbor, const int &y_neighbor){
//     this->neighbors[count_neighbors] = x_neighbor;
//     this->neighbors[NEIGHBORS + this->count_neighbors] = y_neighbor;
//     this->count_neighbors = this->count_neighbors + 1;
// }
