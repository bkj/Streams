#ifndef STREAM_TERMINATORS_IMPL_H
#define STREAM_TERMINATORS_IMPL_H

template<typename T>
size_t Stream<T>::count() {
    check_vacant("count");
    size_t count = 0;
    while(source_->advance()) {
        source_->get();
        count++;
    }
    return count;
}

template<typename T>
template<typename Combiner>
T Stream<T>::reduce(const T& initial, Combiner&& combine) {
    T value = initial;
    while(source_->advance()) {
        value = combine(value, *source_->get());
    }
    return value;
}

template<typename Combiner>
T Stream<T>::reduce(Combiner&& combine) {
    if(source_->advance()) {
        return reduce(*source_->get(), std::forward<Function>(function));
    } else {
        throw EmptyStreamException("reduce");
    }
}

template<typename T>
template<typename Function>
T Stream<T>::no_identity_reduction(const std::string& name,
                                   Function&& function) {
    try {
        return reduce(std::forward<Function>(function));
    } catch(EmptyStreamException& e) {
        throw EmptyStreamException(name);
    }
}

template<typename T>
T Stream<T>::sum() {
    return no_identity_reduction("sum", std::plus<T>());
}

template<typename T>
T Stream<T>::sum(const T& identity) {
    return reduce(identity, std::plus<T>());
}

template<typename T>
T Stream<T>::product() {
    return no_identity_reduction("product", std::multiplies<T>());
}

template<typename T>
T Stream<T>::product(const T& identity) {
    return reduce(identity, std::multiplies<T>());
}

template<typename T>
template<typename Compare>
T Stream<T>::max(Compare&& compare) {
    return no_identity_reduction("max", [less=std::forward<Compare>(compare)]
        (T& a, T& b) {
            return less(a, b) ? b : a;
        });
}

template<typename T>
template<typename Compare>
T Stream<T>::min(Compare&& compare) {
    return no_identity_reduction("min", [less=std::forward<Compare>(compare)]
        (T& a, T& b) {
            return less(a, b) ? a : b;
        });
}

template<typename T>
template<typename Predicate>
bool Stream<T>::any(Predicate&& predicate) {
    check_vacant("any");
    while(source_->advance()) {
        if(predicate(*source_->get())) {
            return true;
        }
    }
    return false;
}

template<typename T>
template<typename Predicate>
bool Stream<T>::all(Predicate&& predicate) {
    check_vacant("all");
    while(source_->advance()) {
        if(!predicate(*source_->get())) {
            return false;
        }
    }
    return true;
}

template<typename T>
template<typename Predicate>
bool Stream<T>::none(Predicate&& predicate) {
    check_vacant("none");
    return !any(std::forward<Predicate>(predicate));
}

template<typename T>
template<typename OutputIterator>
void Stream<T>::copy_to(OutputIterator out) {
    check_vacant("copy_to");
    while(source_->advance()) {
        *out = *source_->get();
        out++;
    }
}

template<typename T>
template<typename OutputIterator>
void Stream<T>::move_to(OutputIterator out) {
    check_vacant("move_to");
    while(source_->advance()) {
        *out = std::move(*source_->get());
        out++;
    }
}

template<typename T>
void Stream<T>::print_to(std::ostream& os, const char* delimiter) {
    check_vacant("print_to");
    copy_to(std::ostream_iterator<T>(os, delimiter));
}


template<typename T>
std::vector<T> Stream<T>::to_vector() {
    check_vacant("to_vector");
    std::vector<T> result;
    copy_to(back_inserter(result));
    return result;
}

template<typename T>
std::list<T> Stream<T>::to_list() {
    check_vacant("to_list");
    std::list<T> result;
    copy_to(back_inserter(result));
    return result;
}

template<typename T>
template<typename Function>
void Stream<T>::for_each(Function&& function) {
    check_vacant("for_each");
    while(source_->advance()) {
        function(*source_->get());
    }
}

#endif