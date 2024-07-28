#pragma once

#include <iostream>
#include <stdexcept>


namespace mtm {

    template <typename T>
    class SortedList {
        /**
         *
         * the class should support the following public interface:
         * if needed, use =defualt / =delete
         *
         * constructors and destructor:
         * 1. SortedList() - creates an empty list.
         * 2. copy constructor
         * 3. operator= - assignment operator
         * 4. ~SortedList() - destructor
         *
         * iterator:
         * 5. class ConstIterator;
         * 6. begin method
         * 7. end method
         *
         * functions:
         * 8. insert - inserts a new element to the list
         * 9. remove - removes an element from the list
         * 10. length - returns the number of elements in the list
         * 11. filter - returns a new list with elements that satisfy a given condition
         * 12. apply - returns a new list with elements that were modified by an operation
         */
    public:
        // Constructors and destructor
        SortedList() : head(nullptr){}
        SortedList(const SortedList& list);
        SortedList& operator=(const SortedList& list);
        ~SortedList();

        // Iterator
        class ConstIterator;
        ConstIterator begin() const;
        ConstIterator end() const;

        // Functions
        void insert(const T& element);
        void remove(ConstIterator iterator);
        int length() const;

        template<typename Condition>
        SortedList filter(Condition c) const;

        template <typename Operation>
        SortedList apply(Operation op) const;

    private:
        struct Node{
            T data;
            Node* next;
            Node(const T& data) : data(data), next(nullptr) {}
        };
        Node* head;

    };

    template <class T>
    class SortedList<T>::ConstIterator {
        /**
         * the class should support the following public interface:
         * if needed, use =defualt / =delete
         *
         * constructors and destructor:
         * 1. a ctor(or ctors) your implementation needs
         * 2. copy constructor
         * 3. operator= - assignment operator
         * 4. ~ConstIterator() - destructor
         *
         * operators:
         * 5. operator* - returns the element the iterator points to
         * 6. operator++ - advances the iterator to the next element
         * 7. operator!= - returns true if the iterator points to a different element
         *
         */
    public:
        // Constructors
        ConstIterator(const ConstIterator& iterator) = default;
        ConstIterator& operator=(const ConstIterator& iterator) = default;
        ~ConstIterator() = default;

        // Operators
        const T& operator*() const;
        ConstIterator& operator++();
        bool operator!=(const ConstIterator& iterator);

        // Friends
        friend SortedList<T>;

    private:
        Node* current;

        // Constructors
        ConstIterator(Node* node) : current(node) {}
    };
}

//////////////////////////////////////// SortedList implementations /////////////////////////////////////////////////

template<typename T>
mtm::SortedList<T>::SortedList(const SortedList& list){
    if (list.head == nullptr) {
        return;
    }
    head = new Node(list.head->data);
    Node* new_current = head;
    Node* list_current = list.head->next;
    while (list_current != nullptr) {
        new_current->next = new Node(list_current->data);
        new_current = new_current->next;
        list_current = list_current->next;
    }
}

template<typename T>
mtm::SortedList<T>& mtm::SortedList<T>::operator=(const SortedList& list){
    if (this == &list) {
        return *this;
    }
    if (list.head != nullptr) {
        // Firstly allocating dynamic memory
        Node* new_head = new Node(list.head->data);
        Node* new_current = new_head;
        Node* list_current = list.head->next;

        while (list_current != nullptr) {
            new_current->next = new Node(list_current->data);
            new_current = new_current->next;
            list_current = list_current->next;
        }
        // Only Then deleting the existing data
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
        // Assigning the new allocated data
        head = new_head;
    }
    return *this;
}

template<typename T>
mtm::SortedList<T>::~SortedList(){
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

template<typename T>
void mtm::SortedList<T>::insert(const T& element){
    Node* new_element = new Node(element);
    if (head == nullptr || element > head->data) {
        new_element->next = head;
        head = new_element;
        return;
    }
    Node* current = head;
    while (current->next != nullptr && !(element > current->next->data)) {
        current = current->next;
    }
    new_element->next = current->next;
    current->next = new_element;
}

template<typename T>
void mtm::SortedList<T>::remove(mtm::SortedList<T>::ConstIterator iterator){
    if (iterator.current != nullptr && head != nullptr) {
        if (head == iterator.current) {
            Node *temp = head;
            head = head->next;
            delete temp;
        } else {
            for (mtm::SortedList<T>::ConstIterator it = this->begin(); it != this->end(); ++it) {
                if (it.current->next == iterator.current) {
                    Node *temp = it.current->next;
                    it.current->next = iterator.current->next;
                    delete temp;
                    break;
                }
            }
        }
    }
}

template<typename T>
int mtm::SortedList<T>::length() const{
    Node* current = head;
    int counter = 0;
    while (current != nullptr) {
        counter++;
        current = current->next;
    }
    return counter;
}

template <typename T>
template <typename Condition>
mtm::SortedList<T> mtm::SortedList<T>::filter(Condition c) const {
    mtm::SortedList<T> list;
    for (const T& element : *this) {
        if (c(element)) {
            list.insert(element);
        }
    }
    return list;
}

template <typename T>
template <typename Operation>
mtm::SortedList<T> mtm::SortedList<T>::apply(Operation op) const{
    mtm::SortedList<T> list;
    for (const T& element : *this) {
        list.insert(op(element));
    }
    return list;
}

template <typename T>
typename mtm::SortedList<T>::ConstIterator mtm::SortedList<T>::begin() const{
    return ConstIterator(head);
}

template <typename T>
typename mtm::SortedList<T>::ConstIterator mtm::SortedList<T>::end() const{
    return ConstIterator(nullptr);
}

//////////////////////////////////////// ConstIterator implementations /////////////////////////////////////////////////

template <typename T>
const T& mtm::SortedList<T>::ConstIterator::operator*() const {
    if (current == nullptr) {
        throw std::runtime_error("Trying to dereference a null iterator");
    }
    return current->data;
}

template <typename T>
typename mtm::SortedList<T>::ConstIterator& mtm::SortedList<T>::ConstIterator::operator++() {
    if (current == nullptr) {
        throw std::out_of_range("Trying to increment a null iterator");
    }
    current = current->next;
    return *this;
}

template <typename T>
bool mtm::SortedList<T>::ConstIterator::operator!=(const ConstIterator& iterator) {
    return current != iterator.current;
}
