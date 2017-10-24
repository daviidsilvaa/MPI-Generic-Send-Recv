
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

template<class T>
MPI_Datatype Make_MPI_Atribute(){
	MPI_Datatype MPI_Attribute;

	const int nitens = 2;
	MPI_Datatype blocktypes[2] = {MPI_INT, ConvertType(getAbstractionDataType<T>())};
	int blocklengths[2] = {1, 1};
	MPI_Aint offsets[2] = {offsetof(Attribute<T>, key), offsetof(Attribute<T>, value)};

	MPI_Type_create_struct(nitens, blocklengths, offsets, blocktypes, &MPI_Attribute);
	MPI_Type_commit(&MPI_Attribute);

	return MPI_Attribute;
}

#endif
