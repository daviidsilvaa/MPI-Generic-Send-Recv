
#ifndef FLOW_HPP
#define FLOW_HPP

#include "Cell.hpp"

template<class T>
class Flow{
public:
    Cell<T> source;
    double flow_rate;
    double last_execute;

    Flow(){ }

    Flow(const Cell<T> &cell, const double &flow_rate){
        this->source = cell;
        this->flow_rate = flow_rate;
    }

    Flow(const Flow<T> &flow){
        this->source = flow.source;
        this->flow_rate = flow.flow_rate;
        this->last_execute = flow.last_execute;
    }

    ~Flow(){ }

    Flow<T>& operator=(const Flow<T> &flow){
        if(this != &flow){
            this->source = flow.source;
            this->flow_rate = flow.flow_rate;
            this->last_execute = flow.last_execute;
        }
        return *this;
    }

    virtual double execute() = 0;
    virtual double execute(const Cell<T> &) = 0;
};

#endif
