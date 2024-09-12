#include <tuple>
#include <vector>
#include <algorithm>
#include <cassert>
#include <stdexcept>
using namespace std;

/**
 * An abstract template base of the KDTree class
 */
template<typename...>
class KDTree;

/**
 * A partial template specialization of the KDTree class
 * The time complexity of functions are based on n and k
 * n is the size of the KDTree
 * k is the number of dimensions
 * @typedef Key         key type
 * @typedef Value       value type
 * @typedef Data        key-value pair
 * @static  KeySize     k (number of dimensions)
 */
template<typename ValueType, typename... KeyTypes>
class KDTree<std::tuple<KeyTypes...>, ValueType> {
public:
    typedef std::tuple<KeyTypes...> Key;
    typedef ValueType Value;
    typedef std::pair<const Key, Value> Data;
    static inline constexpr size_t KeySize = std::tuple_size<Key>::value;
    static_assert(KeySize > 0, "Can not construct KDTree with zero dimension");
protected:
    struct Node {
        Data data;
        Node *parent;
        Node *left = nullptr;
        Node *right = nullptr;

        Node(const Key &key, const Value &value, Node *parent) : data(key, value), parent(parent) {}

        const Key &key() { return data.first; }

        Value &value() { return data.second; }
    };

public:
    /**
     * A bi-directional iterator for the KDTree
     * ! ONLY NEED TO MODIFY increment and decrement !
     */
    class Iterator {
    private:
        KDTree *tree;
        Node *node;

        Iterator(KDTree *tree, Node *node) : tree(tree), node(node) {}

        /**
         * Increment the iterator
         * Time complexity: O(log n)
         */
        void increment() {
            if (this->node->right) {
                this->node = this->node->right;
                while (this->node->left) this->node = this->node->left;
                return;
            }
            else {
                while (this->node->parent) {
                    if (this->node != this->node->parent->left) {
                        this->node = this->node->parent;
                        continue;
                    }
                    this->node = this->node->parent;
                    return;
                }
                this->node = nullptr;
                return;
            }
        }

        /**
         * Decrement the iterator
         * Time complexity: O(log n)
         */
        void decrement() {
            if (this->node->left) {
                this->node = this->node->left;
                while (this->node->right) this->node = this->node->right;
                return;
            }
            else {
                while (this->node->parent) {
                    if (this->node != this->node->parent->right) {
                        this->node = this->node->parent;
                        continue;
                    }
                    this->node = this->node->parent;
                    return;
                }
                this->node = nullptr;
                return;
            }
        }

    public:
        friend class KDTree;

        Iterator() = delete;

        Iterator(const Iterator &) = default;

        Iterator &operator=(const Iterator &) = default;

        Iterator &operator++() {
            increment();
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            increment();
            return temp;
        }

        Iterator &operator--() {
            decrement();
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            decrement();
            return temp;
        }

        bool operator==(const Iterator &that) const { return node == that.node; }

        bool operator!=(const Iterator &that) const { return node != that.node; }

        Data *operator->() { return &(node->data); }

        Data &operator*() { return node->data; }
    };

protected:                      // DO NOT USE private HERE!
    Node *root = nullptr;       // root of the tree
    size_t treeSize = 0;        // size of the tree

    /**
     * Find the node with key
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param node
     * @return the node with key, or nullptr if not found
     */
    template<size_t DIM>
    Node *find(const Key &key, Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node) return nullptr;
        if (key == node->key()) return node;
        if (get<DIM>(key) < get<DIM>(node->key())) return find<DIM_NEXT>(key, node->left);
        else return find<DIM_NEXT>(key, node->right);
    }

    /**
     * Insert the key-value pair, if the key already exists, replace the value only
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param value
     * @param node
     * @param parent
     * @return whether insertion took place (return false if the key already exists)
     */
    template<size_t DIM>
    bool insert(const Key &key, const Value &value, Node *&node, Node *parent) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node) {
            node = new Node(key, value, parent);
            this->treeSize++;
            return true;
        }
        else if (key == node->key()) {
            node->data.second = value;
            return false;
        }
        else if (get<DIM>(key) < get<DIM>(node->key())) return insert<DIM_NEXT>(key, value, node->left, node);
        else return insert<DIM_NEXT>(key, value, node->right, node);
    }

    /**
     * Compare two keys on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return relationship of two keys on a dimension with the compare function
     */
    template<size_t DIM, typename Compare>
    static bool compareKey(const Key &a, const Key &b, Compare compare = Compare()) {
        if (std::get<DIM>(a) != std::get<DIM>(b)) return compare(std::get<DIM>(a), std::get<DIM>(b));
        return compare(a, b);
    }

    /**
     * Compare two nodes on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return the minimum / maximum of two nodes
     */
    template<size_t DIM, typename Compare>
    static Node *compareNode(Node *a, Node *b, Compare compare = Compare()) {
        if (!a) return b;
        if (!b) return a;
        return compareKey<DIM, Compare>(a->key(), b->key(), compare) ? a : b;
    }

    /**
     * Find the minimum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the minimum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMin(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node) return nullptr;
        Node *min = findMin<DIM_CMP, DIM_NEXT>(node->left);
        if (DIM != DIM_CMP) {
            Node *right_min = findMin<DIM_CMP, DIM_NEXT>(node->right);
            min = compareNode<DIM_CMP, less<>>(min, right_min);
        }
        min = compareNode<DIM_CMP, less<>>(min, node);
        return min;
    }

    /**
     * Find the maximum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the maximum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMax(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node) return nullptr;
        Node *max = findMax<DIM_CMP, DIM_NEXT>(node->right);
        if (DIM != DIM_CMP) {
            Node *left_max = findMax<DIM_CMP, DIM_NEXT>(node->left);
            max = compareNode<DIM_CMP, greater<>>(left_max, max);
        }
        max = compareNode<DIM_CMP, greater<>>(node, max);
        return max;
    }

    template<size_t DIM>
    Node *findMinDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) dim %= KeySize;
        if (dim == DIM) return findMin<DIM, 0>(root);
        return findMinDynamic<DIM_NEXT>(dim);
    }

    template<size_t DIM>
    Node *findMaxDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) dim %= KeySize;
        if (dim == DIM) return findMax<DIM, 0>(root);
        return findMaxDynamic<DIM_NEXT>(dim);
    }

    /**
     * Erase a node with key (check the pseudocode in project description)
     * Time Complexity: max{O(k log n), O(findMin)}
     * @tparam DIM current dimension of node
     * @param node
     * @param key
     * @return nullptr if node is erased, else the (probably) replaced node
     */
    template<size_t DIM>
    Node *erase(Node *node, const Key &key) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!node) return nullptr;
        if (key == node->key()) {
            if (!node->left && !node->right) {
                delete node;
                this->treeSize--;
                if (this->treeSize == 0) this->root = nullptr;
                return nullptr;
            }
            else if (node->right) {
                Node *right_min = findMin<DIM, DIM_NEXT>(node->right);
                Key& node_key = const_cast<Key&>(node->key());
                node_key = right_min->key();
                node->data.second = right_min->value();
                node->right = erase<DIM_NEXT>(node->right, right_min->key());
            }
            else {
                Node *left_max = findMax<DIM, DIM_NEXT>(node->left);
                Key& node_key = const_cast<Key&>(node->key());
                node_key = left_max->key();
                node->data.second = left_max->value();
                node->left = erase<DIM_NEXT>(node->left, left_max->key());
            }
        }
        else {
            if (get<DIM>(key) < get<DIM>(node->key())) node->left = erase<DIM_NEXT>(node->left, key);
            else node->right = erase<DIM_NEXT>(node->right, key);
            return node;
        }
        return node;
    }

    template<size_t DIM>
    Node *eraseDynamic(Node *node, size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) dim %= KeySize;
        if (dim == DIM) return erase<DIM>(node, node->key());
        return eraseDynamic<DIM_NEXT>(node, dim);
    }

    template<size_t depth>
    Node *constructKDTree(std::vector<std::pair<Key, Value>> &data, Node *parent) {
        if (data.empty()) return nullptr;
        auto middle = data.begin() + data.size() / 2;
        nth_element(data.begin(), middle, data.begin() + static_cast<int>(data.size()), [](pair<Key, Value> &a, pair<Key, Value> &b) { return compareKey<depth, less<>>(a.first, b.first); });
        for (auto it = data.begin(); it != data.begin() + data.size() / 2; it++) if (get<depth>((*it).first) == get<depth>((*middle).first)) swap(it, --middle);
        Node *node = new Node((*middle).first, (*middle).second, parent);
        vector<pair<Key, Value>> leftData(data.begin(), middle), rightData(middle + 1, data.end());
        node->left = constructKDTree<(depth + 1) % KeySize>(leftData, node);
        node->right = constructKDTree<(depth + 1) % KeySize>(rightData, node);
        treeSize++;
        return node;
    }

    Node *copyNode(Node* dat, Node* parent) {
        if (!dat) return nullptr;
        else {
            Node *copy = new Node{dat->key(), dat->value(), parent};
            copy->right = copyNode(dat->right, copy);
            copy->left = copyNode(dat->left, copy);
            return copy;
        }
    }

    void deleting(Node* data) {
        if (!data) return;
        else {
            deleting(data->left);
            deleting(data->right);
            delete data;
            this->treeSize--;
        }
    }

public:
    KDTree() = default;

    /**
     * Time complexity: O(kn log n)
     * @param v we pass by value here because v need to be modified
     */
    explicit KDTree(std::vector<std::pair<Key, Value>> v) {
        this->treeSize = 0;
        stable_sort(v.begin(), v.end(), [](const pair<Key, Value> &a, const pair<Key, Value> &b) { return compareKey<0, less<>>(a.first, b.first);});
        v.erase(v.begin(), unique(v.rbegin(), v.rend(), [](const pair<Key, Value> &a, const pair<Key, Value> &b) {return a.first == b.first;}).base());
        this->root = constructKDTree<0>(v, nullptr);
    }

    /**
     * Time complexity: O(n)
     */
    KDTree(const KDTree &that) {
        deleting(this->root);
        if (that.root) this->root = copyNode(that.root, nullptr);
        else this->root = nullptr;
        this->treeSize = that.treeSize;
    }

    /**
     * Time complexity: O(n)
     */
    KDTree &operator=(const KDTree &that) {
        deleting(this->root);
        if (that.root) this->root = copyNode(that.root, nullptr);
        else this->root = nullptr;
        this->treeSize = that.treeSize;
        return *this;
    }

    /**
     * Time complexity: O(n)
     */
    ~KDTree() { deleting(this->root); }

    Iterator begin() {
        if (!root) return end();
        auto node = root;
        while (node->left) node = node->left;
        return Iterator(this, node);
    }

    Iterator end() { return Iterator(this, nullptr); }

    Iterator find(const Key &key) { return Iterator(this, find<0>(key, root)); }

    void insert(const Key &key, const Value &value) { insert<0>(key, value, root, nullptr); }

    template<size_t DIM>
    Iterator findMin() { return Iterator(this, findMin<DIM, 0>(root)); }

    Iterator findMin(size_t dim) { return Iterator(this, findMinDynamic<0>(dim)); }

    template<size_t DIM>
    Iterator findMax() { return Iterator(this, findMax<DIM, 0>(root)); }

    Iterator findMax(size_t dim) { return Iterator(this, findMaxDynamic<0>(dim)); }

    bool erase(const Key &key) {
        auto prevSize = treeSize;
        erase<0>(root, key);
        return prevSize > treeSize;
    }

    Iterator erase(Iterator it) {
        if (it == end()) return it;
        auto node = it.node;
        if (!it.node->left && !it.node->right) it.node = it.node->parent;
        size_t depth = 0;
        auto temp = node->parent;
        while (temp) {
            temp = temp->parent;
            ++depth;
        }
        eraseDynamic<0>(node, depth % KeySize);
        return it;
    }

    size_t size() const { return treeSize; }
};