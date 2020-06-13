/*
 *  Filename:   debug.ipp
 *
 *  Created on:  Apr. 17, 2020
 *      Author:  orhan
 *
 *  Description:
 *		Contains debug operation implementations for both the TwoFourTree and the Node class
 */


#ifndef SRC_DEBUG_OPERATIONS_IPP_
#define SRC_DEBUG_OPERATIONS_IPP_

namespace tft {
/**
 * Verify all nodes have the correct child-parent relationship
 */
template <class K, class C, class A>
bool TwoFourTree<K,C,A>::validate() const {
	return root_->validateRelationships();
}

/**
 * Use a deque<parent, child> to do a breadth-first search over the entire tree to ensure
 * every child knows who its parent is
 */
template <class K, class C, class A>
bool TwoFourTree<K,C,A>::Node::validateRelationships() const {
	const Node * node = this;
	bool rc = true;
	// parent -> child
	std::deque<std::pair<const TwoFourTree::Node*, const TwoFourTree::Node*>> allNodes;
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

		int num_children = 0;
		for (int i = 0; i < kMaxNumChildren; i++) {
			if (node->children_[i]) {
				allNodes.push_back(std::make_pair(node,node->children_[i].get()));
				++num_children;
			}
		}
		if (!node->isLeaf() && num_children != node->num_keys_ + 1) {
			std::cout << "number of keys and children mismatch.\n";
			std::cout << "Node contents = [" << node->getString() << "], num_keys = " << node->num_keys_ << ", #children (" << num_children <<"): \n";
			for (int i = 0; i < num_children; i++) {
				std::cout << node->children_[i]->getString() << "   ";
			}
			std::cout << "\n";
			rc = false;
		}
	} // end loop
	return rc;
}


/**
 * Prints the tree level by level
 */
template<class K, class C, class A>
std::string TwoFourTree<K,C,A>::getString() const{
	return root_->getStringAll();
}

template<class K, class C, class A>
void TwoFourTree<K,C,A>::print() const{
	root_->printAll();
}

template<class K, class C, class A>
void TwoFourTree<K,C,A>::Node::printAll() const {
	std::cout << getStringAll();
}

template<class K, class C, class A>
std::string TwoFourTree<K,C,A>::Node::getStringAll() const {

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
	std::deque<const Node*> nodes_to_be_printed;

	nodes_to_be_printed.push_back(this);
	nodes_to_be_printed.push_back(&end_of_level);
	nodes_to_be_printed.push_back(&children_end);

	while (!nodes_to_be_printed.empty()) {
		const Node *node = nodes_to_be_printed.front();
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
			ss << "_:";
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

	ss << std::endl;
	return ss.str();
}

template <class K, class C, class A>
std::string TwoFourTree<K,C,A>::Node::getString() const {
	std::stringstream ss;
	for (int i=0; i < num_keys_-1; i++) {
		ss <<keys_[i] << ",";
	}
	ss << keys_[num_keys_-1] << ":";
	return ss.str();
}

// useful when debugging, not used as part of other functions
template <class K, class C, class A>
void TwoFourTree<K,C,A>::Node::print() const {
	std::cout << getString() << std::endl;
}

template <class K, class C, class A>
void TwoFourTree<K,C,A>::const_iterator::print() const {
	std::cout << getString() << std::endl;
}

template <class K, class C, class A>
std::string TwoFourTree<K,C,A>::const_iterator::getString() const {
	std::stringstream ss;
	ss << "it: ";
	if (node_ == nullptr) {
		ss << "nullptr, idx = " << idx_ ;

	} else if (idx_ == node_->num_keys_) {
		ss << "after end iterator (node:" << node_->getString() << "), idx = " << idx_ ;

	} else if (idx_ == -1) {
		ss << "before beginning iterator (node:" << node_->getString() << "), idx = " << idx_;

	} else {
		ss << "n:" << node_->getString() << " idx_:" << idx_;
	}
	return ss.str();
}
} /* namespace tft */

#endif /* SRC_DEBUG_OPERATIONS_IPP_ */
