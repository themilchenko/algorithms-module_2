#include <cmath>
#include <iostream>
#include <vector>

template <typename T>
struct Node {
  Node<T> *left_;
  Node<T> *right_;
  T data_;
  size_t height_;
  int children_count_;

  Node()
      : left_(nullptr),
        right_(nullptr),
        data_(),
        height_(1),
        children_count_(0){};

  explicit Node(const T &data)
      : left_(nullptr),
        right_(nullptr),
        data_(data),
        height_(1),
        children_count_(1){};

  bool operator==(const Node &other) {
    return data_ == other.data_;
  };
};

template <typename T>
class AvlTree {
 public:
  AvlTree() : root_(nullptr), size_(0){};
  AvlTree(const T &data) : root_(data), size_(1){};

  AvlTree(const AvlTree &other) = default;
  AvlTree &operator=(const AvlTree &other) = default;

  void Add(Node<T> *node, const T &data) {
    root_ = add(node, data);
  };
  void Delete(Node<T> *node, const T &data) {
    root_ = deleteNode(node, data);
  };

  Node<T> *GetStatistics(size_t num);

  void Print(Node<T> *node) const;

  Node<T> *GetRoot() const {
    return root_;
  };

  ~AvlTree() {
    destruct(root_);
  };

 private:
  Node<T> *root_;
  size_t size_;

  Node<T> *find(Node<T> *node, size_t num);

  size_t getHeight(Node<T> *node) const;
  void fixHeight(Node<T> *node);

  Node<T> *add(Node<T> *node, const T &data);
  Node<T> *deleteNode(Node<T> *node, const T &data);

  Node<T> *smallRotateLeft(Node<T> *node);
  Node<T> *smallRotateRight(Node<T> *node);
  Node<T> *bigRotateLeft(Node<T> *node);
  Node<T> *bigRotateRight(Node<T> *node);

  void fixChildren(Node<T> *node);

  Node<T> *makeBalance(Node<T> *node);

  Node<T> *findMin(Node<T> *node);
  Node<T> *removeMin(Node<T> *node);
  std::pair<Node<T> *, Node<T> *> findAndRemoveMin(Node<T> *node);

  Node<T> *findMax(Node<T> *node);
  Node<T> *findPreviousMax(Node<T> *node, Node<T> *max_node);

  void destruct(Node<T> *node);
};

template <typename T>
Node<T> *AvlTree<T>::GetStatistics(size_t num) {
  Node<T> *current_node = root_;

  if (num >= size_) {
    return nullptr;
  }

  while (true) {
    size_t current_index =
        (current_node->left_) ? current_node->left_->children_count_ : 0;
    if (num == current_index) {
      return current_node;
    } else if (num > current_index) {
      current_node = current_node->right_;
      num -= current_index + 1;
    } else {
      current_node = current_node->left_;
    }
  }
}

template <typename T>
size_t AvlTree<T>::getHeight(Node<T> *node) const {
  return (node) ? node->height_ : 0;
}

template <typename T>
void AvlTree<T>::fixHeight(Node<T> *node) {
  node->height_ = ((getHeight(node->right_) > getHeight(node->left_))
                       ? getHeight(node->right_)
                       : getHeight(node->left_)) +
                  1;
}

template <typename T>
Node<T> *AvlTree<T>::smallRotateLeft(Node<T> *node) {
  Node<T> *down_node = node->right_;
  node->right_ = down_node->left_;
  down_node->left_ = node;

  fixChildren(down_node->left_);
  fixChildren(down_node);

  fixHeight(node);
  fixHeight(down_node);
  return down_node;
}

template <typename T>
Node<T> *AvlTree<T>::smallRotateRight(Node<T> *node) {
  Node<T> *down_node = node->left_;
  node->left_ = down_node->right_;
  down_node->right_ = node;

  fixChildren(down_node->right_);
  fixChildren(down_node);

  fixHeight(node);
  fixHeight(down_node);
  return down_node;
}

template <typename T>
Node<T> *AvlTree<T>::bigRotateRight(Node<T> *node) {
  node->left_ = smallRotateLeft(node->left_);
  node = smallRotateRight(node);
  return node;
}

template <typename T>
Node<T> *AvlTree<T>::bigRotateLeft(Node<T> *node) {
  node->right_ = smallRotateRight(node->right_);
  node = smallRotateLeft(node);
  return node;
}

template <typename T>
Node<T> *AvlTree<T>::makeBalance(Node<T> *node) {
  fixHeight(node);
  if ((getHeight(node->right_) - getHeight(node->left_) == 2) &&
      ((getHeight(node->right_->right_) == getHeight(node->right_->left_)) ||
       (getHeight(node->right_->right_) - getHeight(node->right_->left_) ==
        1))) {
    node = smallRotateLeft(node);
    return node;
  } else if ((getHeight(node->left_) - getHeight(node->right_) == 2) &&
             ((getHeight(node->left_->left_) ==
               getHeight(node->left_->right_)) ||
              (getHeight(node->left_->left_) - getHeight(node->left_->right_) ==
               1))) {
    node = smallRotateRight(node);
    return node;
  } else if ((getHeight(node->right_) - getHeight(node->left_) == 2) &&
             (getHeight(node->right_->left_) >
              getHeight(node->right_->right_))) {
    node = bigRotateLeft(node);
    return node;
  } else if ((getHeight(node->left_) - getHeight(node->right_) == 2) &&
             (getHeight(node->left_->right_) > getHeight(node->left_->left_))) {
    node = bigRotateRight(node);
    return node;
  }

  fixChildren(node);
  return node;
}

template <typename T>
void AvlTree<T>::fixChildren(Node<T> *node) {
  if (node->right_ && node->left_) {
    node->children_count_ =
        node->right_->children_count_ + node->left_->children_count_ + 1;
  } else if (node->right_) {
    node->children_count_ = node->right_->children_count_ + 1;
  } else if (node->left_) {
    node->children_count_ = node->left_->children_count_ + 1;
  } else {
    node->children_count_ = 1;
  }
}

template <typename T>
Node<T> *AvlTree<T>::add(Node<T> *node, const T &data) {
  if (!node) {
    Node<T> *element = new Node<T>(data);
    if (!size_) {
      root_ = element;
    }
    ++size_;
    return element;
  } else if (node->data_ > data) {
    node->left_ = add(node->left_, data);
  } else {
    node->right_ = add(node->right_, data);
  }

  return makeBalance(node);
}

template <typename T>
Node<T> *AvlTree<T>::findMax(Node<T> *node) {
  return node->right_ ? findMax(node->right_) : node;
}

template <typename T>
Node<T> *AvlTree<T>::findPreviousMax(Node<T> *node, Node<T> *max_node) {
  return (node->right_ == max_node || node == max_node)
             ? node
             : findPreviousMax(node->right_, max_node);
}

template <typename T>
Node<T> *AvlTree<T>::findMin(Node<T> *node) {
  return node->left_ ? findMin(node->left_) : node;
}

template <typename T>
Node<T> *AvlTree<T>::removeMin(Node<T> *node) {
  if (!node->left_) {
    return node->right_;
  }
  node->left_ = removeMin(node->left_);
  return makeBalance(node);
}

template <typename T>
std::pair<Node<T> *, Node<T> *> AvlTree<T>::findAndRemoveMin(Node<T> *node) {
  if (!node->left_) {
    return std::make_pair(node->right_, node);
  }
  auto res = findAndRemoveMin(node->left_);
  return std::make_pair(makeBalance(node), res.second);
}

template <typename T>
Node<T> *AvlTree<T>::deleteNode(Node<T> *node, const T &data) {
  if (node != nullptr && node->data_ < data) {
    node->right_ = deleteNode(node->right_, data);
  } else if (node != nullptr && node->data_ > data) {
    node->left_ = deleteNode(node->left_, data);
  } else {
    --size_;
    Node<T> *right_node = node->right_;
    Node<T> *left_node = node->left_;
    delete node;
    if (!right_node) {
      return left_node;
    }

    auto min_node = findAndRemoveMin(right_node);
    min_node.second->left_ = left_node;
    min_node.second->right_ = min_node.first;
    return makeBalance(min_node.second);
  }
  return makeBalance(node);
}

template <typename T>
void AvlTree<T>::destruct(Node<T> *node) {
  if (node) {
    destruct(node->left_);
    destruct(node->right_);
    delete node;
  }
}

template <typename T>
void AvlTree<T>::Print(Node<T> *node) const {
  if (node != nullptr) {
    Print(node->left_);
    std::cout << node->data_ << " child count: " << node->children_count_
              << std::endl;
    Print(node->right_);
  }
}

int main() {
  AvlTree<int> tree;
  int commands_num = 0;
  std::cin >> commands_num;
  for (int i = 0; i < commands_num; ++i) {
    int num;
    int k;
    std::cin >> num >> k;
    if (num > 0) {
      tree.Add(tree.GetRoot(), num);
    } else {
      tree.Delete(tree.GetRoot(), abs(num));
    }
    Node<int> *stat = tree.GetStatistics(k);
    if (stat) {
      std::cout << stat->data_ << ' ';
    }
  }

  return 0;
}
