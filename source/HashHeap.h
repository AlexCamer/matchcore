#pragma once

#include <algorithm>
#include <vector>
#include <utility>

//#include "absl/container/flat_hash_map.h"
#include "tsl/robin_map.h"
//#include "ankerl.h"

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
    struct Hash {
        size_t operator()(const uint32_t& k) const {
            return k;
        }
    };

    tsl::robin_map<Key, std::pair<size_t, Value>> m_map;
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
        if (!compare(m_heap[i], m_heap[parent]))
            return;
        internalSwap(i, parent);
        i = parent;
    }
}

template <class Key, class Value, class Compare, size_t children>
void HashHeap<Key, Value, Compare, children>::internalFixDown(size_t i) {
    Compare compare{};
    for (size_t best = i;; i = best, best = i) {
        size_t child = i * children + 1;
        // size_t childrenEnd = std::min(childrenStart + children, m_heap.size());
        size_t sz = size();
        if (child + 0 < sz && compare(m_heap[child + 0], m_heap[best]))
            best = child + 0;
        if (child + 1 < sz && compare(m_heap[child + 1], m_heap[best]))
            best = child + 1;
        if (child + 2 < sz && compare(m_heap[child + 2], m_heap[best]))
            best = child + 2;
        if (child + 3 < sz && compare(m_heap[child + 3], m_heap[best]))
            best = child + 3;
        if (child + 4 < sz && compare(m_heap[child + 4], m_heap[best]))
            best = child + 4;
        if (child + 5 < sz && compare(m_heap[child + 5], m_heap[best]))
            best = child + 5;
        if (child + 6 < sz && compare(m_heap[child + 6], m_heap[best]))
            best = child + 6;
        if (child + 7 < sz && compare(m_heap[child + 7], m_heap[best]))
            best = child + 7;
        if (child + 8 < sz && compare(m_heap[child + 8], m_heap[best]))
            best = child + 8;
        if (child + 9 < sz && compare(m_heap[child + 9], m_heap[best]))
            best = child + 9;
        if (child + 10 < sz && compare(m_heap[child + 10], m_heap[best]))
            best = child + 10;
        if (child + 11 < sz && compare(m_heap[child + 11], m_heap[best]))
            best = child + 11;
        if (child + 12 < sz && compare(m_heap[child + 12], m_heap[best]))
            best = child + 12;
        if (child + 13 < sz && compare(m_heap[child + 13], m_heap[best]))
            best = child + 13;
        if (child + 14 < sz && compare(m_heap[child + 14], m_heap[best]))
            best = child + 14;
        if (child + 15 < sz && compare(m_heap[child + 15], m_heap[best]))
            best = child + 15;
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
    std::swap(m_heap[i], m_heap[j]);
    std::swap(m_map[m_heap[i]].first, m_map[m_heap[j]].first);
}
