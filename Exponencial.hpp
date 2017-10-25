
#ifndef EXPONENCIAL_HPP
#define EXPONENCIAL_HPP

#include "Flow.hpp"

template<class T>
class Exponencial : public Flow<T>{
public:
	Exponencial() : Flow<T>(){ }

	Exponencial(const Cell<T> &cell, const double &flow_rate) : Flow<T>(cell, flow_rate){ }

	double execute(){
		return this->flow_rate * this->source.attribute.value;
	}

	double execute(const Cell<T> &cell){
		return this->flow_rate * cell.attribute.value;
	}
};

#endif
