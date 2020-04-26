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

#include <set>
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
	TwoFourTree() = default;
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

	// typedefs
	typedef Allocator allocator_type;
	typedef typename Allocator::value_type 		value_type;
	typedef typename Allocator::size_type 		size_type;
	typedef typename Allocator::difference_type	difference_type;
	typedef typename Allocator::reference 		reference;
	typedef typename Allocator::pointer 		pointer;
	typedef typename Allocator::const_reference const_reference;
	typedef typename Allocator::const_pointer 	const_pointer;

protected:
	class Node;
public:
	class const_iterator {
	public:
		// redefine as part of iterator so we can make use of std::reverse_iterator<const_iterator>
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef TwoFourTree::value_type value_type;
		typedef TwoFourTree::difference_type difference_type;
		typedef TwoFourTree::pointer pointer;
		typedef TwoFourTree::const_reference reference;
		typedef TwoFourTree::const_reference const_reference;
		typedef TwoFourTree::const_pointer 	const_pointer;


		void print() const;
		std::string getString() const;
		friend std::ostream& operator<<(std::ostream& os, const const_iterator& it) {
			os << it.getString();
			return os;
		}

		const_iterator() = default;
		const_iterator(const const_iterator&) = default;
		const_iterator& operator=(const const_iterator& other) = default;
		const_iterator& operator=(const_iterator&& other) = default;
		~const_iterator() = default;

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

		const_reference operator*() const;
		const_pointer operator->() const;
		const_reference operator[](size_type) const; //optional

		friend TwoFourTree<Key,Compare,Allocator>;
	private:
		const_iterator(Node * node, int idx) : node_(node), idx_(idx) {}
		const_iterator(std::pair<Node *, int> pair) : node_(pair.first), idx_(pair.second) {}
		const_iterator(std::pair<const Node *, int> pair) : node_(pair.first), idx_(pair.second) {}
		const Node *node_ { nullptr };
		int idx_ { -1 };
	};

	typedef const_iterator iterator; // do not allow modification

	typedef std::reverse_iterator<iterator> reverse_iterator; //optional
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator; //optional

	/***** MEMBER FUNCTIONS *****/

	/* ITERATOR FUNCTIONS */

	/**
	 * begin() -> return an iterator to the beginning
	 */
	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;

	/**
	 * end() -> return an iterator to an object past the end
	 */
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;

	/**
	 * rbegin() -> return an iterator to the first element of the reversed set
	 */
	const_reverse_iterator rbegin() const noexcept;
	const_reverse_iterator crbegin() const noexcept;

	/**
	 * rend() -> return an iterator to the element following the last element of the reversed set.
	 */
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
	iterator erase(iterator pos);
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
	std::pair<const_iterator, const_iterator> equal_range (const Key& x) const;
	template<class K> std::pair<const_iterator, const_iterator> equal_range(const K &x) const;

	// lower_bound
	// returns an iterator pointing to the first element that is not less than the key
	const_iterator lower_bound (const Key&x ) const;
	template <class K> const_iterator lower_bound(const K&x) const;

	// upper_bound
	// returns an iterator pointing to the first element that is greater than key
	const_iterator upper_bound (const Key&x ) const;
	template <class K> const_iterator upper_bound(const K&x) const;

	// value_comp, key_comp
	// return function object that compares the values
	value_compare value_comp() const;
	key_compare key_comp() const;

	// debug
	std::string getString() const;
	void print() const;
	bool validate() const;
	friend std::ostream& operator<<(std::ostream& os, const TwoFourTree& tree) {
		os << tree.getString();
		return os;
	}

	protected:
	/**
	 * The internal data structure
	 * Each node contains 1 to 3 Keys
	 * Each node has a parent node (except for the root)
	 * Each node contains 0 to 4 child nodes
	 * Each node is internally sorted
	 */
	class Node {
		friend const_iterator;
	private:
		static constexpr const int kMaxNumKeys = 3;
		static constexpr const int kMaxNumChildren = kMaxNumKeys + 1;
		int num_keys_ { 0 };
		std::array<Key, kMaxNumKeys> keys_;
		std::array<std::unique_ptr<Node>, kMaxNumChildren> children_;
		Node *parent_ { nullptr };

	public:
		Node() = default;
		bool isFull() const;
		bool isLeaf() const;
		Node* getParent() const;
		bool isLargestChild() const;
		bool containsKey(const Key& k);
		std::pair<Node*, int> findKey(const Key &key);
		std::pair<const Node*, int> findLargest() const;

		std::pair<Node*, int> addValue(Key&& value, std::unique_ptr<Node> &root);
		Key extractValue(int index);
		std::pair<Node*, int> addValueOverflow(Key &&key, std::unique_ptr<Node> &root);

		// debug
		void print() const;
		void printAll() const;
		std::string getString() const;
		std::string getStringAll() const;
		bool validateRelationships() const;
		friend std::ostream& operator<<(std::ostream &os, const Node &n) {
			os << n.getString();
			return os;
		}

	private:
		explicit Node(Node *parent) :
				parent_(parent) {
		}

		std::pair<Node*, int> addValue(Key&& value);
		void overflowRecursive(Key&& value, std::unique_ptr<Node> childToRight, std::unique_ptr<Node>& root);
	};

	protected:
	std::unique_ptr<Node> root_;

	iterator begin_iterator_;
	iterator end_iterator_;

	Allocator allocator_;
	Compare  comparator_;
};
} /* namespace tft */



/**************************************************************************************************************
 *                                          IMPLEMENTATION                                                    *
 **************************************************************************************************************/

#include "debug_operations.ipp"
#include "node_operations.ipp"
#include "iter_operations.ipp"

namespace tft {

template<class Key, class C, class A>
bool TwoFourTree<Key,C,A>::contains (const Key& key) const{
	if (!root_)
		return false;

	std::pair<Node*, int> pr = root_->findKey(key);
	return (pr.first != nullptr && pr.second != -1);
}

/**
 * Inserts a value into the tree
 */
template<class K, class C, class A>
std::pair<typename TwoFourTree<K,C,A>::iterator, bool> TwoFourTree<K,C,A>::insert(TwoFourTree::value_type &&value){

	if (!root_) {
		root_.reset(new Node());
		begin_iterator_ = (root_->addValue(std::move(value), root_));
		end_iterator_ = begin_iterator_ + 1;
		return std::make_pair(begin_iterator_, true);
	}

	std::pair<Node *, int> pr = root_->findKey(value);

	if (pr.first == nullptr || pr.second != -1) // value already exists
		return std::make_pair(iterator(pr), false);

	if (pr.first == end_iterator_.node_) { // this node contains the maximum value so we need special handling
		pr = pr.first->addValue(std::move(value), root_);

		// the maximum may be in the same node or it may have moved to a neighbour
		// search for it starting from the parent to ensure the correct node is found
		if (pr.first->getParent() != nullptr) {
			end_iterator_ = iterator(pr.first->getParent()->findLargest()) + 1;
		} else {
			end_iterator_ = iterator(pr.first->findLargest()) + 1;
		}

		return std::make_pair(iterator(pr), true);
	}

	pr = pr.first->addValue(std::move(value), root_);

	return std::make_pair(iterator(pr), true);
}

template<class K, class C, class A>
typename TwoFourTree<K,C,A>::const_iterator TwoFourTree<K,C,A>::begin() const noexcept {
	return cbegin();
}

template<class K, class C, class A>
typename TwoFourTree<K,C,A>::const_iterator TwoFourTree<K,C,A>::cbegin() const noexcept {
	return begin_iterator_;
}

template<class K, class C, class A>
typename TwoFourTree<K,C,A>::const_iterator TwoFourTree<K,C,A>::end() const noexcept {
	return cend();
}
template<class K, class C, class A>
typename TwoFourTree<K,C,A>::const_iterator TwoFourTree<K,C,A>::cend() const noexcept {
	return end_iterator_;
}

template<class K, class C, class A>
typename TwoFourTree<K,C,A>::const_reverse_iterator TwoFourTree<K,C,A>::rbegin() const noexcept {
	return crbegin();
}
template<class K, class C, class A>
typename TwoFourTree<K,C,A>::const_reverse_iterator TwoFourTree<K,C,A>::crbegin() const noexcept {
	return const_reverse_iterator(end_iterator_);
}
template<class K, class C, class A>
typename TwoFourTree<K,C,A>::const_reverse_iterator TwoFourTree<K,C,A>::rend() const noexcept {
	return crend();
}
template<class K, class C, class A>
typename TwoFourTree<K,C,A>::const_reverse_iterator TwoFourTree<K,C,A>::crend() const noexcept {
	return const_reverse_iterator(begin_iterator_);
}


} // namespace tft

#endif /* SRC_TFTREE_HPP_ */
