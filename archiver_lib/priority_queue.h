#ifndef CPP_PILOT_HSE_PRIORITY_QUEUE_H
#define CPP_PILOT_HSE_PRIORITY_QUEUE_H

#include <cstdint>
#include <vector>

#include "binary_trie.h"

class PriorityQueue {
public:
    void push(const BinaryTrie& value);
    size_t size() const;
    void pop();
    BinaryTrie front() const;

private:
    std::vector<BinaryTrie> vertexes_;

    void UpRelaxVertex(size_t current_ind);
    void DownRelaxVertex(size_t current_ind);
};

#endif  // CPP_PILOT_HSE_PRIORITY_QUEUE_H
