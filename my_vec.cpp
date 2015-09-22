#include <iostream>
#include <cstring>

using namespace std;

template <typename T>
class My_vec {
	
	int size, capacity;
	T *ptr;

	void resize(int r, const T& elem)
	{
        	T *new_array = new T[2*capacity];

		memcpy(new_array, ptr, capacity*sizeof(T));	//dest, source, num of bytes

		ptr = new_array;
        	capacity *= 2;

        	if (r > capacity)
                	resize(r, elem);	//ensure the index is not still out of bounds e.g. index = 100
	}

	
public:	
	My_vec(): size(0), capacity(10), ptr(new T[capacity]) {} //constructor
	~My_vec() { delete[] ptr; }	//destructor
						
	int get_capacity() const { return capacity; }
	int get_size() const { return size; }
	bool is_empty() const { return size == 0; }

	My_vec(const My_vec& vec)				//copy constructor
	{
                	size = vec.size;
                	capacity = vec.capacity;
               		ptr = new T[capacity];

                	memcpy(ptr, vec.ptr, capacity*sizeof(T));
	}	
		
	My_vec& operator=(const My_vec& vec)
	{
                	size = vec.size;
                	capacity = vec.capacity;
                	ptr = new T[capacity];

                	memcpy(ptr, vec.ptr, capacity*sizeof(T));

			return *this;
	}
		
	T& operator[](int i) const
	{
		if (i < 0 || i > capacity)
                	cout << "Index out of bounds!";
        	else
                	return ptr[i];
	}

	T& operator[](int i)
	{
		if (i < 0 || i > capacity)
                	cout << "Index out of bounds!";
        	else
                	return ptr[i];
	}

	void push_back(const T& elem)
	{
		if (size >= capacity)
			resize(size, elem);

		ptr[size] = elem;
		size++;
	}

	void insert(int index, const T& elem)
	{

        	if(index < 0)
                	cout << "Invalid index";

        	else
        	{
			
                	if(index > capacity)
                        	resize(index, elem);




			if (index > size)
				ptr[index] = elem;			

                	else if(index <= size)
                	{

	        		T *temp;
                		temp = new T[capacity];
                		memcpy(temp, ptr, capacity*sizeof(T));

                        	for(int i = index; i < capacity; i++)
                                	ptr[i+1] = temp[i];

                        	ptr[index] = elem;                          //shifts array to insert new element

				delete[] temp;
        			temp = NULL;
                	}

                size++;
        	}
	}

	void replace_at_index(int index, const T& elem)
	{

        	if (index < 0 || index > capacity)
                	cout << "Invalid index";

        	else
        	{
			if (index > size)
				insert(index, elem);

	        	else if (size == 0)
			{				
                		size++;
				ptr[index] = elem;
			}

			else
				ptr[index] = elem;
		}
	}

	void remove_at_index(int index)
	{
		if (index == size)
			size--;
		
		else
		{

        		if(index > capacity || index < 0 || size == 0)
                		cout << "Invalid index";

        		else
        		{
	        		T *temp;
        			temp = new T[capacity];			

        			memcpy(temp, ptr, capacity*sizeof(T));

                		for(int i = index; i < capacity - 1; i++)		//shifts array left to close gap
                        		ptr[i] = temp[i+1];

                		size--;
				delete[] temp;
        			temp = NULL;
        		}
		}
	}
};

template <typename T>
ostream& operator<<(ostream& out, My_vec<T>& vec)	
{
	for (int i = 0; i < vec.get_capacity(); i++)
		out << vec[i] << ", ";

	return out;
}

int main() 
{
	My_vec<int> vec;
	vec.push_back(3);
	vec.push_back(2);
	vec.push_back(7);
	vec.push_back(13);
	vec.push_back(87);
	vec.push_back(1);
	vec.push_back(66);
	vec.push_back(9);
	vec.push_back(21);
	vec.push_back(24);
	vec.push_back(35);
	
	vec.insert(18, 12);
	
	vec.remove_at_index(4);
	vec.replace_at_index(3, 1111);	

	cout << vec << endl;
	vec.insert(3, 333);
	cout << vec << endl;

	return 0;
}
