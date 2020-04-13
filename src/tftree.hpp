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

	protected:
	/**
	 * The internal data structure
	 * Each node contains 1 to 3 Keys
	 * Each node has a parent node (except for the root)
	 * Each node contains 0 to 4 child nodes
	 * Each node is internally sorted
	 */
	class Node {
		static constexpr const int kMaxNumKeys = 3;
		static constexpr const int kMaxNumChildren = kMaxNumKeys + 1;
		int num_keys_ { 0 };
		std::array<Key, kMaxNumKeys> keys_;
		std::array<std::unique_ptr<Node>, kMaxNumChildren> children_;
		Node *parent_ { nullptr };

	public:
		explicit Node(Node * parent) :parent_(parent)
		{
		}
		Node() = default;

		bool isFull();
		bool isLeaf();
		bool containsKey(const Key& k);
		std::pair<Node*, bool> getNodeContainingKey(const Key &key);

		Node& getParent();

		bool addValue(Key&& value);
		Key extractValue(int index);
		std::unique_ptr<Node> extractChild(int index);
		bool addChild(std::unique_ptr<Node> newChild);

		void addWithOverflow(Key&& value, std::unique_ptr<Node> childToRight);

		// debug
		void print();
		void printAll();
		std::string getString();

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
	return root_->getNodeContainingKey(key).second;
}

/**
 * Inserts a value into the tree
 * TODO: insertion logic for overflow
 * TODO: Return correct iterator
 */
template<class K, class C, class A>
std::pair<typename TwoFourTree<K,C,A>::iterator, bool> TwoFourTree<K,C,A>::insert(TwoFourTree<K,C,A>::value_type &&value){

	if (!root_) {
		root_.reset(new Node(nullptr));
		root_->addValue(std::move(value));
		return std::make_pair(TwoFourTree<K,C,A>::iterator(), true);
	}

	std::pair<Node *, bool> pr = root_->getNodeContainingKey(value);
	if (pr.second) // value already exists
		return std::make_pair(TwoFourTree<K, C, A>::iterator(), false);

	Node* currentNode = pr.first;

	if (!currentNode) {// when might this occur?
		std::cout << "insert: currentNode is nullptr\n";
		return std::make_pair(TwoFourTree<K,C,A>::iterator(), false);
	}

	if (!currentNode->isFull()) {
		currentNode->addValue(std::move(value));
		return std::make_pair(TwoFourTree<K,C,A>::iterator(), true);
	} else {

		if (currentNode == root_.get()) {
			std::unique_ptr<Node> nr(new Node(nullptr));
			nr->addChild(std::move(root_));
			root_ = std::move(nr);
		}

		// to handle overflow we split the currentNode with 3 values into two nodes with one value each
		// and push the missing 3rd value to the parent

		// e.g. adding 11 here would result in:
		//        [30,                40]
		//       /            |          \
		// [ 13, 15, 17] [33, 35, 37]  [ 43, 45, 47]

		//      [15,     30,        40]
		//     /    |        |          \
		// [13]    [17]  [33, 35, 37]  [ 43, 45, 47]

		// create the new [17] node
		std::unique_ptr<Node> newRightNode (new Node(&currentNode->getParent()));
		newRightNode->addValue (currentNode->extractValue(2));

		// extract the middle value to give to parent
		K middleValue = currentNode->extractValue(1);

		currentNode->addWithOverflow(std::move(middleValue), std::move(newRightNode));

		// finally there must be enough space to put the value in
		Node& parent = currentNode->getParent();
		std::pair<Node*, bool> parentReturn = parent.getNodeContainingKey(value);

		assert(!parentReturn.second);
		currentNode = parentReturn.first;

		// TODO:
		// can we determine which child the value is going to go to before we call addWithOverflow?
		// won't it be either currentNode or newRightNode?
		currentNode->addValue(std::move(value));

		return std::make_pair(TwoFourTree<K, C, A>::iterator(), true);
	}
}

template<class K, class C, class A>
bool TwoFourTree<K,C,A>::Node::addValue(K&& value) {

	if (isFull()) {// TODO: overflow
		std::cout << "addValue: check if node is full before blinding adding value!\n";
		return false;
	}

	if (!isLeaf()) {// not sure how to handle children
		std::cout << "addValue: don't call addValue on an internal node!\n";
		return false;
	}
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
	return true;
}

/**
 *
 */
template <class K, class C, class A>
void TwoFourTree<K,C,A>::Node::addWithOverflow (K&& key, std::unique_ptr<Node> newChild) {
	newChild->parent_ = parent_;

	int myIndex = -1;
	for (int i=0; i <= parent_->num_keys_; i++) {
		if (parent_->children_[i].get() == this) {
			myIndex = i;
			break;
		}
	}

	assert(myIndex != -1);

	if (!parent_->isFull()){
		for (int i=parent_->num_keys_; i > myIndex; i--) {
			parent_->keys_[i] = std::move(parent_->keys_[i-1]);
			parent_->children_[i+1] = std::move(parent_->children_[i]);
		}
		parent_->keys_[myIndex] = std::move(key);
		parent_->num_keys_++;
		parent_->children_[myIndex+1] = std::move(newChild);
		return;

	} else { // cascaded overflow
		std::cout << "TODO: Cascaded overflow" << std::endl;
	}
}


template<class Key, class C, class A>
Key TwoFourTree<Key,C,A>::Node::extractValue(int index) {
	if (index < 0 || index >= num_keys_) {
		std::cout << "extractValue: index out of bounds! index = " << index << std::endl;
		return Key(); // TODO: index out of bounds
	}

	if (!isLeaf()) {
		std::cout << "extractValue: called on an internal node!\n";
		return Key(); // TODO: logic error
	}

	Key k = std::move(keys_[index]);

	for (int i=index; i < num_keys_ - 1; i++)
		keys_[i] = std::move(i+1);

	num_keys_--;
	return std::move(k);
}

/**
 * Traverses the children trying to find value
 * Returns a pair: a boolean indicating whether the value exists in the node or not,
 * and a pointer to a node that would contain the value if it did exist
 */
template<class Key, class C, class A>
std::pair<typename TwoFourTree<Key,C,A>::Node *, bool> TwoFourTree<Key,C,A>::Node::getNodeContainingKey (const Key& key){
	Node * currentNode = this;

	while (!currentNode->isLeaf()) {

		for (int i = 0; i < currentNode->num_keys_; i++) { // iterate over each key / child
			if (key < currentNode->keys_[i]) {
				if (!currentNode->children_[i]) { // if child doesn't exist yet
					currentNode->children_[i].reset(new Node(currentNode)); // create child

					// child must be leaf, stop looking
					return std::make_pair(currentNode->children_[i].get(), false);
				}
				currentNode = currentNode->children_[i].get();
				goto foundCorrectChild; // sue me

			} else if (key == currentNode->keys_[i]) {
				// stop looking
				return std::make_pair(this, true);
			}
		}

		// if we make it here then value is greater than everything in node. Next node is to the right.
		{
			const int rightmost_child_idx = currentNode->num_keys_;
			if (!currentNode->children_[rightmost_child_idx]) { // if child doesn't exist yet
				Node *newNode = new Node(currentNode);
				currentNode->children_[rightmost_child_idx].reset(newNode); // create child

				// child must be leaf, stop looking
				return std::make_pair(currentNode->children_[rightmost_child_idx].get(), false);
			}
			currentNode = currentNode->children_[rightmost_child_idx].get();
		}

	foundCorrectChild:
		continue;
	}

	return std::make_pair(currentNode, currentNode->containsKey(key));
}


template<class K, class C, class A>
std::unique_ptr<typename TwoFourTree<K,C,A>::Node> TwoFourTree<K,C,A>::Node::extractChild(int index) {
	std::cout << "extractChild: dangerous function to use, deprecate this ASAP\n";
	return std::move(children_[index]);
}

//
template<class K, class C, class A>
bool TwoFourTree<K,C,A>::Node::addChild(std::unique_ptr<Node> newChild){
	K& minChildValue = newChild->keys_[0];

	for (int i=0; i < num_keys_; i++) {
		if (minChildValue < keys_[i]) { // i shall be the child index
			if (children_[i]) {// child already exists
				std::cout << "Logic error: Trying to add child but similar exists\n";
				return false;
			}

			newChild->parent_ = this;
			children_[i] = std::move(newChild);
			return true;
		}
	}
	// child is bigger than all

	// check if there is already a child here
	if (children_[num_keys_]) {
		std::cout << "Logic error: Trying to add child but similar exists 2\n";
		return false;
	}

	newChild->parent_ = this;
	children_[num_keys_] = std::move(newChild);
	return true;
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

template <class K, class C, class A>
typename TwoFourTree<K,C,A>::Node& TwoFourTree<K,C,A>::Node::getParent() {
	return *parent_;
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
