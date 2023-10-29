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


//-----Реализация односвязного списка-----//
//звено односвязного списка
template <typename T>
struct Node
{
	T val;
	Node* next = nullptr;

	Node(){}
	Node(T v) : val{ v } {};
	~Node() {};
};

/// @brief односвязный список
template<typename T>
class List
{
	//указатель на первый элемент списка
	Node<T>* _first = nullptr;

public:

	//найти указатель на элемент по его значению, если такой есть
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

	//добавить элемент в список
	void insert(T v)
	{
		Node<T>* new_node = new Node(v);
		new_node->next = _first;
		_first = new_node;
	}

	//возвращает указатель на предыдущий к элементу, чье значение передается, если такой есть
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

	//удалить элемент по его значению, если такой есть
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

	//вывести список в cout
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


//-----Реализация двоичного дерева поиска-----
//'лист' двоичного дерева
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

//класс, реализующий структуру двоичного дерева
template<typename T>
class Bin_tree
{
	Leaf<T>* _root = nullptr;

	Leaf<T>* make_empty(Leaf<T>* t)
	{
		if (!t) return nullptr;

		make_empty(t->left);
		make_empty(t->right);
		delete t;

		return nullptr;
	}

	Leaf<T>* min(Leaf<T>* t)
	{
		if (!t) return nullptr;
		else if (!t->left) return t;
		else return min(t->left);
	}

	Leaf<T>* max(Leaf<T>* t)
	{
		if (!t) return nullptr;
		else if (!t->right) return t;
		else return max(t->right);
	}

	Leaf<T>* insert(T x, Leaf<T>* t)
	{
		if (!t) t = new Leaf<T>(x);
		else if (x < t->val) t->left = insert(x, t->left);
		else if (x > t->val) t->right = insert(x, t->right);
		return t;
	}

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

	void traverse(Leaf<T>* t) {
		if (!t) return;
		traverse(t->left);
		cout << t->val << " ";
		traverse(t->right);
	}

public:
	//найти указатель на элемент по его значению, если такой есть
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

	//возвращает указатель на минимальный элемент дерева
	Leaf<T>* min()
	{
		return min(_root);
	}

	//возвращает указатель на максимальный элемент дерева
	Leaf<T>* max()
	{
		return max(_root);
	}

	void insert(T x)
	{
		_root = insert(x, _root);
	}

	void remove(T x)
	{
		_root = remove(x, _root);
	}

	void show()
	{
		traverse(_root);
		cout << endl;
	}

	Bin_tree(){}
	~Bin_tree()
	{
		_root = make_empty(_root);
	}

};

template<typename T>
bool compare(T x, T y)
{
	return x > y ? 1 : 0;
}



//-----Реализация неубывающей пирамиды-----
//класс неубывающей пирамиды
template<typename T>
struct PQ
{
	//количество элементов очереди
	size_t _q_size = 0;
	//тело очереди
	vector<T> _q;

	//меняет местами содержимое, находящееся на i-ой и j-ой позиции в пирамиде 
	void _swap(size_t i, size_t j)
	{
		//_q[i] = x, _q[j] = y
		//_q[j] = _q[j] ^ (_q[i] = _q[i] ^ (_q[j] = _q[i] ^ _q[j]));

		T temp;
		temp = _q[i];
		_q[i] = _q[j];
		_q[j] = temp;
	}

	//подянть элемент с позиции k в нужную иерархическую позицию
	void _bubble_up(size_t k)
	{
		int par_k = parent(k);
		if (par_k == -1) return;//корень
		if (_q[par_k] > _q[k]) _swap(k, par_k);
		_bubble_up(par_k);
	}

	//опустить элемент с позиции k в нужную иерархическую позицию
	void _bubble_down(size_t k)
	{ 
		size_t
			//индекс потомка
			c = young_child(k),
			//индекс наименьшего потомка
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

	/////////////
	//возвращает позицию предка для узла с номером k
	int parent(size_t k)
	{
		return k == 0 ? -1 : k / 2;
	}

	//возвращает позицию левого потомка для узла с номером k
	size_t young_child(size_t k)
	{
		return 2 * k;
	}

	//добавить элемент x в пирамиду, сохранив иерархию пирамиды
	void insert(T x)
	{
		assert(_q_size <= _q.size());
		++_q_size;
		_q.push_back(x);
		_bubble_up(_q_size - 1);
	}

	//задать пирамиду массивом элементов
	void make(vector<T> s)
	{
		for (size_t i = 0; i < s.size(); ++i) this->insert(s[i]);
	}

	//алгоритм "быстрого" создания пирамиды
	void make_fast(vector<T> s)
	{
		for (size_t i = 0; i < s.size(); ++i) _q.push_back(s[i]);
		for (int i = _q_size - 1; i >= 0; --i) _bubble_down(i);
	}

	//возвращает и удаляет минимальный элемент пирамиды
	T extract_min()
	{
		assert(_q_size >= 0);
		T min = _q[0];
		_q[0] = _q[_q_size - 1];
		--_q_size;
		_bubble_down(0);
		return min;
	}

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



//-----Реализация пирамидальной сортировки-----
template<typename T>
void heapsort(vector<T>& s)
{
	PQ<T> q;
	q.make(s);
	for (size_t i = 0; i < s.size(); ++i) s[i] = q.extract_min();
}

//-----Реализация сортировки слиянием-----
// Объединяем два отсортированных подмассива `s[low…mid]` и `s[mid+1…high]`
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

//-----Сортировка слиянием-----
template<typename T>
void mergesort(vector<T>& s, size_t low, size_t high)
{
	if (high <= low) return;

	size_t mid = (low + ((high - low) / 2));

	mergesort(s, low, mid);          
	mergesort(s, mid + 1, high);

	merge(s, low, mid, high);        
}


//-----Реализация быстрой сортировки-----
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


//-----Двоичный поиск-----
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
