#include <memory>
#include <bitset>
#include <unordered_map>

static const size_t BITSET_SIZE = 5;

template <typename T>
class Node {
public:
    Node() {}

    Node(std::shared_ptr<Node<T>> parent) : parent_(parent) {}

    Node(std::shared_ptr<Node<T>> parent, const T &value)
        : parent_(parent), value_(value) {}

    Node(const Node<T> &other)
        : children_(other.children_), parent_(other.parent_),
          value_(other.value_) {}

    Node(const Node<T> &other, const T &value)
        : children_(other.children_), parent_(other.parent_), value_(value) {}

    ~Node() = default;

    std::shared_ptr<Node<T>> GetChild(unsigned long long name) {
        std::bitset<BITSET_SIZE> child_bits(name);
        return children_[child_bits];
    }

    std::shared_ptr<Node<T>> GetParent() {
        return parent_;
    }

    const T& GetValue() {
        return value_;
    }

    void ChangeParent(std::shared_ptr<Node> new_parent) {
        parent_ = new_parent;
    }

    void ChangeChild(unsigned long long name, std::shared_ptr<Node<T>> new_child) {
        std::bitset<BITSET_SIZE> child_bits(name);
        children_[child_bits] = new_child;
    }
private:
    std::unordered_map<std::bitset<BITSET_SIZE>, std::shared_ptr<Node<T>>> children_;
    std::weak_ptr<Node<T>> parent_;
    T value_;
};

template <typename T>
class ImmutableVectorImpl {
public:
    ImmutableVectorImpl() : root_(std::make_shared<Node<T>>(Node<T>())) {}

    ImmutableVectorImpl(std::shared_ptr<Node<T>> root) : root_(root) {}

    ~ImmutableVectorImpl() = default;

    ImmutableVectorImpl Set(size_t index, const T& value) {
        std::shared_ptr<Node<T>> new_root =
            std::make_shared<Node<T>>(Node<T>(*root_));
        std::shared_ptr<Node<T>> cur_node = new_root;
        bool first_iter = true;
        while (index != 0 || first_iter) {
            unsigned long long cur_bits = (index & ((2 << BITSET_SIZE) - 1));

            index >>= BITSET_SIZE;
            
            std::shared_ptr<Node<T>> new_node;
            if (index == 0) {
              new_node = std::make_shared<Node<T>>(
                    Node<T>(*cur_node->GetChild(cur_bits), value));
            } else {
              new_node = std::make_shared<Node<T>>(
                    Node<T>(*cur_node->GetChild(cur_bits)));
            }
            cur_node->ChangeChild(cur_bits, new_node);
            new_node->ChangeParent(cur_node);
            cur_node = new_node;
            first_iter = false;
        }
        return ImmutableVectorImpl(new_root);
    }

    ImmutableVectorImpl PushBack(const T& value, size_t index) {
        std::shared_ptr<Node<T>> new_root = std::make_shared<Node<T>>(Node<T>(*root_));
        std::shared_ptr<Node<T>> cur_node = new_root;
        
        bool first_iter = true;
        while (index != 0 || first_iter) {
            unsigned long long cur_bits = (index & (2<<BITSET_SIZE) - 1);
            index >>= BITSET_SIZE;
            std::shared_ptr<Node<T>> new_node;
            if (index == 0) {
                new_node = std::make_shared<Node<T>>(
                    Node<T>(cur_node, value)
                );
                cur_node->ChangeChild(cur_bits, new_node);
                break;
            }
            auto cur_child = cur_node->GetChild(cur_bits);
            if (!cur_child) {
                new_node = std::make_shared<Node<T>>(
                    Node<T>(cur_node)
                );
            } else {
                new_node = std::make_shared<Node<T>>(
                    Node<T>(*cur_node->GetChild(cur_bits))
                );
            }
            cur_node->ChangeChild(cur_bits, new_node);
            new_node->ChangeParent(cur_node);
            cur_node = new_node;
            first_iter = false;
        }
        return ImmutableVectorImpl<T>(new_root);
    }

    ImmutableVectorImpl PopBack(size_t index) {
        std::shared_ptr<Node<T>> new_root = std::make_shared<Node<T>>(Node<T>(*root_));
        std::shared_ptr<Node<T>> cur_node = new_root;
        bool first_iter = true;
        while (index != 0 || first_iter) {
            unsigned long long cur_bits = (index & (2<<BITSET_SIZE) - 1);
            index >>= BITSET_SIZE;
            if (index == 0) {
                cur_node->ChangeChild(cur_bits, nullptr);
                break;
            }
            std::shared_ptr<Node<T>> new_node = std::make_shared<Node<T>>(
                Node<T>(*cur_node->GetChild(cur_bits))
            );
            cur_node->ChangeChild(cur_bits, new_node);
            new_node->ChangeParent(cur_node);
            cur_node = new_node;
            first_iter = false;
        }
        return ImmutableVectorImpl(new_root);
    }

    const T& Get(size_t index) const {
        std::shared_ptr<Node<T>> cur_node = root_;
        bool first_iter = true;
        while (index != 0 || first_iter) {
            unsigned long long cur_bits = (index & ((2 << BITSET_SIZE) - 1));
            index >>= BITSET_SIZE;
            cur_node = cur_node->GetChild(cur_bits);
            first_iter = false;
        }
        return cur_node->GetValue();
    }
private:
    std::shared_ptr<Node<T>> root_;
};
