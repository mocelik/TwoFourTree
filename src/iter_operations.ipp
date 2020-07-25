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

	(*this) = node_->getSuccessor(idx_);
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

	(*this) = node_->getPredecessor(idx_);
	return *this;
}


template<class K, class C, class A>
typename TwoFourTree<K,C,A>::const_iterator TwoFourTree<K,C,A>::const_iterator::operator+(TwoFourTree<K,C,A>::size_type s) const {
	const_iterator rc (*this); // copy ctor
	for (TwoFourTree<K,C,A>::size_type i = 0; i < s; i++)
		++rc;
	return rc;
}

template<class K, class C, class A>
typename TwoFourTree<K,C,A>::const_iterator TwoFourTree<K,C,A>::const_iterator::operator-(TwoFourTree<K,C,A>::size_type s) const {
	const_iterator rc (*this); // copy ctor
	for (TwoFourTree<K,C,A>::size_type i = 0; i < s; i++)
		--rc;
	return rc;
}

template<class K, class C, class A>
bool TwoFourTree<K,C,A>::const_iterator::operator!=(const const_iterator& rhs) const {
	return !(*this == rhs);
}

} /* namespace tft */

#endif /* SRC_ITER_OPERATIONS_IPP_ */
