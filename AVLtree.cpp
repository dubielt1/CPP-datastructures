#include <iostream>
//<algorithm> has std::max()

using namespace std;

template <typename T>
struct bintree
{
	T val;
	bintree *left;
	bintree *right;
	bintree *parent;
	
	bintree(const T &value, bintree *par = NULL) {val = value; parent = par; this->left = NULL; this->right = NULL;}
};

//overload insert & remove so they can be called with only val, absent root
template <typename T> class tree
{
	public:
		bintree <T> *root;

		tree() { root = NULL; }

		void rotateRight(bintree<T> *temp)
		{
			bintree<T> *t = temp->left;

			if (temp == root); //If rotating at root, skip 'else' clauses that access parent node

			else if (temp == temp->parent->left) //change to && temp != root to get rid of above 'if'
				temp->parent->left = t; 

			else
				temp->parent->right = t;

			if (t->right != NULL) //exchange child, if it exists
			{			
				temp->left = t->right; //ternary if exists
				t->right->parent = temp;
			}

			else //if not ^^, temp->left is now NULL, or else it would still = t
				temp->left = NULL;

			t->parent = temp->parent;
			temp->parent = t;
		
			t->right = temp;

			if (temp == root)
				root = t;

			
			cout << "right rotation on " << temp->val << endl;

		}
	
		void rotateLeft(bintree<T> *temp)
		{
			bintree<T> *t = temp->right;

			if (temp == root); //If rotating at root, skip 'else' clauses that access parent node

			else if (temp == temp->parent->left) //ensure temp parent now points to t
                                temp->parent->left = t;

                        else
                                temp->parent->right = t;

			if (t->left != NULL) //exchange child, if it exists
			{
				temp->right = t->left;
				t->left->parent = temp;
			}
			
			else  //if not ^^, temp->right is now NULL, or else it would still = t
				temp->right = NULL;

			t->parent = temp->parent;
			temp->parent = t;

			t->left = temp;

			if (temp == root)
				root = t;

			cout << "left rotation on " << temp->val << endl;

		}	

		void balance(bintree<T>* temp)
		{
			
			//check height(left) - height(right)
			//if 2: va < curr->left->val then left-left case; else left-right case
			//if -2: va > curr->right->val then right-right case; else right-left case

			int balanceFactor = height(temp->left) - height(temp->right);

			//first if statement may be superfluous
			if ( (balanceFactor >= 2) || (balanceFactor <= -2) )
			{

				if (balanceFactor >= 2)
				{
					int balanceChildren = height(temp->left->left) - height(temp->left->right);

					if (balanceChildren >= 0)
						//right rotate temp
						rotateRight(temp);
					else
					{
						//left rotate temp->left
						//right rotate temp
						rotateLeft(temp->left);
						rotateRight(temp);
					}
				}				

				else if (balanceFactor <= -2)
				{

					int balanceChildren = height(temp->right->left) - height(temp->right->right);

					if (balanceChildren <= 0)
						//left rotate temp
						rotateLeft(temp);
					else
					{
						//right rotate temp->right
						//left rotate temp
						rotateRight(temp->right);
						rotateLeft(temp);
					}
				}

			}
		}				

		void insert(const T &va) { insert(root, va); }

		void insert(bintree<T> *temp, const T &va)
		{			
			if (!root)
				temp = root = new bintree<T>(va, NULL);

			else if (va < temp->val)
			{

				if (!temp->left)
					temp->left = new bintree<T>(va, temp);
				else
					insert(temp->left, va); 
			}
			else if (va > temp->val)
			{
				
				if (!temp->right)
					temp->right = new bintree<T>(va, temp);
				else
					insert(temp->right, va); 
			}

			//balance here

			balance(temp);

		}

		bintree<T>* seek(bintree<T> *temp, const T &va)
		{
			if (va == temp->val)
				return temp;

			else if (va < temp->val)
				seek(temp->left, va);

			else if (va > temp->val)
				seek(temp->right, va); 
		
			else
				return temp = NULL;
		}

		//In order successor for deletion of node with two children
		bintree<T>* minimum(bintree<T> *temp)
		{
			if (temp->left)
				minimum(temp->left);
			else return temp;
		}			

		void remove(const T &va) { remove(root, va); }

		void remove(bintree<T> *temp, const T &va)
		{
			bintree<T> *del = seek(temp, va);
			temp = del->parent;
			
			if (!del->left && !del->right)
				//CASE: delete leaf
			{
				if (del == del->parent->left)
					del->parent->left = NULL;
				else if (del == del->parent->right)
					del->parent->right = NULL;

				//delete del;
			}
			else if (!del->left || !del->right)
			{	//CASE: node with one child

				if (!del->left) //del has right child
				{
					del->right->parent = del->parent;
					if (del == del->parent->left)
						del->parent->left = del->right;
					else if (del == del->parent->right)
						del->parent->right = del->right;
				}
				else if (!del->right) //del has left child
				{
					del->left->parent = del->parent;
					if (del == del->parent->left) //del is left child of parent
						del->parent->left = del->left;
					else if (del == del->parent->right) //del is right child of parent
						del->parent->right = del->left;
				}

				//delete del;
			}
			else
			{	//CASE: node with two children
				//inorder successor (smallest value in right subtree)
				bintree<T> *min = minimum(del->right);
			
				//delete the inorder successor
				remove(min, min->val);

				//assign inorder successor val to del
				del->val = min->val;

				//points to deleted node
				min = NULL;
			}

			//balance here, checking all the way up to root
			while (temp != NULL)
			{
				balance(temp);
				temp = temp->parent;

			}



		}
		
		void printpreorder() { printpreorder(root); }

		void printpreorder(bintree<T> *temp)
		{
			if (temp !=NULL)
			{
				cout << temp->val << " ";
				printpreorder(temp->left);
				printpreorder(temp->right);
			}
		}

		void printinorder() { printinorder(root); }

		void printinorder(bintree<T> *temp)
		{	
			if (temp !=NULL)
			{
				printinorder(temp->left);
				cout << temp->val << " ";
				printinorder(temp->right);
			}
		}

		void printpostorder() { printpostorder(root); }

		void printpostorder(bintree<T> *temp)
		{	
			if (temp != NULL)
			{
				printinorder(temp->left);
				printinorder(temp->right);
				cout << temp->val << " ";
			}
		}

		int height(bintree<T> *temp)
		{
			if (temp == NULL)
				return 0;
		
			return max( height(temp->left), height(temp->right) ) + 1;
		}

		void invert() { invert(root); }
		void invert(bintree<T>* temp)
		{
			if ( (temp != NULL) && (temp->left != NULL || temp->right != NULL) )
			{
				invert(temp->left);
				invert(temp->right);
				bintree<T>* placeholder = temp->left;
				temp->left = temp->right;
				//temp->left->val = temp->right->
				temp->right = placeholder;
			}
		}

};

int main()
{

	tree<int> t;
	t.insert(20);
	t.insert(15);
	t.insert(32);
	t.insert(17);
	t.insert(18);
	t.insert(19);

	t.remove(15);
	t.remove(17);

	t.printinorder();
	t.invert();
	t.printinorder();

	return 0;
}
