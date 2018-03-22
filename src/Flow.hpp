
#ifndef FLOW_HPP
#define FLOW_HPP

#include "Cell.hpp"

template<class T>
class Flow{
public:
    Cell<T> source;
    int target[NEIGHBORS+NEIGHBORS];
    int count_targets;
    double flow_rate;
    double last_execute;

    Flow(){ }

    Flow(const Cell<T> &cell, const double &flow_rate){
        this->source = cell;
        this->count_targets = cell.count_neighbors;
        this->flow_rate = flow_rate;
        for(int i = 0; i < this->count_targets; i++){
            // x from target
            target[i] = cell.neighbors[i];
            // y from target
            target[NEIGHBORS + i] = cell.neighbors[NEIGHBORS + i];
        }
    }

    Flow(const Flow<T> &flow){
        this->source = flow.source;
        this->count_targets = flow.count_targets;
        for(int i = 0; i < this->count_targets; i++){
            target[i] = flow.target[i];
            target[NEIGHBORS + i] = flow.target[NEIGHBORS + i];
        }
        this->flow_rate = flow.flow_rate;
        this->last_execute = flow.last_execute;
    }

    ~Flow(){ }

    Flow<T>& operator=(const Flow<T> &flow){
        if(this != &flow){
            this->source = flow.source;
            this->count_targets = flow.count_targets;
            for(int i = 0; i < this->count_targets; i++){
                target[i] = flow.target[i];
                target[NEIGHBORS + i] = flow.target[NEIGHBORS + i];
            }
            this->flow_rate = flow.flow_rate;
            this->last_execute = flow.last_execute;
        }
        return *this;
    }

    virtual double execute() = 0;
};

#endif
