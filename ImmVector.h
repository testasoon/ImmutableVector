#pragma once
#include <memory>
#include <stdexcept>
#include "ImmVectorImpl.h"



template <class T>
class ImmutableVector {
public:
    ImmutableVector()
        : size_(0), data_(std::make_shared<ImmutableVectorImpl<T>>()) {}

    explicit ImmutableVector(size_t count, const T &value = T())
        : ImmutableVector() {
        for (size_t i = 0; i < count; ++i) {
            data_ = std::make_shared<ImmutableVectorImpl<T>>(
                data_->PushBack(value, size_++)
            );
        }
    }

    ImmutableVector(const ImmutableVector &other) = delete;
    ImmutableVector(ImmutableVector &&other) = delete;
    ImmutableVector operator=(const ImmutableVector &&other) = delete;
    ImmutableVector operator=(ImmutableVector &&other) = delete;

    template <typename Iterator>
    ImmutableVector(Iterator first, Iterator last) : ImmutableVector() {
        while (first != last) {
            data_ = std::make_shared<ImmutableVectorImpl<T>>(
                data_->PushBack(*first, size_++)
            );
            ++first;
        }
    }

    ImmutableVector(std::initializer_list<T> l) : ImmutableVector() {
        for (auto& item: l) {
            data_ = std::make_shared<ImmutableVectorImpl<T>>(
                data_->PushBack(item, size_++)
                
            );
        }
    }

    ImmutableVector Set(size_t index, const T& value) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range!");
        }
        auto data = std::make_shared<ImmutableVectorImpl<T>>(
            data_->Set(index, value)
        );
        return ImmutableVector(size_, data);
    }

    const T& Get(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range!");
        }
        return data_->Get(index);
    }

    ImmutableVector PushBack(const T& value) {
        if (data_ == nullptr) {
            throw std::runtime_error("data_ is nullptr");
        }
        auto data = std::make_shared<ImmutableVectorImpl<T>>(
            data_->PushBack(value, size_)
        );
        return ImmutableVector(size_ + 1, data);
    }

    ImmutableVector PopBack() {
        if (size_ == 0) {
            throw std::out_of_range("Vector empty!");
        }
        auto data = std::make_shared<ImmutableVectorImpl<T>>(
            data_->PopBack(size_ - 1)
        );
        return ImmutableVector(size_ - 1, data);
    }

    size_t Size() const {
        return size_;
    }
private:
    ImmutableVector(size_t size, std::shared_ptr<ImmutableVectorImpl<T>> data)
        : size_(size), data_(data) {}

    size_t size_;
    std::shared_ptr<ImmutableVectorImpl<T>> data_;
};
