﻿// разработать итераторы для контейнера VectorList, который хранит данные в списке векторов. Для упрощения задачи от вас требуется реализовать только 
// константные итераторы (прямой и обратный), а также реализовать методы begin, end, rbegin и rend. Для того, чтобы разобраться в устройстве контейнера, 
// начните работу над задачей с реализации метода size. Заполнение контейнера будет происходить при помощи метода append, который уже реализован 
// (реализация в комментариях дана для ознакомления). Метод append гарантирует, что в списке не будет пустых векторов.

#include <iostream>
#include <vector>
#include <list>

template<class T>
class VectorList
{
private:
    using VectT = std::vector<T>;
    using ListT = std::list<VectT>;

public:
    using value_type = T;

    VectorList() = default;
    VectorList(VectorList const&) = default;
    VectorList(VectorList&&) = default;

    VectorList& operator=(VectorList&&) = default;
    VectorList& operator=(VectorList const&) = default;

    // метод, который будет использоваться для заполнения VectorList
    // гарантирует, что в списке не будет пустых массивов
    template<class It>
    void append(It p, It q); // определена снаружи
    //{
    //    if (p != q)
    //        data_.push_back(VectT(p, q));
    //}


    bool empty() const { return size() == 0; }

    // определите метод size
    size_t size() const
    {
        size_t count = 0;
        for (auto it = data_.cbegin(); it != data_.cend(); it++)
            count += (*it).size();
        return count;
    }

    // определите const_iterator
    class const_iterator : public std::iterator<std::bidirectional_iterator_tag, const T>
    {
    public:
        using itv = typename VectT::const_iterator;
        using itl = typename ListT::const_iterator;
        const_iterator() = default;
        const_iterator(const const_iterator&) = default;
        const_iterator(itl l, itv v, ListT const* data)
            :c_itl(l), c_itv(v), p_data(data) {};
        const_iterator& operator++() {
            if ((++c_itv) == c_itl->cend() && (++c_itl) != p_data->end()) {
                c_itv = c_itl->cbegin();
            }
            return *this;
        }
        const_iterator operator++(int) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }
        const_iterator& operator--() {
            if (c_itv != c_itl->cbegin()) {
                c_itv--;
                return *this;
            }
            if (c_itl != p_data->cbegin()) {
                c_itl--;
                c_itv = std::prev(c_itl->cend());
            }
            return *this;
        }
        const_iterator operator--(int) {
            auto tmp = *this;
            --(*this);
            return tmp;
        }
        const T& operator*() const{
            return *c_itv;
        }
        const T* operator->() const {
            return &(*c_itv);
        }
        bool operator== (const const_iterator& other) const {
            return c_itv == other.c_itv;
        }
        bool operator!= (const const_iterator& other) const {
            return !(c_itv == other.c_itv);
        }       
    private:
        ListT const* p_data;
        itv c_itv;
        itl c_itl;
    };

    // определите методы begin / end
    const_iterator begin() const {
        if (empty())
            return const_iterator();
        auto bl = data_.cbegin();
        auto bv = bl->cbegin();
        return const_iterator(bl, bv, &data_);
    }
    const_iterator end()   const {
        if (empty())
            return const_iterator();
        auto el = --data_.cend();
        auto ev = el->cend();
        return const_iterator(el, ev, &data_);
    }

    // определите const_reverse_iterator
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // определите методы rbegin / rend
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }

private:
    ListT data_;
};

int main() {
    return 0;
}