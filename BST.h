#pragma once
#ifndef _BST_h
#define _BST_h
#include "String.h"
#include <assert.h>
template <typename T>
struct BST {
private:
	struct Node {
		T name;
		Node* left;
		Node* right;
		Node* parent;

		Node(T name) {
			this->name = name;
			left = right = parent = nullptr;
		}

		~Node(void) {
			delete left;
			delete right;
		}
	};

private:

	static Node* recursiveCopy(Node* root) {
		if (root == nullptr) { return 0; }

		Node* result = new Node(root->name);
		result->name = root->name;
		result->parent = nullptr;

		Node* left = recursiveCopy(root->left);
		Node* right = recursiveCopy(root->right);

		result->left = left;
		result->right = right;
		if (left != nullptr) { left->parent = result; }
		if (right != nullptr) { right->parent = result; }

		return result;
	}



	static Node* successor(Node* p) {
		if (p->right != 0) { // p has a right subtree
							 /* successor to p will be smallest node in right subtree */
			Node* next = p->right;
			while (next->left != 0) {
				next = next->left;
			}
			return next;
		} else { // p does not have a right subtree
				 /* successor to p will be closest ancestor where we go up a left branch */
			Node* parent = p->parent;
			while (parent != 0 && parent->right == p) {
				p = parent;
				parent = p->parent;
			}
			return parent;
		}
	}

	void remove(Node* p) {
		if (p == 0) { // WTF?
			return;
		}

		if (p->right == nullptr) { // easy case! 
			Node* child = p->left;
			Node* parent = p->parent;
			if (parent == nullptr) {
				if (child != nullptr) {
					child->parent = nullptr;
				} 
				root = child;
				p->left = p->right = nullptr;
				delete p;
				return;
			}
			if (p == parent->left) {
				parent->left = child;
			} else {
				parent->right = child;
			}
			if (child != nullptr) {
				child->parent = parent;
			}
			delete p;
		} else { // nasty case! 
			Node* victim = successor(p);
			p->name = victim->name;
			Node* parent = victim->parent;
			Node* child = victim->right;
			if (parent->left == victim) {
				parent->left = child;
			} else {
				parent->right = child;
			}
			if (child != nullptr) {
				child->parent = parent;
			}
			victim->right = victim->left = nullptr;
			delete victim;
		}
	}

	static Node* find(Node* root, T name) {
		while (root != 0 && root->name != name) {
			if (name < root->name) {
				root = root->left;
			}
			else {
				root = root->right;
			}
		}
		return root;
	}

	Node* root;

public:

	BST(void) { root = nullptr; }
	~BST(void) {
		delete root;
	}

	BST(const BST& that) {
		root = recursiveCopy(that.root);
	}

	BST& operator=(const BST& that) {
		if (this != &that) {
			delete root;
			root = recursiveCopy(that.root);
		}
		return *this;
	}

	bool isIn(T name) {	//NOT SURE IF THIS WORKS YET
		Node* p = find(root, name);
		if (p == nullptr) {
			return false;
		}
		return true;
	}
	T* findValue(T name) {

		Node* p = find(root, name);
		if (p != nullptr) {
			return &(p->name);
		}
		else {
			return nullptr;
		}
	}

	void insert(T name) {
		Node* child = new Node(name);

		if (root == nullptr) {
			root = child;
			return;
		}

		Node* parent = nullptr;
		Node* p = root;
		while (p != nullptr) {
			parent = p;
			if (name < p->name) {
				p = p->left;
			} else {
				p = p->right;
			}
		}

		if (name < parent->name) {
			assert(parent->left == nullptr);
			parent->left = child;
		} else {
			assert(parent->right == nullptr);
			parent->right = child;
		}
		child->parent = parent;
	}

	void remove(T name) {
		Node* p = find(root, name);
		remove(p);
	}
	/*
	void print(const char* spec) {
		if (root == nullptr) { return; } // special case for empty tree


		Node* p;

		// set p to smallest 
		p = root;
		while (p->left != nullptr) {
			p = p->left;
		}

		const char* prefix = "";
		while (p != nullptr) {
			printf("%s", prefix);
			printf(spec, p->name);
			prefix = ", ";
			p = successor(p);
		}
		printf("\n");

	}*/
};


#endif /* _BST_h */
