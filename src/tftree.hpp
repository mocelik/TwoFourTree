/*
 *  Filename:   tftree.hpp
 *
 *  Created on:  Mar. 22, 2020
 *      Author:  orhan
 *
 *  Description:
 *
 */

#ifndef SRC_TFTREE_HPP_
#define SRC_TFTREE_HPP_

#include <memory>
#include <type_traits>
#include <algorithm> // sort

#include <deque> // debug / print
#include <cassert>
#include <sstream>
#include <iostream>

namespace tft {


template<class Key, class Compare = std::less<Key>, class Allocator = std::allocator<Key>>
class TwoFourTree {
public:
	typedef Key key_type;

	/* CONSTRUCTORS */

	// default
	TwoFourTree();
	explicit TwoFourTree(const Compare &comp, const Allocator &alloc =
			Allocator());
	explicit TwoFourTree(const Allocator &alloc);

	// using begin and end iterator
	template<class InputIt>
	TwoFourTree(InputIt first, InputIt last, const Compare &comp = Compare(),
			const Allocator &alloc = Allocator());
	template<class InputIt>
	TwoFourTree(InputIt first, InputIt last, const Allocator &alloc);

	// copy constructors
	TwoFourTree(const TwoFourTree &other);
	TwoFourTree(const TwoFourTree &other, const Allocator &alloc);

	// move constructors
	TwoFourTree(TwoFourTree &&other);
	TwoFourTree(TwoFourTree &&other, const Allocator &alloc);

	// initializer lists
	TwoFourTree(std::initializer_list<typename Allocator::value_type> init,
			const Compare &comp = Compare(), const Allocator &alloc =
					Allocator());
	TwoFourTree(std::initializer_list<typename Allocator::value_type> init,
			const Allocator &alloc);

	 /* DESTRUCTOR */
	~TwoFourTree() = default;

    typedef Compare key_compare;
    typedef Compare value_compare;

	// Allocator
	typedef Allocator allocator_type;
	typedef typename Allocator::value_type value_type;
	typedef typename Allocator::reference reference;
	typedef typename Allocator::const_reference const_reference;
	typedef typename Allocator::difference_type difference_type;
	typedef typename Allocator::size_type size_type;

	// Iterator
	class iterator {
	public:
		typedef typename Allocator::difference_type difference_type;
		typedef typename Allocator::value_type value_type;
		typedef typename Allocator::reference reference;
		typedef typename Allocator::pointer pointer;
		typedef std::bidirectional_iterator_tag iterator_category; //or another tag

		iterator() = default;
		iterator(const iterator&) = default;
		~iterator() = default;

		iterator& operator=(const iterator&) = default;
		bool operator==(const iterator&) const;
		bool operator!=(const iterator&) const;
		bool operator<(const iterator&) const; //optional
		bool operator>(const iterator&) const; //optional
		bool operator<=(const iterator&) const; //optional
		bool operator>=(const iterator&) const; //optional

		iterator& operator++();
		iterator operator++(int); //optional
		iterator& operator--(); //optional
		iterator operator--(int); //optional
		iterator& operator+=(size_type); //optional
		iterator operator+(size_type) const; //optional
//		friend iterator operator+ <>(size_type, const iterator&); //optional
		iterator& operator-=(size_type); //optional
		iterator operator-(size_type) const; //optional
		difference_type operator-(iterator) const; //optional

		reference operator*() const;
		pointer operator->() const;
		reference operator[](size_type) const; //optional
	};
	class const_iterator {
	public:
		typedef typename Allocator::difference_type difference_type;
		typedef typename Allocator::value_type value_type;
		typedef typename Allocator::reference reference;
		typedef typename Allocator::pointer pointer;
		typedef std::bidirectional_iterator_tag iterator_category; //or another tag

		const_iterator() = default;
		const_iterator(const const_iterator&);
		const_iterator(const iterator&);
		~const_iterator() = default;

		const_iterator& operator=(const const_iterator&);
		bool operator==(const const_iterator&) const;
		bool operator!=(const const_iterator&) const;
		bool operator<(const const_iterator&) const; //optional
		bool operator>(const const_iterator&) const; //optional
		bool operator<=(const const_iterator&) const; //optional
		bool operator>=(const const_iterator&) const; //optional

		const_iterator& operator++();
		const_iterator operator++(int); //optional
		const_iterator& operator--(); //optional
		const_iterator operator--(int); //optional
		const_iterator& operator+=(size_type); //optional
		const_iterator operator+(size_type) const; //optional
//		friend const_iterator operator+ <>(size_type, const const_iterator&); //optional
		const_iterator& operator-=(size_type); //optional
		const_iterator operator-(size_type) const; //optional
		difference_type operator-(const_iterator) const; //optional

		reference operator*() const;
		pointer operator->() const;
		reference operator[](size_type) const; //optional
	};

	typedef std::reverse_iterator<iterator> reverse_iterator; //optional
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator; //optional

	/***** MEMBER FUNCTIONS *****/

	/* ITERATOR FUNCTIONS */

	/**
	 * begin() -> return an iterator to the beginning
	 */
	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;

	/**
	 * end() -> return an iterator to an object past the end
	 */
	iterator end() noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;

	/**
	 * rbegin() -> return an iterator to the first element of the reversed set
	 */
	reverse_iterator rbegin() noexcept;
	const_reverse_iterator rbegin() const noexcept;
	const_reverse_iterator crbegin() const noexcept;

	/**
	 * rend() -> return an iterator to the element following the last element of the reversed set.
	 */
	reverse_iterator rend() noexcept;
	const_reverse_iterator rend() const noexcept;
	const_reverse_iterator crend() const noexcept;

	/* CAPACITY FUNCTIONS */

	// true if empty, false otherwise
	[[nodiscard]] bool empty() const noexcept; // c++20

	// Returns the number of elements in the container (std::distance(begin(), end()))
	size_type size() const noexcept;

	// Returns the maximum number of elements possible due to system or library implementation requirements
	// std::distance(begin(), end()) for the largest possible container
	size_type max_size() const noexcept;

	/* MODIFIER FUNCTIONS */

	// clear
	// erases all elements from the container. after this call, size() returns zero.
	void clear() noexcept;

	// insert
	// inserts elements into the container, if the container doesn't already contain an element with an equivalent key
	std::pair<iterator, bool> insert(value_type &&value);
	iterator insert(const_iterator hint, const value_type &value);
	iterator insert(const_iterator hint, value_type &&value);
	template<class InputIt> void insert(InputIt first, InputIt last);
	void insert(std::initializer_list<value_type> ilist);
//	insert_return_type insert(node_type&& nh); // c++17
//	iterator insert(const_iterator hint, node_type&& nh); // c++17

	// emplace
	// insert a new element into the container constructed in-place with the given args
	template<class ... Args>
	std::pair<iterator, bool> emplace(Args &&... args);

	// emplace_hint
	// insert a new element into the container as close as possibel to the position just before hint
	template<class ... Args>
	iterator emplace_hint(const_iterator hint, Args &&...args);

	// erase
	// removes the element at pos
	iterator erase(const_iterator pos); // c++11
	iterator erase(iterator pos); // c++17
	iterator erase(const_iterator first, const_iterator last);
	size_type erase(const key_type &key);

	// swap
	// exchange the contents of the container with those of other
	void swap (TwoFourTree& other) noexcept
				(std::allocator_traits<Allocator>::is_always_equal::value && std::is_nothrow_swappable<Compare>::value);

//	node_type extract(const_iterator position);
//	node_type extract (const key_type& x);

	// attempt to extract ("splice") each element in source and insert into *this using *this's Compare
	// TODO consider also supplying merge taking std::set
	template<class C2>
	void merge(TwoFourTree<Key, C2, Allocator> &source);
	template<class C2>
	void merge(TwoFourTree<Key, C2, Allocator> &&source);

	/* LOOKUP FUNCTIONS */

	// count
	// returns the number of elements with key that compares equivalent to the specified argument
	// which is either 1 or 0 since this container does not allow duplicates
	size_type count(const Key &key) const;
	template<class K> size_type count(const K &x) const;

	// find
	iterator find(const Key &key);
	const_iterator find(const Key &key) const;
	template<class K> iterator find(const K &x);
	template<class K> const_iterator find(const K &x) const;

	// contains
	// true if there is an element with key equivalent to x
	bool contains (const Key& key) const;
	template<class K> bool contains (const K& x) const;

	// equal_range
	std::pair<iterator, iterator> equal_range(const Key& x);
	std::pair<const_iterator, const_iterator> equal_range (const Key& x) const;
	template<class K> std::pair<iterator, iterator> equal_range(const K &x);
	template<class K> std::pair<const_iterator, const_iterator> equal_range(const K &x) const;

	// lower_bound
	// returns an iterator pointing to the first element that is not less than the key
	iterator lower_bound (const Key& x);
	const_iterator lower_bound (const Key&x ) const;
	template <class K> iterator lower_bound(const K&x);
	template <class K> const_iterator lower_bound(const K&x) const;

	// upper_bound
	// returns an iterator pointing to the first element that is greater than key
	iterator upper_bound (const Key& x);
	const_iterator upper_bound (const Key&x ) const;
	template <class K> iterator upper_bound(const K&x);
	template <class K> const_iterator upper_bound(const K&x) const;

	// value_comp, key_comp
	// return function object that compares the values
	value_compare value_comp() const;
	key_compare key_comp() const;

	// debug
	void print() const;
	bool validate();

	protected:
	/**
	 * The internal data structure
	 * Each node contains 1 to 3 Keys
	 * Each node has a parent node (except for the root)
	 * Each node contains 0 to 4 child nodes
	 * Each node is internally sorted
	 */
	class Node {
	private:
		static constexpr const int kMaxNumKeys = 3;
		static constexpr const int kMaxNumChildren = kMaxNumKeys + 1;
		int num_keys_ { 0 };
		std::array<Key, kMaxNumKeys> keys_;
		std::array<std::unique_ptr<Node>, kMaxNumChildren> children_;
		Node *parent_ { nullptr };

	public:
		Node() = default;
		bool isFull();
		bool isLeaf();
		bool containsKey(const Key& k);
		std::pair<Node*, int> findKey(const Key &key);

		std::pair<Node*, int> addValue(Key&& value, std::unique_ptr<Node> &root);
		Key extractValue(int index);
		std::pair<Node*, int> addValueOverflow(Key &&key, std::unique_ptr<Node> &root);

		// debug
		void print();
		void printAll();
		std::string getString();
		bool validateRelationships();

	private:
		explicit Node(Node *parent) :
				parent_(parent) {
		}

		std::pair<Node*, int> addValue(Key&& value);
		void overflowRecursive(Key&& value, std::unique_ptr<Node> childToRight, std::unique_ptr<Node>& root);
	};

	protected:
	Allocator allocator_;
	Compare  comparator_;

	iterator last_node_;
	std::unique_ptr<Node> root_;
};


/**************************************************************************************************************
 *                                          IMPLEMENTATION                                                    *
 **************************************************************************************************************/

template<class Key, class Compare, class Allocator>
TwoFourTree<Key, Compare, Allocator>::TwoFourTree() : root_(nullptr){
	// intentionally unimplemented (yet)
}

template <class K, class C, class A>
bool TwoFourTree<K,C,A>::validate() {
	return root_->validateRelationships();
}

template <class K, class C, class A>
bool TwoFourTree<K,C,A>::Node::validateRelationships() {
	Node * node = this;
	bool rc = true;
	// parent -> child
	std::deque<std::pair<typename TwoFourTree<K,C,A>::Node*, typename TwoFourTree<K,C,A>::Node*>> allNodes;
	allNodes.push_back(std::make_pair(nullptr, node));

	while (!allNodes.empty()) {
		auto check = allNodes.front();
		allNodes.pop_front();
		node = check.second;

		if (check.first != check.second->parent_) {
			rc = false;
			std::cout << "parent of [" << check.second->getString() << "] doesn't have correct parent.\n";
			std::cout << "parent should be: ";
			if (check.first == nullptr)
				std::cout << "nullptr\n";
			else
				std::cout << check.first->getString() << "\n";
			std::cout << "instead it is: ";
			if (check.second->parent_ == nullptr)
				std::cout << "nullptr\n";
			else
				std::cout << check.second->parent_->getString() << "\n";
		}

		for (int i = 0; i < node->num_keys_+1; i++) {
			if (node->children_[i]) {
				allNodes.push_back(std::make_pair(node,node->children_[i].get()));
			}
		}
	} // end loop
	return rc;
}



template<class K, class C, class A>
void TwoFourTree<K,C,A>::print() const{
	root_->printAll();
}

template<class K, class C, class A>
void TwoFourTree<K,C,A>::Node::printAll(){

	Node end_of_level(nullptr);
	Node null_node(nullptr);
	Node children_end(nullptr);

	std::stringstream ss;

	/*
	 * Create a deque to hold the node values
	 * It contains either:
	 * 		1. A node with real data
	 * 		2. A node indicating the end of that level
	 * 		3. An empty node (e.g. 4th child of node with 2 elements)
	 */
	std::deque<Node*> nodes_to_be_printed;

	nodes_to_be_printed.push_back(this);
	nodes_to_be_printed.push_back(&end_of_level);
	nodes_to_be_printed.push_back(&children_end);

	while (!nodes_to_be_printed.empty()) {
		Node *node = nodes_to_be_printed.front();
		nodes_to_be_printed.pop_front();

		if (node == &end_of_level) { // when we come across this, we know we reached the end of this line
			ss << "\n"; // print newline
			nodes_to_be_printed.push_back(&end_of_level); // put it back in so we know for the next iteration
			if (nodes_to_be_printed.front() == &end_of_level) // check if there will even be a next iteration
				break; // if there isn't then we've traversed the entire node
			else
				continue;
		}

		if (node == &null_node) {
			ss << "null:";
			continue;
		}

		if (node == &children_end) {
			ss << "|";
			continue;
		}

		ss << node->getString();

		for (int i=0; i < node->kMaxNumChildren; i++) {
			if (node->children_[i])
				nodes_to_be_printed.push_back(node->children_[i].get());
			else
				nodes_to_be_printed.push_back(&null_node);
		}

		nodes_to_be_printed.push_back(&children_end);
	} // end loop

	std::cout << ss.rdbuf() << std::endl;
}

template <class K, class C, class A>
std::string TwoFourTree<K,C,A>::Node::getString() {
	std::stringstream ss;
	for (int i=0; i < num_keys_-1; i++) {
		ss <<keys_[i] << ",";
	}
	ss << keys_[num_keys_-1] << ":";
	return ss.str();
}

template <class K, class C, class A>
void TwoFourTree<K,C,A>::Node::print() {
	for (int i=0; i < num_keys_; i++) {
		std::cout << "node[" << i << "]: " << keys_[i] << "\t";
	}
	std::cout << std::endl;
}

template<class Key, class C, class A>
bool TwoFourTree<Key,C,A>::contains (const Key& key) const{
	if (!root_)
		return false;

	std::pair<Node*, int> pr = root_->findKey(key);
	return (pr.first != nullptr && pr.second != -1);
}

/**
 * Inserts a value into the tree
 * TODO: Return correct iterator
 */
template<class K, class C, class A>
std::pair<typename TwoFourTree<K,C,A>::iterator, bool> TwoFourTree<K,C,A>::insert(TwoFourTree<K,C,A>::value_type &&value){

	if (!root_) {
		root_.reset(new Node());
		root_->addValue(std::move(value));
		return std::make_pair(TwoFourTree<K,C,A>::iterator(), true);
	}

	std::pair<Node *, int> pr = root_->findKey(value);

	if (pr.first == nullptr || pr.second != -1) // value already exists
		return std::make_pair(TwoFourTree<K, C, A>::iterator(), false);

	pr.first->addValue(std::move(value), root_); // root_ may change
	return std::make_pair(TwoFourTree<K, C, A>::iterator(), true);
}

template<class K, class C, class A>
std::pair<typename TwoFourTree<K,C,A>::Node *, int> TwoFourTree<K,C,A>::Node::addValue(K&& value, std::unique_ptr<Node> &root) {
	assert(isLeaf()); // incorrect usage -> find the leaf using findKey

	if (isFull()) {
		return addValueOverflow(std::move(value), root);
	}

	return addValue(std::move(value));
}


template<class K, class C, class A>
std::pair<typename TwoFourTree<K,C,A>::Node *, int> TwoFourTree<K,C,A>::Node::addValue(K&& value) {
	assert(isLeaf()); // incorrect usage -> find the leaf using findKey
	assert(!isFull()); // use other signature - may need to modify root?

	// find the index to put the value in
	int idx;
	for (idx = 0; idx < num_keys_; idx++) {
		if (keys_[idx] > value)
			break;
	}

	// move the other values out of the way
	for (int k = num_keys_; k > idx; k--) {
		keys_[k] = std::move(keys_[k - 1]);
	}
	keys_[idx] = std::move(value);
	num_keys_++;
	return std::make_pair(this, idx);
}

/**
 * To handle overflow we split the currentNode with 3 values into two nodes with one value each
 * and push the missing 3rd value to the parent
 * Then we put the requested value into either the current node or the newly created one
 *
 * e.g. adding 11 would result in this tree:
 *
 *        [30,                40]
 *       /            |          \
 * [ 13, 15, 17] [33, 35, 37]  [ 43, 45, 47]
 *
 *				becoming:
 *      [15,         30,        40]
 *     /    |         |          \
 * [11,13] [17]  [33, 35, 37]  [ 43, 45, 47]
 */
template <class K, class C, class A>
std::pair<typename TwoFourTree<K,C,A>::Node *, int> TwoFourTree<K,C,A>::Node::addValueOverflow(K &&value, std::unique_ptr<Node> &root) {
	assert(isLeaf());

	// create the new neighbour node ([17] in the example above)
	std::unique_ptr<Node> newRightNode(new Node()); // note: parent may be updated later if a cascaded overflow occurs
	newRightNode->addValue(extractValue(2));

	// extract the middle value to give to parent
	K middleValue = extractValue(1);

	std::pair<Node *, int> value_location;
	if (value < middleValue) {
		value_location = addValue(std::move(value)); // this node now has space so it won't be recursive
	} else {
		value_location = newRightNode->addValue(std::move(value));
	}

	if (parent_ == nullptr) {
		assert(this == root.get());
		std::unique_ptr<Node> newRoot(new Node(nullptr));
		root->parent_ = newRoot.get();
		newRoot->children_[0] = std::move(root);
		root = std::move(newRoot);
	}

	parent_->overflowRecursive(std::move(middleValue), std::move(newRightNode),
			root);
	return value_location;
}

/**
 * Handles overflows coming from one of its children
 * If this node is full, it recursively calls itself, splitting into additional nodes as required
 *
 * Note: tail recursion optimisation is possible here so there is no performance penalty
 */
template <class K, class C, class A>
void TwoFourTree<K,C,A>::Node::overflowRecursive (K&& key, std::unique_ptr<Node> newChild, std::unique_ptr<Node>& root) {

	if (!isFull()){

		// find the index to put the value in
		int idx;
		for (idx = 0; idx < num_keys_; idx++) {
			if (keys_[idx] > key)
				break;
		}

		// move the other values and children out of the way
		for (int k = num_keys_; k > idx; k--) {
			keys_[k] = std::move(keys_[k - 1]);
			children_[k+1] = std::move(children_[k]);
		}
		keys_[idx] = std::move(key);
		num_keys_++;

		newChild->parent_ = this;
		children_[idx+1] = std::move(newChild);
		return;

	} else { // cascaded overflow

		if (parent_ == nullptr) {
			if (this != root.get()) {
				std::cout << "Error, I don't have parent but I'm not the root either\n";
				assert(this == root.get());
			}

			std::unique_ptr<Node> newRoot(new Node(nullptr));
			parent_ = newRoot.get();
			newRoot->children_[0] = std::move(root);
			root = std::move(newRoot);
		}

		if (key < keys_[0]) { // overflow coming from child #0
			std::unique_ptr<Node> newRightNode(new Node(parent_));
			newRightNode->keys_[0] = std::move(keys_[2]);
			newRightNode->num_keys_ = 1;
			if (children_[2]) {
				children_[2]->parent_ = newRightNode.get();
				newRightNode->children_[0] = std::move(children_[2]);
			}
			if (children_[3]) {
				children_[3]->parent_ = newRightNode.get();
				newRightNode->children_[1] = std::move(children_[3]);
			}

			// extract the middle value to give to parent
			K middleValue = std::move(keys_[1]);

			// key < everything so...
			keys_[1] = std::move(keys_[0]);
			keys_[0] = std::move(key);
			num_keys_ = 2;

			newChild->parent_ = this;
			children_[2] = std::move(children_[1]);
			children_[1] = std::move(newChild);

			parent_->overflowRecursive(std::move(middleValue),
					std::move(newRightNode), root);
			return;

		} else if (key < keys_[1]) { // overflow coming from child #1

			std::unique_ptr<Node> newRightNode(new Node(parent_));
			newRightNode->keys_[0] = std::move(keys_[2]);
			newRightNode->num_keys_ = 1;
			if (children_[2]) {
				children_[2]->parent_ = newRightNode.get();
				newRightNode->children_[0] = std::move(children_[2]);
			}
			if (children_[3]) {
				children_[3]->parent_ = newRightNode.get();
				newRightNode->children_[1] = std::move(children_[3]);
			}

			// extract the middle value to give to parent
			K middleValue = std::move(keys_[1]);

			keys_[1] = std::move(key);
			num_keys_ = 2;

			newChild->parent_ = this;
			children_[2] = std::move(newChild);

			parent_->overflowRecursive(std::move(middleValue),
					std::move(newRightNode), root);
			return;

		} else if (key < keys_[2]) { // overflow coming from child #2
			std::unique_ptr<Node> newRightNode(new Node(parent_));
			newRightNode->keys_[0] = std::move(key);
			newRightNode->keys_[1] = std::move(keys_[2]);
			newRightNode->num_keys_ = 2;
			if(children_[2]) {
				children_[2]->parent_ = newRightNode.get();
				newRightNode->children_[0] = std::move(children_[2]);
			}
			if (children_[3]) {
				children_[3]->parent_ = newRightNode.get();
				newRightNode->children_[2] = std::move(children_[3]);
			}
			newChild->parent_ = newRightNode.get();
			newRightNode->children_[1] = std::move(newChild);

			// extract the middle value to give to parent
			K middleValue = std::move(keys_[1]);

			num_keys_ = 1;
			// remaining parent keys and children stay as is

			parent_->overflowRecursive(std::move(middleValue),
					std::move(newRightNode), root);
			return;
		} else { // overflow coming from child #3
			std::unique_ptr<Node> newRightNode(new Node(parent_));
			newRightNode->keys_[0] = std::move(keys_[2]);
			newRightNode->keys_[1] = std::move(key);
			newRightNode->num_keys_ = 2;
			if (children_[2]) {
				children_[2]->parent_ = newRightNode.get();
				newRightNode->children_[0] = std::move(children_[2]);
			}
			if (children_[3]) {
				children_[3]->parent_ = newRightNode.get();
				newRightNode->children_[1] = std::move(children_[3]);
			}
			newChild->parent_ = newRightNode.get();
			newRightNode->children_[2] = std::move(newChild);

			// extract the middle value to give to parent
			K middleValue = std::move(keys_[1]);

			num_keys_ = 1;
			// remaining parent keys and children stay as is

			parent_->overflowRecursive(std::move(middleValue),
					std::move(newRightNode), root);
			return;
		}
	}
}


template<class Key, class C, class A>
Key TwoFourTree<Key,C,A>::Node::extractValue(int index) {
	assert(index > 0 && index < num_keys_);
	assert(isLeaf()); // do not call this on an internal node

	Key k = std::move(keys_[index]);

	for (int i=index; i < num_keys_ - 1; i++)
		keys_[i] = std::move(i+1);

	num_keys_--;
	return std::move(k);
}

/**
 * Traverses the children trying to find specified key
 * Returns the node that contains it and the index of the key
 *
 * If the key doesn't exist, it will return the node that should
 * have contained it (guaranteed to be a leaf node) and -1 as index
 */
template<class Key, class C, class A>
std::pair<typename TwoFourTree<Key,C,A>::Node *, int> TwoFourTree<Key,C,A>::Node::findKey (const Key& key){
	const int doesntContain = -1;
	Node * currentNode = this;

	while (!currentNode->isLeaf()) {

		for (int i = 0; i < currentNode->num_keys_; i++) { // iterate over each key / child
			if (key < currentNode->keys_[i]) {
				currentNode = currentNode->children_[i].get();
				goto foundCorrectChild; // sue me

			} else if (key == currentNode->keys_[i]) {
				return std::make_pair(this, i); // stop looking
			}
		}

		// if we make it here then value is greater than everything in node. Next node is to the right.
		currentNode = currentNode->children_[currentNode->num_keys_].get();

	foundCorrectChild:
		assert (currentNode != nullptr); // property of TwoFourTree - internal nodes must have num_keys + 1 children
		continue;
	}

	for (int i=0; i < currentNode->num_keys_; i++)
		if (key == currentNode->keys_[i])
			return std::make_pair(currentNode, i);

	return std::make_pair(currentNode, doesntContain);
}

template<class K, class C, class A>
bool TwoFourTree<K,C,A>::Node::isFull() {
	return (num_keys_ == kMaxNumKeys);
}

template<class K, class C, class A>
bool TwoFourTree<K,C,A>::Node::isLeaf() {
	for (int i = 0; i < kMaxNumChildren; i++) {
		if (children_[i])
			return false;
	}
	return true;
}

template<class  K, class C, class A>
bool TwoFourTree<K,C,A>::Node::containsKey (const K& k) {
	for (int i=0; i < num_keys_; i++)
		if (keys_[i] == k)
			return true;
	return false;
}


} // namespace tft
template <class Key, class Compare, class Alloc>
bool operator== (const tft::TwoFourTree<Key,Compare,Alloc>& lhs, const tft::TwoFourTree<Key,Compare,Alloc>& rhs);

template <class Key, class Compare, class Alloc>
bool operator!= (const tft::TwoFourTree<Key,Compare,Alloc>& lhs, const tft::TwoFourTree<Key,Compare,Alloc>& rhs);

template <class Key, class Compare, class Alloc>
bool operator< (const tft::TwoFourTree<Key,Compare,Alloc>& lhs, const tft::TwoFourTree<Key,Compare,Alloc>& rhs);

template <class Key, class Compare, class Alloc>
bool operator<= (const tft::TwoFourTree<Key,Compare,Alloc>& lhs, const tft::TwoFourTree<Key,Compare,Alloc>& rhs);

template <class Key, class Compare, class Alloc>
bool operator> (const tft::TwoFourTree<Key,Compare,Alloc>& lhs, const tft::TwoFourTree<Key,Compare,Alloc>& rhs);

template <class Key, class Compare, class Alloc>
bool operator>= (const tft::TwoFourTree<Key,Compare,Alloc>& lhs, const tft::TwoFourTree<Key,Compare,Alloc>& rhs);

// TODO: consider implementing three way compare
//template <class Key, class Compare, class Alloc>
//? operator<=> (const tft::TwoFourTree<Key,Compare,Alloc>& lhs, const tft::TwoFourTree<Key,Compare,Alloc>& rhs);



#endif /* SRC_TFTREE_HPP_ */
