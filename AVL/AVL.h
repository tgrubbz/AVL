#pragma once
#include <string>

template <class T>
class AVL
{
public:
	AVL();
	~AVL();

	void insert(T);
	void clear();
	std::string serialize();
	void remove(T);

private:
	class node
	{
	public:
		node(T);
		~node();

		T v;
		node * l = nullptr;
		node * r = nullptr;
		int height = 0;
	};

	node * root = nullptr;
	void insert(node **, T);
	void rebalance(node **);
	void height(node **);
	int balanceFactor(node **);
	void rr_rot(node **);
	void rl_rot(node **);
	void ll_rot(node **);
	void lr_rot(node **);
	void clear(node **);
	void serialize(node **, std::string *);
	void remove(node **, T);
	T predecessor(node **);
};

template<class T>
AVL<T>::AVL()
{
}

template<class T>
AVL<T>::~AVL()
{
	clear();
}

template<class T>
AVL<T>::node::node(T data)
{
	v = data;
}

template<class T>
AVL<T>::node::~node()
{
}

template<class T>
void AVL<T>::insert(T data)
{
	insert(&root, data);
}

template<class T>
void AVL<T>::clear()
{
	clear(&root);
}

template<class T>
void AVL<T>::clear(node ** n)
{
	if (*n == nullptr)
	{
		return;
	}

	clear(&((*n)->l));
	clear(&((*n)->r));

	delete (*n);
	*n = nullptr;
}

template<class T>
std::string AVL<T>::serialize()
{
	std::string s = "";
	if (root == nullptr)
	{
		return s;
	}
	serialize(&root, &s);
	return s;
}

template<class T>
void AVL<T>::serialize(node ** n, std::string * s)
{
	if (*n == nullptr)
	{
		*s += "/";
		return;
	}

	*s += std::to_string((*n)->v);
	serialize(&((*n)->l), &(*s));
	serialize(&((*n)->r), &(*s));
}

template<class T>
void AVL<T>::remove(T data)
{
	remove(&root, data);
}

template<class T>
void AVL<T>::remove(node ** n, T data)
{
	if (*n == nullptr)
	{
		return;
	}
	else if (data < (*n)->v)
	{
		remove(&((*n)->l), data);
	}
	else if (data > (*n)->v)
	{
		remove(&((*n)->r), data);
	}
	else
	{
		if ((*n)->l == nullptr && (*n)->r == nullptr)
		{
			delete (*n);
			*n = nullptr;
			return;
		}
		else if ((*n)->l == nullptr)
		{
			node * t = (*n);
			(*n) = (*n)->r;
			delete t;
			return;
		}
		else if ((*n)->r == nullptr)
		{
			node * t = (*n);
			(*n) = (*n)->l;
			delete t;
			return;
		}
		else
		{
			data = predecessor(n);
			(*n)->v = data;
			remove(&((*n)->l), data);
		}
	}

	height(&(*n));
	rebalance(&(*n));
}

template<class T>
T AVL<T>::predecessor(node ** n)
{
	node * t = (*n)->l;
	while (t->r)
	{
		t = t->r;
	}
	return t->v;
}

template<class T>
void AVL<T>::rebalance(node ** n)
{
	int bf = balanceFactor(n);
	if (bf < -1)
	{
		if(balanceFactor(&((*n)->r)) < 0)
		{
			// right right rotation
			rr_rot(&(*n));
		}
		else
		{
			// right left rotation
			rl_rot(&(*n));
		}
	}
	else if (bf > 1)
	{
		if (balanceFactor(&((*n)->l)) > 0)
		{
			// left left rotation
			ll_rot(&(*n));
		}
		else
		{
			// left right rotation
			lr_rot(&(*n));
		}
	}
}

template<class T>
void AVL<T>::height(node ** n)
{
	if (*n == nullptr)
	{
		return;
	}

	if ((*n)->l != nullptr && (*n)->r != nullptr)
	{
		(*n)->height = 1 + std::max((*n)->l->height, (*n)->r->height);
	}
	else if ((*n)->l != nullptr)
	{
		(*n)->height = 1 + (*n)->l->height;
	}
	else if ((*n)->r != nullptr)
	{
		(*n)->height = 1 + (*n)->r->height;
	}
	else
	{
		(*n)->height = 0;
	}
}

template<class T>
int AVL<T>::balanceFactor(node ** n)
{
	if (*n == nullptr)
	{
		return 0;
	}

	if ((*n)->l != nullptr && (*n)->r != nullptr)
	{
		return (*n)->l->height - (*n)->r->height;
	}
	else if ((*n)->l != nullptr)
	{
		return (*n)->l->height - (-1);
	}
	else
	{
		return (-1) - (*n)->r->height;
	}
}

template<class T>
void AVL<T>::rr_rot(node ** n)
{
	// Set a temp to the tree's right node
	node * t = (*n)->r;

	// Set the tree's right node to temp's left node
	(*n)->r = t->l;

	// Set temp's left node to the tree's root
	t->l = *n;

	*n = t;
	height(&((*n)->l));
}

template<class T>
void AVL<T>::rl_rot(node ** n)
{
	ll_rot(&((*n)->r));
	height(&((*n)->r));
	rr_rot(&(*n));
}

template<class T>
void AVL<T>::ll_rot(node ** n)
{
	// Set a temp to the tree's left node
	node * t = (*n)->l;

	// Set the tree's left node to temp's right node
	(*n)->l = t->r;

	// Set temp's left node to the tree's root
	t->r = *n;

	*n = t;
	height(&((*n)->r));
}

template<class T>
void AVL<T>::lr_rot(node ** n)
{
	rr_rot(&((*n)->l));
	height(&((*n)->l));
	ll_rot(&(*n));
}

template<class T>
void AVL<T>::insert(node ** n, T data)
{
	if (*n == nullptr)
	{
		*n = new node(data);
		return;
	}
	else if (data < (*n)->v)
	{
		// Recursive call to insert into the left sub tree		
		insert(&((*n)->l), data);
	}
	else if (data >(*n)->v)
	{
		// Recursive call to insert into the right sub tree		
		insert(&((*n)->r), data);
	}
	else
	{
		// Value found in the tree so stop
		return;
	}

	height(&(*n));
	rebalance(&(*n));
}
