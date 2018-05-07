//submit this file
//you do NOT need to include any header in this file
//just write your BST implementation here right away

template <typename KeyType, typename ValueType>
BST<KeyType, ValueType>::BST(const BST& another) : root(nullptr), size(0) {
    if (another.root == nullptr) {
        return;
    }
    this->root = new BSTNode<KeyType, ValueType>(another.root->data.key, another.root->data.value);
    this->root->left = BST<KeyType, ValueType>(another.root->left);
    this->root->right = BST<KeyType, ValueType>(another.root->right);
}

template <typename KeyType, typename ValueType>
bool BST<KeyType, ValueType>::isEmpty() const {
    return this->root == nullptr;
}

template <typename KeyType, typename ValueType>
const BST<KeyType, ValueType>* BST<KeyType, ValueType>::findMin() const {
    if (this->leftSubtree().isEmpty()) {
        // minimum found (leftmost item)
        return this;
    }
    if (this->root == nullptr) {
        // tree empty
        return nullptr;
    }
    // recurse on left subtree
    return this->leftSubtree().findMin();
}

template <typename KeyType, typename ValueType>
const BST<KeyType, ValueType>& BST<KeyType, ValueType>::rightSubtree() const {
    return this->root->right;
}

template <typename KeyType, typename ValueType>
const BST<KeyType, ValueType>& BST<KeyType, ValueType>::leftSubtree() const {
    return this->root->left;
}

template <typename KeyType, typename ValueType>
bool BST<KeyType, ValueType>::add(KeyType key, ValueType value) {
    if (this->isEmpty()) {
        // tree empty, add as root
        this->root = new BSTNode<KeyType, ValueType>(key, value);
        this->size = 1;
        return true;
    }
    if (key == this->root->data.key) {
        // key already exist
        return false;
    } else if (key < this->root->data.key) {
        // recurse on left tree
        if (this->root->left.add(key, value)) {
            this->size += 1;
            return true;
        } else {
            return false;
        }
    } else {
        // recurse on right tree
        if (this->root->right.add(key, value)) {
            this->size += 1;
            return true;
        } else {
            return false;
        }
    }
}

template <typename KeyType, typename ValueType>
bool BST<KeyType, ValueType>::remove(KeyType key) {
    if (this->isEmpty()) {
        // tree empty, key not found
        return false;
    }
    if (key == this->root->data.key) {
        // remove this node
        if (this->leftSubtree().isEmpty() && this->rightSubtree().isEmpty()) {
            // node is leaf, remove directly
            delete this->root;
            this->root = nullptr;
            this->size = 0;
            return true;
        } else if (!this->leftSubtree().isEmpty() && !this->rightSubtree().isEmpty()) {
            // node has 2 children, replace with minimum in right subtree
            const BST<KeyType, ValueType>* replacement = this->rightSubtree().findMin();
            this->root->data.key = replacement->root->data.key;
            this->root->data.value = replacement->root->data.value;
            this->root->right.remove(replacement->root->data.key);
            return true;
        } else {
            // node has 1 child, replace with only child
            BSTNode<KeyType, ValueType>* temp = this->root;
            this->root = this->rightSubtree().isEmpty() ? this->root->left.root : this->root->right.root;
            this->size = 1;
            temp->left.root = nullptr;
            temp->right.root = nullptr;
            delete temp;
            return true;
        }
    } else if (key < this->root->data.key) {
        // recurse on left subtree
        if (this->root->left.remove(key)) {
            this->size -= 1;
            return true;
        } else {
            return false;
        }
    } else {
        // recurse on right subtree
        if (this->root->right.remove(key)) {
            this->size -= 1;
            return true;
        } else {
            return false;
        }
    }
}

template <typename KeyType, typename ValueType>
ValueType BST<KeyType, ValueType>::get(KeyType key) const {
    if (this->isEmpty()) {
        // tree empty, not found
        if (typeid(ValueType) == typeid(int)) {
            return 0;
        } else {
            return ValueType();
        }
    }
    if (key == this->root->data.key) {
        // found
        return this->root->data.value;
    } else if (key < this->root->data.key) {
        // recurse on left subtree
        return this->leftSubtree().get(key);
    } else {
        // recurse on right subtree
        return this->rightSubtree().get(key);
    }
}

template <typename KeyType, typename ValueType>
int BST<KeyType, ValueType>::count() const {
    if (this->isEmpty()) {
        // base case
        return 0;
    }
    return this->leftSubtree().count() + this->rightSubtree().count() + 1;
}

template <typename KeyType, typename ValueType>
int BST<KeyType, ValueType>::height() const {
    if (this->root == nullptr || (this->leftSubtree().isEmpty() && this->rightSubtree().isEmpty())) {
        // base case
        return 0;
    }
    return max(this->leftSubtree().height(), this->rightSubtree().height()) + 1;
}

template <typename KeyType, typename ValueType>
const Pair<KeyType, ValueType>* BST<KeyType, ValueType>::operator[] (int n) const {
    if (n < 0 || n >= this->count()) {
        // n out of range
        return nullptr;
    }
    if (n < this->leftSubtree().count()) {
        // n inside left subtree
        return this->leftSubtree()[n];
    } else if (n == this->leftSubtree().count()) {
        // n is current node
        return &this->root->data;
    } else {
        // n inside right subtree
        return this->rightSubtree()[n - this->leftSubtree().count() - 1];
    }
}

template <typename KeyType, typename ValueType>
void BST<KeyType, ValueType>::print(std::ostream &os) const {
    for (int i = 0; i < this->count(); i++) {
        const Pair<KeyType, ValueType>* data = (*this)[i];
        os << '(' << data->key << ',' << data->value << ')';
    }
}
