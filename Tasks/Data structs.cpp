#include <iostream>
#include <algorithm>
#include <ranges>
#include <functional>
#include <cmath>
#include <string>
#include <cassert>
#include <random> 
#include <vector>

using namespace std;


//-----Realization of a linked list-----
//node of a linked kist
template <typename T>
struct Node
{
	T val;
	Node* next = nullptr;

	Node(){}
	Node(T v) : val{ v } {};
	~Node() {};
};

//linked list class
template<typename T>
class List
{
	//the pointer of a first list element
	Node<T>* _first = nullptr;

public:

	//find a pointer of an element by its value, if its contains
	Node<T>* find(T target_val)
	{
		if (!_first) return nullptr;

		auto p = _first;
		while (p)
		{
			if (p->val == target_val) return p;
			p = p->next;
		}

		return nullptr;
	}

	//insert an element in list
	void insert(T v)
	{
		Node<T>* new_node = new Node(v);
		new_node->next = _first;
		_first = new_node;
	}

	//return a pointer of previous of an element whose value is passed, if its contains
	Node<T>* prev(T v)
	{
		if (!_first || !_first->next) return nullptr;

		auto p = _first;
		while (p)
		{
			if ((p->next)->val == v) return p;
			p = p->next;
		}

		return nullptr;
	}

	//delete an element from the list by its value, if its contains
	void erase(T v)
	{
		if (!_first) return;

		auto res_find = find(v);

		if (!res_find) return;

		if (res_find == _first)
		{
			auto next_first = _first->next;
			delete _first;
			_first = next_first;
			return;
		}

		auto prev_res_find = prev(v);
		auto next_res_find = res_find->next;
		delete prev_res_find->next;
		prev_res_find->next = next_res_find;
	}

	//show the list (cout)
	void show()
	{
		auto p = _first;
		while (p)
		{
			cout << p->val << ' ';
			p = p->next;
		}
		cout << endl;
	}

	List() {}
	~List()
	{
		while (_first->next)
		{
			cout << "Node with value " << _first->next->val << " deleted from list successfully." << endl;
			this->erase(_first->next->val);
		}
		delete _first;
	}


};


//-----Realization of a binary search tree-----
//a 'leaf' of binary tree
template<typename T>
struct Leaf
{
	T val;
	Leaf* parent = nullptr;
	Leaf* left = nullptr;
	Leaf* right = nullptr;

	Leaf(){}
	Leaf(T v): val{v} {}
	~Leaf(){}
};

//a binary search tree class
template<typename T>
class Bin_tree
{
	Leaf<T>* _root = nullptr;

	//erase all tree
	Leaf<T>* make_empty(Leaf<T>* t)
	{
		if (!t) return nullptr;

		make_empty(t->left);
		make_empty(t->right);
		delete t;

		return nullptr;
	}

	//return a min element of the tree
	Leaf<T>* min(Leaf<T>* t)
	{
		if (!t) return nullptr;
		else if (!t->left) return t;
		else return min(t->left);
	}

	//return a max element of the tree
	Leaf<T>* max(Leaf<T>* t)
	{
		if (!t) return nullptr;
		else if (!t->right) return t;
		else return max(t->right);
	}

	//insert an element in the tree
	Leaf<T>* insert(T x, Leaf<T>* t)
	{
		if (!t) t = new Leaf<T>(x);
		else if (x < t->val) t->left = insert(x, t->left);
		else if (x > t->val) t->right = insert(x, t->right);
		return t;
	}
	
	//remove an element from the tree
	Leaf<T>* remove(T x, Leaf<T>* t) {
		Leaf<T>* temp;
		if (!t) return nullptr;

		else if (x < t->val)
			t->left = remove(x, t->left);

		else if (x > t->val)
			t->right = remove(x, t->right);

		else if (t->left && t->right)
		{
			temp = min(t->right);
			t->val = temp->val;
			t->right = remove(t->val, t->right);
		}
		else
		{
			temp = t;
			if (!t->left) t = t->right;
			else if (!t->right) t = t->left;
			delete temp;
		}

		return t;
	}

	//show the tree (cout)
	void traverse(Leaf<T>* t) {
		if (!t) return;
		traverse(t->left);
		cout << t->val << " ";
		traverse(t->right);
	}

public:
	//find a pointer of an element by its value, if its contains
	Leaf<T>* find(T target_val)
	{
		if (!_root) return nullptr;
		auto p = _root;
		while (p)
		{
			if (p->val == target_val) return p;
			else if (p->val < target_val) p = p->left;
			else p = p->right;
		}
		return nullptr;
	}

	//return a min element of the tree
	Leaf<T>* min()
	{
		return min(_root);
	}

	//return a max element of the tree
	Leaf<T>* max()
	{
		return max(_root);
	}

	//insert an element in the tree
	void insert(T x)
	{
		_root = insert(x, _root);
	}

	//remove an element from the tree
	void remove(T x)
	{
		_root = remove(x, _root);
	}

	//show the tree (cout)
	void show()
	{
		traverse(_root);
		cout << endl;
	}

	//constructors and destructors
	Bin_tree(){}
	~Bin_tree()
	{
		_root = make_empty(_root);
	}

};

//-----Realization of a non-decreasing heap-----
//non-decreasing heap class
template<typename T>
struct PQ
{
	//quantity of elements of the heap
	size_t _q_size = 0;
	//body of the heap
	vector<T> _q;

	//swap 2 elements by its numbers
	void _swap(size_t i, size_t j)
	{
		swap(_q[i], _q[j]);
	}

	//elevate an element from position k to nessesary hierarchical position
	void _bubble_up(size_t k)
	{
		int par_k = parent(k);
		if (par_k == -1) return;//root
		if (_q[par_k] > _q[k]) _swap(k, par_k);
		_bubble_up(par_k);
	}

	//drop an element from position k to nessesary hierarchical position
	void _bubble_down(size_t k)
	{ 
		size_t
			//index of one of the children
			c = young_child(k),
			//index of the least child
			min_index = k;
		for (size_t i = 0; i <= 1; ++i)
		{
			if ((c + i < _q_size) && (_q[min_index] > _q[c + i])) min_index = c + i;
		}
		if (min_index != k)
		{
			_swap(k, min_index);
			_bubble_down(min_index);
		}
	}

	//return position of the parent for node with position k
	int parent(size_t k)
	{
		return k == 0 ? -1 : k / 2;
	}

	//return position of the left child for node with position k
	size_t young_child(size_t k)
	{
		return 2 * k;
	}

	//insert element x in the heap
	void insert(T x)
	{
		assert(_q_size <= _q.size());
		++_q_size;
		_q.push_back(x);
		_bubble_up(_q_size - 1);
	}

	//make heap by vector of elements
	void make(vector<T> s)
	{
		for (size_t i = 0; i < s.size(); ++i) this->insert(s[i]);
	}

	//fast creating of the heap
	void make_fast(vector<T> s)
	{
		for (size_t i = 0; i < s.size(); ++i) _q.push_back(s[i]);
		for (int i = _q_size - 1; i >= 0; --i) _bubble_down(i);
	}

	//return and delete a min element from the heap
	T extract_min()
	{
		assert(_q_size >= 0);
		T min = _q[0];
		_q[0] = _q[_q_size - 1];
		--_q_size;
		_bubble_down(0);
		return min;
	}

	//constrictos and destructors
	PQ(){}
	PQ(size_t new_size): _q_size{new_size} {}
	~PQ(){}
};


template<typename T>
int heap_compare(PQ<T> q, size_t i, int count, T x)
{
	assert(count >= 0 || i >= q._q_size);
	if (q._q[i] < x)
	{
		count = heap_compare(q, q.young_child(i), count - 1, x);
		count = heap_compare(q, q.young_child(i) + 1, count, x);
	}
	return count;
}



//-----Realization of heapsort-----
template<typename T>
void heapsort(vector<T>& s)
{
	PQ<T> q;
	q.make(s);
	for (size_t i = 0; i < s.size(); ++i) s[i] = q.extract_min();
}

//-----Realization of mergesort-----
// merge 2 sorted subarrays `s[low…mid]` è `s[mid+1…high]`
template<typename T>
void merge(vector<T>& s, size_t low, size_t mid, size_t high)
{
	vector<T> buffer1, buffer2;

	for (size_t i = low; i <= mid; ++i) buffer1.push_back(s[i]);
	for (size_t i = mid + 1; i <= high; ++i) buffer2.push_back(s[i]);

	size_t i = 0, j = 0, k =low, sz1 = mid - low + 1, sz2 = high - mid;

	while (i < sz1 && j < sz2)
	{
		if (buffer1[i] <= buffer2[j]) s[k++] = buffer1[i++];
		else s[k++] = buffer2[j++];
	}

	while (i < sz1) s[k++] = buffer1[i++];
	while (j < sz2) s[k++] = buffer2[j++];
}

//-----Mergsort-----
template<typename T>
void mergesort(vector<T>& s, size_t low, size_t high)
{
	if (high <= low) return;

	size_t mid = (low + ((high - low) / 2));

	mergesort(s, low, mid);          
	mergesort(s, mid + 1, high);

	merge(s, low, mid, high);        
}


//-----Realization of quicksort-----
//divides an array by 2 subarrays and
//returns the index of the element relative to which the split was performed
template<typename T>
int partition(vector<T>& s, int begin, int end)
{
	auto pivot = s[end];
	int pivot_index = begin;

	for (size_t i = begin; i < end; ++i)
	{
		if (s[i] <= pivot)
		{
			swap(s[i], s[pivot_index++]);
		}
	}

	swap(s[pivot_index], s[end]);
	return pivot_index;
}

//-----Quicksort-----
template<typename T>
void quicksort(vector<T>& s, int start, int end)
{
	if (start < end)
	{
		int p = partition(s, start, end);
		quicksort(s, start, p - 1);
		quicksort(s, p + 1, end);
	}
}


//-----Binary search-----
template<typename T>
int binary_search(vector<T> const& s, T key)
{
	size_t left = 0, right = s.size() - 1;
	while (left <= right)
	{
		size_t mid = left + (right - left) / 2;
		if (s[mid] == key) return mid;
		else if (s[mid] < key) left = mid + 1;
		else right = mid - 1;
	}

	return -1;
}

int main()
{
	mt19937_64 rng(random_device{}());
	uniform_int_distribution<int> unid(-99, 99);

	vector<int> m(20), n{71, 46, 79, 1, 57, 16, -67, -46, -54, 86, -57, 61, 73, -60, -58, 86, 21, 75, -69, -70};

	for (auto& x : m)
		x = unid(rng);

	int key = unid(rng);

	quicksort(m, 0, m.size() - 1);

	//for (size_t i = 0; i < n.size(); ++i) cout << n[i] << endl;
	size_t i = 0;
	while(i++ < 100)
	{
		cout << binary_search(m, key) << endl;
		cout << binary_search(m.begin(), m.end(), key) << endl;
		cout << "key: " << key << endl;
		cout << "---" << endl;
		key = unid(rng);
	}

	//heapsort(v);

	//PQ<int> p;
	//p.make_fast(v);
	//cout << "res heap_compare: " << heap_compare(p, 5, 1, 5899) << endl;
	//mergesort(m, 0, m.size() - 1);

	/*for (size_t i = 0; i < m.size(); ++i)
	{
		cout << n[i] << endl;
	}*/

	//quicksort(m, 0, m.size() - 1);

	/*for (size_t i = 0; i < m.size(); ++i)
	{
		cout << m[i] << endl;
	}*/


	//cout << is_sorted(m.begin(), m.end()) << endl;

}
