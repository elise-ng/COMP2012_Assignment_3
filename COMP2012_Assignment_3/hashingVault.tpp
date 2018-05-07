//submit this file
//you do NOT need to include any header in this file
//just write your HashingVault implementation here right away

template <typename KeyType, typename ValueType>
HashingVault<KeyType, ValueType>::HashingVault(int size, Container<KeyType, ValueType>** table, int (*fun)(KeyType)) : table(table), size(size), fun(fun) {
}

template <typename KeyType, typename ValueType>
HashingVault<KeyType, ValueType>::~HashingVault() {
    for (int i = 0; i < this->size; i += 1) {
        delete this->table[i];
        this->table[i] = nullptr;
    }
    delete this->table;
    this->table = nullptr;
}

template <typename KeyType, typename ValueType>
bool HashingVault<KeyType, ValueType>::add(KeyType key, ValueType value) {
    return this->table[this->fun(key)]->add(key, value);
}

template <typename KeyType, typename ValueType>
ValueType HashingVault<KeyType, ValueType>::get(KeyType key) const {
    return this->table[this->fun(key)]->get(key);
}

template <typename KeyType, typename ValueType>
bool HashingVault<KeyType, ValueType>::remove(KeyType key) {
    return this->table[this->fun(key)]->remove(key);
}

template <typename KeyType, typename ValueType>
void HashingVault<KeyType, ValueType>::rehash(int size, Container<KeyType, ValueType>** table, int (*fun)(KeyType)) {
    int oldSize = this->size;
    Container<KeyType, ValueType>** oldTable = this->table;
    this->size = size;
    this->table = table;
    this->fun = fun;
    for (int i = 0; i < oldSize; i += 1) {
        for (int j = 0; j < oldTable[i]->count(); j += 0) {
            Pair<KeyType, ValueType>* pair = *(oldTable[i])[j];
            this->add(pair->key, pair->value);
            oldTable[i]->remove(pair->key);
            pair = nullptr;
        }
        delete oldTable[i];
        oldTable[i] = nullptr;
    }
    delete oldTable;
    oldTable = nullptr;
}
