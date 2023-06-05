#pragma once

#include <algorithm>
#include <vector>
#include <utility>

#include "absl/container/flat_hash_map.h"

template <class Key, class Value, class Compare, size_t children = 16>
class HashHeap {
public:
    size_t contains(const Key& key);
    bool insert(const Key& key, const Value& value);
    Value& at(const Key& key);
    void erase(const Key& key);
    std::pair<const Key&, Value&> peek();
    void pop();
    size_t size();
    bool empty();
    void clear();

private:
    absl::flat_hash_map<Key, std::pair<size_t, Value>> m_map;
    std::vector<Key> m_heap;

    void internalFixUp(size_t i);
    void internalFixDown(size_t i);
    void internalPopBack();
    void internalSwap(size_t i, size_t j);
};

template <class Key, class Value, class Compare, size_t children>
size_t HashHeap<Key, Value, Compare, children>::contains(const Key& key) {
    return m_map.contains(key);
}

template <class Key, class Value, class Compare, size_t children>
bool HashHeap<Key, Value, Compare, children>::insert(const Key& key, const Value& value) {
    auto result = m_map.insert(std::make_pair(key, std::make_pair(size(), value)));
    if (!result.second)
        return false;
    m_heap.push_back(key);
    internalFixUp(size() - 1);
    return true;
}

template <class Key, class Value, class Compare, size_t children>
Value& HashHeap<Key, Value, Compare, children>::at(const Key& key) {
    return m_map.at(key).second;
}

template <class Key, class Value, class Compare, size_t children>
void HashHeap<Key, Value, Compare, children>::erase(const Key& key) {
    if (key == m_heap.back()) {
        internalPopBack();
        return;
    }
    size_t index = m_map.at(key).first;
    internalSwap(index, size() - 1);
    internalPopBack();
    if (m_heap.at(index) > key)
        internalFixUp(index);
    else
        internalFixDown(index);
}

template <class Key, class Value, class Compare, size_t children>
std::pair<const Key&, Value&> HashHeap<Key, Value, Compare, children>::peek() {
    Key& key = m_heap.at(0);
    return std::pair<Key&, Value&>(key, at(key));
}

template <class Key, class Value, class Compare, size_t children>
void HashHeap<Key, Value, Compare, children>::pop() {
    erase(m_heap.front());
}

template <class Key, class Value, class Compare, size_t children>
size_t HashHeap<Key, Value, Compare, children>::size() {
    return m_map.size();
}

template <class Key, class Value, class Compare, size_t children>
bool HashHeap<Key, Value, Compare, children>::empty() {
    return m_map.empty();
}

template <class Key, class Value, class Compare, size_t children>
void HashHeap<Key, Value, Compare, children>::clear() {
    m_map.clear();
    m_heap.clear();
}

template <class Key, class Value, class Compare, size_t children>
void HashHeap<Key, Value, Compare, children>::internalFixUp(size_t i) {
    Compare compare{};
    for (; i > 0;) {
        size_t parent = (i - 1) / children;
        if (!compare(m_heap.at(i), m_heap.at(parent)))
            return;
        internalSwap(i, parent);
        i = parent;
    }
}

template <class Key, class Value, class Compare, size_t children>
void HashHeap<Key, Value, Compare, children>::internalFixDown(size_t i) {
    Compare compare{};
    for (size_t best = i;; i = best, best = i) {
        size_t childrenStart = i * children + 1;
        size_t childrenEnd = std::min(childrenStart + children, m_heap.size());
        for (size_t child = childrenStart; child < childrenEnd; child++) {
            if (compare(m_heap.at(child), m_heap.at(best)))
                best = child;
        }
        if (best == i)
            return;
        internalSwap(i, best);
    }
}

template <class Key, class Value, class Compare, size_t children>
void HashHeap<Key, Value, Compare, children>::internalPopBack() {
    m_map.erase(m_heap.back());
    m_heap.pop_back();
}

template <class Key, class Value, class Compare, size_t children>
void HashHeap<Key, Value, Compare, children>::internalSwap(size_t i, size_t j) {
    std::swap(m_heap.at(i), m_heap.at(j));
    std::swap(m_map.at(m_heap.at(i)).first, m_map.at(m_heap.at(j)).first);
}
