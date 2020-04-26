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
bool TwoFourTree<K,C,A>::Node::isFull() const {
	return (num_keys_ == kMaxNumKeys);
}

template<class K, class C, class A>
bool TwoFourTree<K,C,A>::Node::isLeaf() const {
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

template<class  K, class C, class A>
typename TwoFourTree<K,C,A>::Node * TwoFourTree<K,C,A>::Node::getParent () const {
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
