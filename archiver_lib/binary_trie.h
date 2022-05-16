#ifndef CPP_PILOT_HSE_BINARY_TRIE_H
#define CPP_PILOT_HSE_BINARY_TRIE_H

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

class BinaryTrie {
public:
    struct Vertex {
        std::shared_ptr<Vertex> left = nullptr;
        std::shared_ptr<Vertex> right = nullptr;
        bool terminal = false;
        size_t value = -1;
        size_t weight = 0;

        Vertex(std::shared_ptr<Vertex> left_, std::shared_ptr<Vertex> right_);
        Vertex(size_t value_, size_t weight_);
        Vertex();

        ~Vertex() = default;
    };

    bool operator<(const BinaryTrie& other) const;

    BinaryTrie(size_t value, size_t weight);
    BinaryTrie(std::shared_ptr<Vertex> root);
    BinaryTrie(const BinaryTrie& first, const BinaryTrie& second);

    ~BinaryTrie() = default;

    std::shared_ptr<Vertex> GetRoot() const;
    size_t GetWeight() const;
    std::vector<std::pair<size_t, int16_t>> GetBinaryCodes() const;

private:
    std::shared_ptr<Vertex> root_ = nullptr;
    size_t weight_ = 0;

    BinaryTrie();

    void DFSWithCodes(std::shared_ptr<Vertex> current, int16_t prefix,
                      std::vector<std::pair<size_t, int16_t>>& codes) const;
};

#endif  // CPP_PILOT_HSE_BINARY_TRIE_H
