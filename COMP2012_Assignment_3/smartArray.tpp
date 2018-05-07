//submit this file
//you do NOT need to include any header in this file
//just write your SmartArray implementation here right away

template <typename KeyType, typename ValueType>
SmartArray<KeyType, ValueType>::SmartArray() : data(nullptr), size(0) {
}

template <typename KeyType, typename ValueType>
SmartArray<KeyType, ValueType>::~SmartArray() {
    for (int i = 0; i < this->size; i += 1) {
        delete this->data[i];
        this->data[i] = nullptr;
    }
    delete [] this->data;
    this->data = nullptr;
}

template <typename KeyType, typename ValueType>
bool SmartArray<KeyType, ValueType>::add(KeyType key, ValueType value) {
    if (this->size == 0) {
        // first item, init array
        this->data = new Pair<KeyType, ValueType>*[1];
        this->data[0] = new Pair<KeyType, ValueType>(key, value);
        this->size = 1;
        return true;
    }
    if (this->has(key)) {
        // key exist already
        return false;
    }
    // expand array, insert new data
    Pair<KeyType, ValueType>** oldData = this->data;
    this->data = new Pair<KeyType, ValueType>*[this->size + 1];
    // prepare new data for insertion
    Pair<KeyType, ValueType>* newData = new Pair<KeyType, ValueType>(key, value);
    bool inserted = false;
    int migrateCount = 0;
    // migrate data, insert new data in order
    for (int i = 0; i < this->size; i += 1) {
        if (!inserted && oldData[i]->key > key) {
            // insert before first larger item
            this->data[migrateCount] = newData;
            migrateCount += 1;
            inserted = true;
        }
        // migrate old data
        this->data[migrateCount] = oldData[i];
        migrateCount += 1;
    }
    if (!inserted) {
        this->data[migrateCount] = newData;
    }
    delete [] oldData;
    oldData = nullptr;
    this->size += 1;
    return true;
}

template <typename KeyType, typename ValueType>
bool SmartArray<KeyType, ValueType>::remove(KeyType key) {
    if (!this->has(key)) {
        // key does not exist
        return false;
    }
    // shrink array
    Pair<KeyType, ValueType>** oldData = this->data;
    this->data = new Pair<KeyType, ValueType>*[this->size - 1];
    int migrateCount = 0;
    for (int i = 0; i < this->size; i += 1) {
        if (oldData[i]->key == key) {
            // remove target data
            delete oldData[i];
            oldData[i] = nullptr;
            continue;
        }
        // migrate remaining data
        this->data[migrateCount] = oldData[i];
        migrateCount += 1;
    }
    delete [] oldData;
    oldData = nullptr;
    this->size -= 1;
    return true;
}

template <typename KeyType, typename ValueType>
ValueType SmartArray<KeyType, ValueType>::get(KeyType key) const {
    for (int i = 0; i < this->size; i += 1) {
        if (this->data[i]->key == key) {
            // key found
            return this->data[i]->value;
        }
    }
    // key not found
    if (typeid(ValueType) == typeid(int)) {
        return 0;
    } else {
        return ValueType();
    }
}

template <typename KeyType, typename ValueType>
int SmartArray<KeyType, ValueType>::count() const {
    return this->size;
}

template <typename KeyType, typename ValueType>
const Pair<KeyType, ValueType>* SmartArray<KeyType, ValueType>::operator[] (int n) const {
    if (n < 0 || n >= this->size) {
        // n out of range
        return nullptr;
    }
    return this->data[n]; // array already in order
}

template <typename KeyType, typename ValueType>
void SmartArray<KeyType, ValueType>::print(ostream& os) const {
    for (int i = 0; i < this->size; i += 1) {
        // array already in order
        os << '(' << this->data[i]->key << ',' << this->data[i]->value << ')';
    }
}

template <typename KeyType, typename ValueType>
bool SmartArray<KeyType, ValueType>::has(KeyType key) const {
    return this->get(key) != (typeid(ValueType) == typeid(int) ? 0 : ValueType());
}
