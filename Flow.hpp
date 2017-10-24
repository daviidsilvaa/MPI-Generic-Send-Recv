
#ifndef FLOW_HPP
#define FLOW_HPP

#include "Cell.hpp"

template<class T>
class Flow{
public:
    Cell<T> cell;
    double flow_rate;
    double time;

    Flow(){ }

    Flow(const Cell<T> &cell, const double &flow_rate, const double &time){
        this->cell = cell;
        this->flow_rate = flow_rate;
        this->time = time;
    }

    Flow(const Flow<T> &flow){
        this->cell = flow.cell;
        this->flow_rate = flow.flow_rate;
        this->time = flow.time;
    }

    Flow<T>& operator=(const Flow<T> &flow){
        if(this != &flow){
            this->cell = flow.cell;
            this->flow_rate = flow.flow_rate;
            this->time = flow.time;
        }
        return *this;
    }

    ~Flow(){ }

    virtual double execute() = 0;
};

#endif
