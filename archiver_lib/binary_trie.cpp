#include "binary_trie.h"

#include <cstdint>
#include <memory>
#include <vector>
#include <utility>

BinaryTrie::Vertex::Vertex(size_t value_, size_t weight_) : value(value_), weight(weight_) {
    terminal = true;
    left = nullptr;
    right = nullptr;
}

BinaryTrie::Vertex::Vertex() {
    left = nullptr;
    right = nullptr;
    terminal = false;
    value = -1;
    weight = 0;
}

BinaryTrie::Vertex::Vertex(std::shared_ptr<Vertex> left_, std::shared_ptr<Vertex> right_)
    : left(left_), right(right_) {
    terminal = false;
    size_t weight_left = (left == nullptr ? 0 : left->weight);
    size_t weight_right = (right == nullptr ? 0 : right->weight);
    weight = weight_left + weight_right;
}
BinaryTrie::BinaryTrie() {
    root_ = nullptr;
    weight_ = 0;
}

BinaryTrie::BinaryTrie(size_t value, size_t weight) : weight_(weight) {
    root_ = std::make_shared<Vertex>(Vertex(value, weight));
}

BinaryTrie::BinaryTrie(std::shared_ptr<Vertex> root) : root_(root) {
    weight_ = (root_ == nullptr ? 0 : root_->weight);
}

std::shared_ptr<BinaryTrie::Vertex> BinaryTrie::GetRoot() const {
    return root_;
}

size_t BinaryTrie::GetWeight() const {
    return weight_;
}

BinaryTrie::BinaryTrie(const BinaryTrie& first, const BinaryTrie& second) {
    root_ = std::make_shared<Vertex>(BinaryTrie::Vertex(first.GetRoot(), second.GetRoot()));
    weight_ = (root_ == nullptr ? 0 : root_->weight);
}

bool BinaryTrie::operator<(const BinaryTrie& other) const {
    return weight_ < other.GetWeight();
}

void BinaryTrie::DFSWithCodes(std::shared_ptr<Vertex> current, int16_t prefix,
                              std::vector<std::pair<size_t,int16_t>>& codes) const {
    if (current == nullptr) {
        return;
    }
    if (current->terminal == true) {
        codes.push_back({current->value, prefix});
    }
    if (current->left != nullptr) {
        DFSWithCodes(current->left, (prefix << 1), codes);
    }
    if (current->right != nullptr) {
        DFSWithCodes(current->right, ((prefix << 1) ^ 1), codes);
    }
}

std::vector<std::pair<size_t, int16_t>> BinaryTrie::GetBinaryCodes() const {
    std::vector<std::pair<size_t, int16_t>> codes;
    DFSWithCodes(root_, 1, codes);
    return codes;
}
