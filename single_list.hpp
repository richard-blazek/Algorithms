#ifndef SINGLE_LIST_H_INCLUDED
#define SINGLE_LIST_H_INCLUDED

#include <utility>

template<typename T>
class single_list
{
private:
    struct node
    {
        T value;
        node* next;

        node(T value, node* next)
            : value(std::move(value)), next(next) {}

        node(const node& src)
            : value(src.value), next(src.next ? new node(*src.next) : nullptr) {}

        node(node&& src) = delete;
        node& operator=(const node&) = delete;
        node& operator=(node&&) = delete;

        ~node()
        {
            delete next;
        }
    };
    node* root = nullptr;
public:
    single_list() = default;

    single_list(const single_list& src)
        : root(src.root ? new node(*src.root) : nullptr) {}

    single_list& operator=(const single_list& src)
    {
        delete root;
        root = src.root ? new node(*src.root) : nullptr;
        return *this;
    }

    single_list(single_list&& src) : root(src.root)
    {
        src.root = nullptr;
    }

    single_list& operator=(single_list&& src)
    {
        delete root;
        root = src.root;
        src.root = nullptr;
        return *this;
    }

    ~single_list()
    {
        delete root;
    }

    class iterator
    {
    private:
        node** ptr;
    public:
        iterator(node** current) : ptr(current) {}

        bool operator==(const iterator& second)
        {
            node* a = ptr ? *ptr : nullptr;
            node* b = second.ptr ? *second.ptr : nullptr;
            return a == b;
        }

        bool operator!=(const iterator& second)
        {
            return !(*this == second);
        }

        T& operator*()
        {
            return (**ptr).value;
        }

        iterator& operator++()
        {
            ptr = &(**ptr).next;
            return *this;
        }

        iterator operator++(int)
        {
            iterator old = *this;
            ptr = &(**ptr).next;
            return old;
        }

        void insert(T value)
        {
            *ptr = new node(std::move(value), *ptr);
        }

        void erase()
        {
            if (*ptr)
            {
                node* new_this = (**ptr).next;
                (**ptr).next = nullptr;
                delete *ptr;
                *ptr = new_this;
            }
        }
    };

    iterator begin()
    {
        return iterator(&root);
    }

    iterator end()
    {
        return iterator(nullptr);
    }

    bool empty()
    {
        return root == nullptr;
    }

    void push_front(T value)
    {
        iterator b = begin();
        b.insert(std::move(value));
    }

    void pop_front()
    {
        iterator b = begin();
        b.erase();
    }
};

#endif