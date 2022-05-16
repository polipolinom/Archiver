#include "priority_queue.h"

#include <cassert>
#include <cstdint>
#include <vector>

#include "binary_trie.h"

void PriorityQueue::UpRelaxVertex(size_t current_ind) {
    assert(current_ind < vertexes_.size());
    size_t parent_ind = (current_ind - 1) / 2;
    while (current_ind > 0 && vertexes_[current_ind] < vertexes_[parent_ind]) {
        std::swap(vertexes_[parent_ind], vertexes_[current_ind]);
        current_ind = parent_ind;
        parent_ind = (current_ind - 1) / 2;
    }
}

void PriorityQueue::DownRelaxVertex(size_t current_ind) {
    assert(current_ind < vertexes_.size());
    size_t left_ind = current_ind * 2 + 1;
    size_t right_ind = current_ind * 2 + 2;
    while (left_ind < vertexes_.size()) {
        size_t min_child_ind = left_ind;
        if (right_ind < vertexes_.size() && vertexes_[right_ind] < vertexes_[left_ind]) {
            min_child_ind = right_ind;
        }
        if (vertexes_[current_ind] < vertexes_[min_child_ind]) {
            break;
        }
        std::swap(vertexes_[min_child_ind], vertexes_[current_ind]);
        current_ind = min_child_ind;
        left_ind = current_ind * 2 + 1;
        right_ind = current_ind * 2 + 2;
    }
}

void PriorityQueue::push(const BinaryTrie& value) {
    vertexes_.push_back(value);
    UpRelaxVertex(vertexes_.size() - 1);
}

size_t PriorityQueue::size() const {
    return vertexes_.size();
}

void PriorityQueue::pop() {
    assert(!vertexes_.empty());
    std::swap(vertexes_.back(), vertexes_.front());
    vertexes_.pop_back();
    if (!vertexes_.empty()) {
        DownRelaxVertex(0);
    }
}
BinaryTrie PriorityQueue::front() const {
    assert(!vertexes_.empty());
    return vertexes_[0];
}
