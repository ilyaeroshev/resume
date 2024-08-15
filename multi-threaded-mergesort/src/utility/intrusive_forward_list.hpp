#pragma once

#include <cassert>
#include <cstddef>

template <typename T>
struct IntrusiveForwardListNode {
  // Shortcut
  using Node = IntrusiveForwardListNode;

  Node* next_ = nullptr;

  Node* Next() noexcept { 
    return next_; 
  }

  void SetNext(Node* node) noexcept { 
    next_ = node; 
  }

  void ResetNext() noexcept { 
    SetNext(nullptr);
  }

  void LinkBefore(Node* node) noexcept { 
    SetNext(node); 
  }

  T* AsItem() noexcept { 
    return static_cast<T*>(this); 
  }
};

template <typename T>
class IntrusiveForwardList {
  // Node type
  using Node = IntrusiveForwardListNode<T>;

  // Shortcut
  using List = IntrusiveForwardList<T>;

 public:
  IntrusiveForwardList() = default;

  // Non-copyable
  IntrusiveForwardList(const List&) = delete;
  IntrusiveForwardList& operator=(const List&) = delete;

  // Move-constructable
  IntrusiveForwardList(List&& other) {
    head_ = other.head_;
    tail_ = other.tail_;
    size_ = other.size_;
    other.Reset();
  }

  List& operator=(List&&) = delete;

  bool IsEmpty() const noexcept { 
    return head_ == nullptr; 
  }

  bool NonEmpty() const noexcept {
    return head_;
  }

  size_t Size() const noexcept {
    return size_;
  }

  void PushFront(Node* node) noexcept {
    if (IsEmpty()) {
      tail_ = node;
    }
    node->SetNext(head_);
    head_ = node;
    ++size_;
  }

  void PushBack(Node* node) noexcept {
    node->SetNext(nullptr);

    if (IsEmpty()) {
      head_ = tail_ = node;
    } else {
      tail_->SetNext(node);
      tail_ = node;
    }

    ++size_;
  }

  T* PopFront() noexcept {
    if (IsEmpty()) {
      return nullptr;
    }

    Node* res = head_;

    head_ = head_->Next();

    if (head_ == nullptr) {
      tail_ = nullptr;
    }

    --size_;

    res->SetNext(nullptr);
    return res->AsItem();
  }

  ~IntrusiveForwardList() {
    assert(IsEmpty());
  }

 private:
  void Reset() noexcept {
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
  }

  Node* head_{nullptr};
  Node* tail_{nullptr};
  size_t size_{0};
};