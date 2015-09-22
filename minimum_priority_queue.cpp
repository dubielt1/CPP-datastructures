#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <string>

using namespace std;

template <typename T>
class item
{
	int key;
	T element;
	public:
		item() : key(int()), element(T()) {}
		item(int k, T e) : key(k), element(e) {}
		int getkey() { return key; }
		void setkey(int k) { key = k; }
		T& getel() { return element; }
		int operator*() { return key; }

		item& operator=(item<T>& rhs)
		{
			key = rhs.getkey();
			element = rhs.getel();
			return *this;
		}
};

template <typename T>
class isLess			//comparator class
{
	public:
	bool operator()(T& e1, T& e2) { return e1 < e2; }
	bool operator()(item<T>& i1, item<T>& i2) { return *i1 < *i2; }
};

template <typename T>
class vectorheap
{
	vector<T> V;
	
	public:
		vectorheap() : V(1, T()) {}	//created w/ one element, so items/nodes begin at index 1, not 0

		typedef typename vector<T>::iterator Position;

		int size() const { return (V.size() - 1); }  //-1 because of 'sentinal' node at index 0

		Position left(const Position& p) { return pos(2*idx(p)); }

		Position right(const Position& p) { return pos(2*idx(p) + 1); }

		Position parent(const Position& p) { return pos(idx(p)/2); }

		bool hasLeft(const Position& p) const { return 2*idx(p) <= size(); }

		bool hasRight(const Position& p) const { return 2*idx(p) + 1 <= size(); }

		bool isRoot(const Position& p) const { return idx(p) == 1; }

		Position root() { return pos(1); }

		Position last() { return pos(size()); }

		void addLast(const T& e) { V.push_back(e); }

		void removeLast() { V.pop_back(); }

		void swap(const Position& p, const Position& q) { T e = *q; *q = *p; *p = e; }

		void printq()
		{
			typename vector<T>::iterator i;
			cout << "The Queue structure: \n";
			for (i = V.begin()+1; i != V.end(); i++)
				cout << *i << '\n';
		}

		Position search(int k)
		{
			typename vector<T>::iterator i;
			int idx = 1;
			for (i = V.begin()+1; **i != k; i++)
				idx++;
			return pos(idx);
		}			

	protected:

		Position pos(int i) { return V.begin() + i; }
		int idx(const Position& p) const { return p - V.begin(); }
};

template <typename T, typename C>
class heapMPQ
{
	vectorheap<T> Tr;
	C isLess;

	typedef typename vectorheap<T>::Position Position;	

	public:
		int size() const { return Tr.size(); }

		bool empty() const { return size() == 0; }

		void print() { Tr.printq(); }

		T& min() { return *(Tr.root()); }

		void insert(const T& e)
		{
			Tr.addLast(e);				//add to the bottom and "bubble up"
			Position v = Tr.last(); 		//v is position of e.. position is an iterator for underlying vector
			while (!Tr.isRoot(v))			//up-heap bubbling
			{
				Position u = Tr.parent(v); 	//u is parent of v
				if (!isLess(*v, *u)) break; 	//if u(parent) < v(child), break loop, ordering is correct
				Tr.swap(v, u); 			//if v(child) < u(parent), bubble v up
				v = u;				// v is now equal to its parent, bubbling up the heap for next loop of while
			}
		}

		void deckey(int initk, int fink)  //can decrease an item's key a.k.a. move it up the heap
		{
			Position v = Tr.search(initk);

			(*v).setkey(fink);
			
			while (!Tr.isRoot(v))
			{
				Position u = Tr.parent(v);
				if (!isLess(*v, *u))
					 break;

				Tr.swap(v, u); 	//swaps values iterators point to 
				v = u;		//v is now equal to its parent, moving up the heap for next loop of while
			}
		}
		
		void removeMin()
		{
			if (size() == 1)
				Tr.removeLast(); //removes minimum

			else
			{
				Position u = Tr.root();
				Tr.swap(u, Tr.last());	//last value is now at minumum(root) position
				Tr.removeLast();	//moves minimum value to last then pops
							//u still represents root, but with its new value
				
				while (Tr.hasLeft(u))   //down-heap bubbling
				{
					Position v = Tr.left(u);
					if (Tr.hasRight(u) && isLess(*(Tr.right(u)), *v)) //right < left
						v = Tr.right(u);


					//ensures smallest value moves up to root, making v either root left or root right
					//then swapping appropriate values				
	

					if (isLess(*v, *u)) //child < parent
					{
						Tr.swap(u, v); // child swapped with parent
						u = v;		//parent moves down to child position 
					}
					else break;
				}
			}
		}
};

template <typename T, typename C>
class listMPQ
{
	list<T> L;
	C isLess;

	public:
		int size() const { return L.size(); }

		bool empty() const { return L.size() == 0; }

		void print() //Prints the queue structure
		{
			typename list<T>::iterator i;
			cout << "The Queue structure: \n";
			for (i = L.begin(); i != L.end(); i++)
				cout << *i << '\n';
		}				

		void insert(T& e) //insertion sort method
		{
			typename list<T>::iterator p;
			p = L.begin();	
			while (p != L.end() && !isLess(e, *p)) ++p;
			L.insert(p, e); //inserts e in the position before p [e, p] because p > e
		}
		T& min() { return L.front(); }

		void removeMin() { L.pop_front(); } //0 comparisons on removal
};

template <typename T>
ostream& operator<<(ostream& os, item<T>& i)
{
	os << i.getkey() << " " << i.getel();
	return os;
}

int main()
{

	heapMPQ<item<string>, isLess<string> > hcity;
	listMPQ<item<string>, isLess<string> > lcity;



	string city;
	int dist = 0;

	ifstream file("file path here");
	
	getline(file, city);
	for(; !file.eof(); getline(file, city) )
	{
		file >> ws;
		file >> dist;
		file >> ws;
		item<string> i = item<string>(dist, city);
		hcity.insert(i);
		lcity.insert(i);
	}

	file.close();

	cout << "\n\n Heap based priority queue\n\n";
	for (;!hcity.empty();)
	{
		cout << hcity.min() << endl;
		hcity.removeMin();
	}

	cout << "\n\n List based priority queue\n\n";
	lcity.print();

	return 0;
}
