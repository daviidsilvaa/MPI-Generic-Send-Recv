
#ifndef ATTRIBUTE_HPP
#define ATTRIBUTE_HPP

template<typename T>
class Attribute{
public:
    int key;
    T value;

    Attribute(){ }

    Attribute(const int &key, const T &value){
		this->SetKey(key);
		this->SetValue(value);
	}

    Attribute(const Attribute<T> &attrib){
        this->SetKey(attrib.key);
        this->SetValue(attrib.value);
    }

    ~Attribute(){ }

	int GetKey(void){
		return this->key;
	}
	void SetKey(const int &key){
		this->key = key;
	}

	T GetValue(void){
		return this->value;
	}
	void SetValue(const T &value){
		this->value = value;
	}
};

#endif
