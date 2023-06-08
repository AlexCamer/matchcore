#pragma once

#include <algorithm>
#include <vector>
#include <utility>

#include "tsl/robin_map.h"
#include "../include/types.h"

template <class Key, class Value, class Compare, usize Children = 16>
class HashHeap {
public:
    bool contains(const Key& key);
    bool insert(const Key& key, const Value& value);
    void erase(const Key& key);
    Value& operator[](const Key& key);

    std::pair<const Key&, Value&> top();
    void pop();

    usize size();
    bool empty();

private:
    using IndexedValue = typename std::pair<usize, Value>;
    using HashMap = typename tsl::robin_map<Key, IndexedValue>;
    using Heap = typename std::vector<Key>;

    HashMap m_hash_map;
    Heap m_heap;

    void internal_fix_up(size_t i);
    void internal_fix_down(size_t i);
    void internal_pop_back();
    void internal_swap(size_t i, size_t j);
};

template <class Key, class Value, class Compare, usize Children>
bool HashHeap<Key, Value, Compare, Children>::contains(const Key& key) {
    return m_hash_map.contains(key);
}

template <class Key, class Value, class Compare, usize Children>
bool HashHeap<Key, Value, Compare, Children>::insert(const Key& key, const Value& value) {
    if (!m_hash_map.try_emplace(key, size(), value).second)
        return false;
    m_heap.push_back(key);
    internal_fix_up(size() - 1);
    return true;
}

template <class Key, class Value, class Compare, usize Children>
Value& HashHeap<Key, Value, Compare, Children>::operator[](const Key& key) {
    return m_hash_map[key];
}

template <class Key, class Value, class Compare, usize Children>
void HashHeap<Key, Value, Compare, Children>::erase(const Key& key) {
    usize index = m_hash_map[key].first;
    internal_swap(index, size() - 1);
    internal_pop_back();
    if (index < size())
        (m_heap[index] > key) ? internal_fix_up(index) : internal_fix_down(index);
}

template <class Key, class Value, class Compare, usize Children>
std::pair<const Key&, Value&> HashHeap<Key, Value, Compare, Children>::top() {
    return std::pair<const Key&, Value&>{ m_heap.front(), m_hash_map[m_heap.front()] };
}

template <class Key, class Value, class Compare, usize Children>
void HashHeap<Key, Value, Compare, Children>::pop() {
    erase(m_heap.front());
}

template <class Key, class Value, class Compare, usize Children>
size_t HashHeap<Key, Value, Compare, Children>::size() {
    return m_hash_map.size();
}

template <class Key, class Value, class Compare, usize Children>
bool HashHeap<Key, Value, Compare, Children>::empty() {
    return m_hash_map.empty();
}

template <class Key, class Value, class Compare, usize Children>
void HashHeap<Key, Value, Compare, Children>::internal_fix_up(size_t i) {
    Compare compare{};
    for (; i > 0;) {
        size_t parent = (i - 1) / Children;
        if (!compare(m_heap[i], m_heap[parent]))
            return;
        internal_swap(i, parent);
        i = parent;
    }
}

template <class Key, class Value, class Compare, usize Children>
void HashHeap<Key, Value, Compare, Children>::internal_fix_down(size_t i) {
    Compare compare{};
    for (size_t best = i;; i = best, best = i) {
        size_t children_start = i * Children + 1;
        size_t children_end = std::min(children_start + Children, m_heap.size());
        for (size_t child = children_start; child < children_end; child++)
            best = (compare(m_heap[child], m_heap[best])) ? child : best;
        if (best == i)
            return;
        internal_swap(i, best);
    }
}

template <class Key, class Value, class Compare, usize Children>
void HashHeap<Key, Value, Compare, Children>::internal_pop_back() {
    m_hash_map.erase(m_heap.back());
    m_heap.pop_back();
}

template <class Key, class Value, class Compare, usize Children>
void HashHeap<Key, Value, Compare, Children>::internal_swap(size_t i, size_t j) {
    std::swap(m_heap[i], m_heap[j]);
    std::swap(m_hash_map[m_heap[i]].first, m_hash_map[m_heap[j]].first);
}
