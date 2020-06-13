/*
 *  Filename:   node_operations.ipp
 *
 *  Created on:  Apr. 18, 2020
 *      Author:  orhan
 *
 *  Description:
 *		Contains the implementations of Node methods, which are the essential tree operations
 */

#ifndef SRC_NODE_OPERATIONS_IPP_
#define SRC_NODE_OPERATIONS_IPP_

namespace tft {

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
			assert(this == root.get());

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
	assert(index >= 0 && index < num_keys_);
	assert(isLeaf()); // do not call this on an internal node

	Key k = std::move(keys_[index]);

	for (int i=index; i < num_keys_ - 1; i++)
		keys_[i] = std::move(keys_[i+1]);

	num_keys_--;
	return std::move(k);
}


template<class Key, class C, class A>
std::pair<const typename TwoFourTree<Key,C,A>::Node*, int>  TwoFourTree<Key,C,A>::Node::removeValue(
		const Key& value, std::unique_ptr<Node>& root) {
	assert(this == root.get()); // for now...

	auto r = findWithRemove(value); // returns the leaf node and index (which should be r.first->num_keys_-1 UNLESS value was already in leaf)
	if (r.first == nullptr || r.second == -1) // doesn't exist
		return r;
	assert(r.first->isLeaf());
	assert(r.first->num_keys_ > 1 || r.first == root.get());
	r.first->extractValue(r.second);
	return std::make_pair(r.first, r.second-1);
}

/**
 * Logical one step traversal down the tree in search of the specified key.
 *
 * returns a pair<Node*, bool>.
 * 	The Node* points to the next node in the direction of key.
 * 	The bool returns true if the key is in the current node, in which case the Node* will be this.
 *
 */
template<class K, class C, class A>
std::pair<typename TwoFourTree<K,C,A>::Node*, int>  TwoFourTree<K,C,A>::Node::traverse_step(const K& sought_key){
	for (int i = 0; i < num_keys_; i++) {
		if (sought_key < keys_[i]) {
			return std::make_pair(children_[i].get(), -1);
		} else if (sought_key == keys_[i]) {
			return std::make_pair(this, i);
		}
	}
	return std::make_pair(children_[num_keys_].get(), -1);
}

template<class K, class C, class A>
int TwoFourTree<K,C,A>::Node::getKeyIndex(const K& key){
	for (int i = 0; i < num_keys_; i++)
		if (keys_[i] == key)
			return i;
	return -1;
}

template<class Key, class C, class A>
std::pair<typename TwoFourTree<Key,C,A>::Node*, int>  TwoFourTree<Key,C,A>::Node::findWithRemove(
		const Key& key) {

	std::pair<Node*, int> original_key_location { nullptr, -1 };

	// traverse the tree until the key is found
	// note that makeThreeNode is intentionally not called on *this
	auto traverse_iter = this->traverse_step(key);
	while (!traverse_iter.first->isLeaf() && traverse_iter.second == -1) { // first is the node, second is false if traverse succeeded
		traverse_iter.first = traverse_iter.first->makeThreeNode();
		traverse_iter = traverse_iter.first->traverse_step(key);
	}
	if (traverse_iter.first->parent_ != nullptr) // if not root
		traverse_iter.first = traverse_iter.first->makeThreeNode();

	assert (original_key_location.first == nullptr && original_key_location.second == -1);

	original_key_location = std::make_pair(traverse_iter.first, traverse_iter.first->getKeyIndex(key));
	if (original_key_location.second == -1) // key doesn't exist
		return std::make_pair(nullptr, -1);

	// Now the key is found.
	// We need to keep traversing until we get to the predecessor node
	// Then we swap the key with the predecessor, and return the predecessor location

	if (original_key_location.first->isLeaf()) {
		return original_key_location;
	}

	auto pred = original_key_location.first->getPredecessor(original_key_location.second);
	pred.first = pred.first->makeThreeNode();
	pred.second = pred.first->num_keys_ - 1;

	// TODO: optimize...
	// current solution always checks if makeThreeNode moved the key down to the predecessor node
	// if it isn't, we should be able to move to a new while loop that doesn't check for that anymore
	while (!pred.first->isLeaf()) {
		pred = pred.first->getPredecessor(pred.second);
		pred.first = pred.first->makeThreeNode();
		if (pred.first->getKeyIndex(key) == -1)
			pred.second = pred.first->num_keys_ - 1;
		else {
			pred.second = pred.first->getKeyIndex(key);
			original_key_location = pred;
		}
	}

	int key_idx = pred.first->getKeyIndex(key);
	if (key_idx != -1) {
		return std::make_pair(pred.first, key_idx);
	} else {
		std::swap(pred.first->keys_[pred.second],
				original_key_location.first->keys_[original_key_location.second]);
		return pred;
	}
}

template<class K, class C, class A>
typename TwoFourTree<K,C,A>::Node* TwoFourTree<K,C,A>::Node::makeThreeNode() {
	if (num_keys_ != 1)
		return this;
	assert(parent_ != nullptr);

	int my_idx = getMyChildIdx();
	if (my_idx > 0 && parent_->children_[my_idx - 1]->num_keys_ >= 2) {
		return transferFromLeft();
	}
	if (my_idx < (parent_->num_keys_)
			&& parent_->children_[my_idx + 1]->num_keys_ >= 2) {
		return transferFromRight();
	}

	if (parent_->num_keys_ > 1) {
		return fusion();
	}

	return shrink();
}

template<class K, class C, class A>
typename TwoFourTree<K,C,A>::Node * TwoFourTree<K,C,A>::Node::transferFromRight() {

	int my_idx = getMyChildIdx();
	std::unique_ptr < Node > &right = parent_->children_[my_idx + 1];

	// transfer from right to parent and *this
	keys_[num_keys_] = std::move(parent_->keys_[my_idx]);
	parent_->keys_[my_idx] = std::move(right->keys_[0]);
	children_[num_keys_ + 1] = std::move(right->children_[0]);
	if (children_[num_keys_ + 1])
		children_[num_keys_ + 1]->parent_ = this;

	// fix formatting of right
	for (int i = 1; i < right->num_keys_; i++) {
		right->keys_[i-1] = std::move(right->keys_[i]);
		right->children_[i-1] = std::move(right->children_[i]);
	}
	right->children_[right->num_keys_-1] = std::move(right->children_[right->num_keys_]);

	--right->num_keys_;
	++num_keys_;
	return this;
}

template<class K, class C, class A>
typename TwoFourTree<K,C,A>::Node * TwoFourTree<K,C,A>::Node::transferFromLeft() {

	int my_idx = getMyChildIdx();
	std::unique_ptr < Node > &left = parent_->children_[my_idx - 1];

	// make space in *this for the incoming key from parent
	for (int i = num_keys_; i > 0; --i){
		keys_[i] = std::move(keys_[i-1]);
		children_[i+1] = std::move(children_[i]);
	}
	children_[1] = std::move(children_[0]);

	// transfer from parent and left
	keys_[0] = std::move(parent_->keys_[my_idx-1]);
	parent_->keys_[my_idx - 1] = std::move(left->keys_[left->num_keys_ - 1]);
	children_[0] = std::move(left->children_[left->num_keys_]);
	if (children_[0])
		children_[0]->parent_ = this;

	--left->num_keys_;
	++num_keys_;
	return this;
}

template<class K, class C, class A>
typename TwoFourTree<K,C,A>::Node * TwoFourTree<K,C,A>::Node::fusion() {
	if ( parent_->num_keys_ < 2) {
		std::cout << "fusion failed. cannot steal value from parent because it only has one.\n";
		return this;
	}

	int my_idx = getMyChildIdx();
	if (my_idx > 0) { // prefer to fuse with left
		auto &left = parent_->children_[my_idx - 1];
		left->keys_[1] = std::move(parent_->keys_[my_idx-1]);
		left->keys_[2] = std::move(keys_[0]);

		if (children_[0])
			children_[0]->parent_ = left.get();
		if (children_[1])
			children_[1]->parent_ = left.get();
		left->children_[2] = std::move(children_[0]);
		left->children_[3] = std::move(children_[1]);

		std::unique_ptr<Node> self = std::move(parent_->children_[my_idx]);
		for (auto i = my_idx; i < parent_->num_keys_; i++) {
			parent_->children_[i] = std::move(parent_->children_[i + 1]);
			parent_->keys_[i-1] = std::move(parent_->keys_[i]);
		}

		--parent_->num_keys_;
		left->num_keys_ = 3;

		// Warning: *this will be destroyed
		return left.get();
	} else { // forced to fuse with right
		auto right = std::move(parent_->children_[my_idx + 1]);

		keys_[1] = std::move(parent_->keys_[my_idx]);
		keys_[2] = std::move(right->keys_[0]);
		children_[2] = std::move(right->children_[0]);
		children_[3] = std::move(right->children_[1]);
		if(children_[2])
			children_[2]->parent_ = this;
		if(children_[3])
			children_[3]->parent_ = this;

		// fix parent
		for (int i = my_idx; i < parent_->num_keys_ - 1; ++i) {
			parent_->keys_[i] = std::move(parent_->keys_[i + 1]);
			parent_->children_[i + 1] = std::move(parent_->children_[i + 2]);
		}
		--parent_->num_keys_;

		num_keys_ = 3;
		return this;
	}
}


template<class K, class C, class A>
typename TwoFourTree<K,C,A>::Node * TwoFourTree<K,C,A>::Node::shrink() {
	assert(parent_->parent_ == nullptr); // assert parent is root

	std::unique_ptr<Node> right, left; // one of these contains *this
	left = std::move(parent_->children_[0]);
	right = std::move(parent_->children_[1]);

	parent_->keys_[1] = std::move(parent_->keys_[0]);
	parent_->keys_[0] = std::move(left->keys_[0]);
	parent_->keys_[2] = std::move(right->keys_[0]);

	parent_->children_[0] = std::move(left->children_[0]);
	parent_->children_[1] = std::move(left->children_[1]);
	parent_->children_[2] = std::move(right->children_[0]);
	parent_->children_[3] = std::move(right->children_[1]);

	if (parent_->children_[0])
		parent_->children_[0]->parent_ = parent_;

	if (parent_->children_[1])
			parent_->children_[1]->parent_ = parent_;

	if (parent_->children_[2])
			parent_->children_[2]->parent_ = parent_;

	if (parent_->children_[3])
			parent_->children_[3]->parent_ = parent_;

	parent_->num_keys_ = 3;
	// Warning: *this will be destroyed
	return parent_;
}

template <class K, class C, class A>
int TwoFourTree<K,C,A>::Node::getMyChildIdx() const{
	assert(parent_ != nullptr);
	for (int i=0; i <= parent_->num_keys_; ++i) {
		if (parent_->children_[i].get() == this)
			return i;
	}
	std::cerr << "Error: I am not my parents child!\n";
	return -1;
}

template <class K, class C, class A>
typename TwoFourTree<K,C,A>::Node* TwoFourTree<K,C,A>::Node::leftSibling() {
	return parent_->children_[getMyChildIdx() - 1].get();
}

template <class K, class C, class A>
typename TwoFourTree<K,C,A>::Node* TwoFourTree<K,C,A>::Node::rightSibling() {
	return parent_->children_[getMyChildIdx() + 1].get();
}

template<class Key, class C, class A>
std::pair<const typename TwoFourTree<Key,C,A>::Node*, int>  TwoFourTree<Key,C,A>::Node::getSuccessor(int to_index) const {
	assert(0 <= to_index && to_index < num_keys_);
	const Node* node_ = this;
	int idx_ = to_index;

	if (node_->children_[idx_ + 1]) { // if there is a child after this key
		// go all the way to the leaf
		const Node *node = node_->children_[idx_ + 1].get();
		while (!node->isLeaf()) {
			node = node->children_[0].get();
		}
		node_ = node;
		idx_ = 0;
		return std::make_pair(node_, idx_);

	} else if (idx_ < (node_->num_keys_ - 1)) { // no child -> go right if there is something to the right
		assert(node_->isLeaf());
		++idx_;
		return std::make_pair(node_, idx_);

	} else { // no child, nothing to the right -> go up
		assert (idx_ == node_->num_keys_ -1);

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
				return std::make_pair(node_, idx_);
			}

			// If this node is not the biggest child then next value is parent at idx [child idx]
			for (int i = 0; i < parent->num_keys_; i++) {
				if (parent->children_[i].get() == node_) {
					node_ = parent;
					idx_ = i;
					return std::make_pair(node_, idx_);
				}
			}

			// node_ is the biggest child of parent_
			// repeat same process on upper level
			assert(parent->children_[parent->num_keys_].get() == node_);
			node_ = parent;
			continue;
		}
	}
}

template<class Key, class C, class A>
std::pair<const typename TwoFourTree<Key,C,A>::Node*, int> TwoFourTree<Key,C,A>::Node::getPredecessor(int to_index) const {
	assert(0 <= to_index && to_index < num_keys_);
	const Node * node_ = this;
	int idx_ = to_index;

	if (node_->children_[idx_]) { // if there is a child before this key
		// go all the way to the leaf
		const Node *node = node_->children_[idx_].get();
		while (!node->isLeaf()) {
			node = node->children_[node->num_keys_].get();
		}
		node_ = node;
		idx_ = node_->num_keys_ - 1;
		return std::make_pair(node_, idx_);

	} else if (idx_ > 0) { // no child -> go left if there is something to the left
		assert(node_->isLeaf());
		--idx_;
		return std::make_pair(node_, idx_);

	} else { // no child, nothing to the left -> go up
		assert(idx_ == 0);

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
				return std::make_pair(node_, idx_);
			}

			// If this node is not the smallest child then next value is parent at idx [child idx - 1]
			for (int i = 1; i <= parent->num_keys_; i++) {
				if (parent->children_[i].get() == node_) {
					node_ = parent;
					idx_ = i - 1;
					return std::make_pair(node_, idx_);
				}
			}

			// node_ is the smallest child of parent_
			// repeat same process on upper level
			assert(parent->children_[0].get() == node_);
			node_ = parent;
			continue;
		}
	}
}

template<class Key, class C, class A>
std::pair<typename TwoFourTree<Key,C,A>::Node*, int> TwoFourTree<Key,C,A>::Node::getSuccessor(int to_index){
	auto rc = static_cast<const Node&>(*this).getSuccessor(to_index);
	return std::make_pair(const_cast<Node*>(rc.first), rc.second);
}

template<class Key, class C, class A>
std::pair<typename TwoFourTree<Key,C,A>::Node*, int> TwoFourTree<Key,C,A>::Node::getPredecessor(int to_index){
	auto rc = static_cast<const Node&>(*this).getPredecessor(to_index);
	return std::make_pair(const_cast<Node*>(rc.first), rc.second);
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
	std::pair<Node*, int> iter { this, -1 };
	while (!iter.first->isLeaf()) {
		iter = iter.first->traverse_step(key);
		if (iter.second != -1)
			return iter; // found key
	}

	// if we reach here then iter.first is a leaf node
	// final check to see if we can find the key
	iter.second = iter.first->getKeyIndex(key);
	return iter;
}

template<class K, class C, class A>
bool TwoFourTree<K,C,A>::Node::isFull() const {
	return (num_keys_ == kMaxNumKeys);
}

template<class K, class C, class A>
bool TwoFourTree<K,C,A>::Node::isLeaf() const {
	return !children_[0];
}


template<class  K, class C, class A>
bool TwoFourTree<K,C,A>::Node::containsKey (const K& k) {
	for (int i=0; i < num_keys_; i++)
		if (keys_[i] == k)
			return true;
	return false;
}

template<class  K, class C, class A>
const typename TwoFourTree<K,C,A>::Node * TwoFourTree<K,C,A>::Node::getParent () const {
	return parent_;
}

template<class  K, class C, class A>
std::pair<const typename TwoFourTree<K,C,A>::Node *, int> TwoFourTree<K,C,A>::Node::findLargest () const {
	const Node * node = this;
	while (!node->isLeaf()) {
		node = node->children_[num_keys_].get(); // rightmost child
		assert(node != nullptr);
	}
	return std::make_pair(node, node->num_keys_ - 1);
}

} /* namespace tft */



#endif /* SRC_NODE_OPERATIONS_IPP_ */
