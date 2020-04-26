/*
 *  Filename:   iter_operations.ipp
 *
 *  Created on:  Apr. 26, 2020
 *      Author:  orhan
 *
 *  Description:
 *  	Contains the implementations for iterator operations
 *
 */

#ifndef SRC_ITER_OPERATIONS_IPP_
#define SRC_ITER_OPERATIONS_IPP_

namespace tft {

template<class K, class C, class A>
bool TwoFourTree<K,C,A>::const_iterator::operator==(const const_iterator& rhs) const {
	return (node_ == rhs.node_ && idx_ == rhs.idx_);
}

template<class K, class C, class A>
typename TwoFourTree<K,C,A>::const_reference TwoFourTree<K,C,A>::iterator::operator*() const {
	assert(idx_ < (node_->num_keys_));
	return node_->keys_.at(idx_);
}

template<class K, class C, class A>
typename TwoFourTree<K,C,A>::const_iterator& TwoFourTree<K,C,A>::const_iterator::operator++() {
	assert (idx_ <= node_->num_keys_);

	{ // SPECIAL CASES
		if (idx_ == node_->num_keys_) { // if this is the end_iterator
			// we do not need to define this behaviour and could let it crash and burn
			// but lets be nice and return without doing anything instead
			return *this;
		}

		if (idx_ == -1) { // if this is the rbegin iterator
			idx_ = 0;
			// node_ should already be correctly set
			return *this;
		}
	}

	if (node_->children_[idx_+1]) { // if there is a child after this key
		// go all the way to the leaf
		const Node * node = node_->children_[idx_+1].get();
		while (!node->isLeaf()) {
			node = node->children_[0].get();
		}
		node_ = node;
		idx_ = 0;
		return *this;

	} else if (idx_ < (node_->num_keys_ - 1)) { // no child -> go right if there is something to the right
		assert(node_->isLeaf());
		++idx_;
		return *this;

	} else if (idx_ == node_->num_keys_ - 1) { // no child, nothing to the right -> go up

		// need a loop in case we are right-most child and need to go up multiple levels
		// possibly all the way up to root. If we DO get to the root, set the iterator
		// back to the original node and set the idx to one-past-end
		const Node *begin_node = node_;
		while (true) {
			const Node *parent = node_->parent_;

			// check if we are root
			if (parent == nullptr) {
				// set equal to past_end_iterator
				node_ = begin_node;
				idx_ = node_->num_keys_;
				// leave node_ as is so that operator-- is valid
				return *this;
			}

			// If this node is not the biggest child then next value is parent at idx [child idx]
			for (int i = 0; i < parent->num_keys_; i++) {
				if (parent->children_[i].get() == node_) {
					node_ = parent;
					idx_ = i;
					return *this;
				}
			}

			// node_ is the biggest child of parent_
			// repeat same process on upper level
			assert(parent->children_[parent->num_keys_].get() == node_);
			node_ = parent;
			continue;
		}
	}

	// shouldn't reach here
	assert(false); // unhandled case
	return *this;
}

template<class K, class C, class A>
typename TwoFourTree<K,C,A>::const_iterator& TwoFourTree<K,C,A>::const_iterator::operator--() {
	assert(idx_ <= node_->num_keys_);

	{ // SPECIAL CASES
		if (idx_ == node_->num_keys_) { // if this is the end_iterator
			idx_ = node_->num_keys_ -1;
			return *this;
		}

		if (idx_ == -1) { // if this is the rbegin iterator
			// don't need to define this behaviour
			// lets just not do anything
			return *this;
		}
	}

	if (node_->children_[idx_]) { // if there is a child before this key
		// go all the way to the leaf
		const Node *node = node_->children_[idx_].get();
		while (!node->isLeaf()) {
			node = node->children_[node->num_keys_].get();
		}
		node_ = node;
		idx_ = node_->num_keys_ - 1;
		return *this;

	} else if (idx_ > 0) { // no child -> go left if there is something to the left
		assert(node_->isLeaf());
		--idx_;
		return *this;

	} else if (idx_ == 0) { // no child, nothing to the left -> go up

	// need a loop in case we are left-most child and need to go up multiple levels
	// possibly all the way up to root. If we DO get to the root, set the iterator
	// back to the original node and set the idx to -1
		const Node *begin_node = node_;
		while (true) {
			const Node *parent = node_->parent_;

			// check if we are root
			if (parent == nullptr) {
				// set equal to before-begin
				node_ = begin_node;
				idx_ = -1;
				return *this;
			}

			// If this node is not the smallest child then next value is parent at idx [child idx - 1]
			for (int i = 1; i <= parent->num_keys_; i++) {
				if (parent->children_[i].get() == node_) {
					node_ = parent;
					idx_ = i - 1;
					return *this;
				}
			}

			// node_ is the smallest child of parent_
			// repeat same process on upper level
			assert(parent->children_[0].get() == node_);
			node_ = parent;
			continue;
		}
	}

	// shouldn't reach here
	assert(false); // unhandled case
	return *this;
}


template<class K, class C, class A>
typename TwoFourTree<K,C,A>::const_iterator TwoFourTree<K,C,A>::const_iterator::operator+(TwoFourTree<K,C,A>::size_type s) const {
	const_iterator rc (*this); // copy ctor
	for (auto i = 0; i < s; i++)
		++rc;
	return rc;
}

template<class K, class C, class A>
typename TwoFourTree<K,C,A>::const_iterator TwoFourTree<K,C,A>::const_iterator::operator-(TwoFourTree<K,C,A>::size_type s) const {
	const_iterator rc (*this); // copy ctor
	for (auto i = 0; i < s; i++)
		--rc;
	return rc;
}

template<class K, class C, class A>
bool TwoFourTree<K,C,A>::const_iterator::operator!=(const const_iterator& rhs) const {
	return !(*this == rhs);
}

} /* namespace tft */

#endif /* SRC_ITER_OPERATIONS_IPP_ */
