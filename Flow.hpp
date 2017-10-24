
#ifndef FLOW_HPP
#define FLOW_HPP

#include "Cell.hpp"

template<class T>
class Flow{
public:
    double flow_rate;
    double time;
    Cell<T> source;

    Flow(){ }

    Flow(const Cell<T> &cell, const double &flow_rate, const double &time){
        this->source = cell;
        this->flow_rate = flow_rate;
        this->time = time;
    }

    Flow(const Flow<T> &flow){
        this->source = flow.source;
        this->flow_rate = flow.flow_rate;
        this->time = flow.time;
    }

    Flow<T>& operator=(const Flow<T> &flow){
        if(this != &flow){
            this->source = flow.source;
            this->flow_rate = flow.flow_rate;
            this->time = flow.time;
        }
        return *this;
    }

    ~Flow(){ }

    virtual double execute() = 0;
};

#endif
