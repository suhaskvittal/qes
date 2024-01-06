/*
 *  author: Suhas Vittal
 *  date:   5 January 2024
 * */

#include <sstream>

namespace qes {

template <class T, class U>
Instruction<T, U>::Instruction()
    :name(),
    operands(),
    annotations(),
    property_map()
{}

template <class T, class U>
Instruction<T, U>::Instruction(std::string name, std::vector<T> operands)
    :name(name),
    operands(operands),
    annotations(),
    property_map()
{}

template <class T, class U>
Instruction<T, U>::Instruction(const Instruction<T, U>& other)
    :name(other.name),
    operands(other.operands),
    annotations(other.annotations),
    property_map(other.property_map)
{}

template <class T, class U>
Instruction<T, U>::Instruction(Instruction<T, U>&& other)
    :name(std::move(other.name)),
    operands(std::move(other.operands)),
    annotations(std::move(other.annotations)),
    property_map(std::move(other.property_map))
{}

template <class T, class U>
template <class X>
Instruction<T, U>::Instruction(std::string name, std::vector<X> _operands)
    :name(name),
    operands(_operands.size())
{
    for (size_t i = 0; i < _operands.size(); i++) {
        operands[i] = _operands[i];
    }
}

template <class T, class U>
template <class ITER>
Instruction<T, U>::Instruction(std::string name, ITER begin, ITER end)
    :name(name),
    operands()
{
    for (auto it = begin; it != end; it++) {
        T op(*it);
        operands.push_back(op);
    }
}

template <class T, class U> inline Instruction<T, U>&
Instruction<T, U>::operator=(const Instruction<T, U>& other) {
    name = other.name;
    operands = other.operands;
    annotations = other.annotations;
    property_map = other.property_map;
    return *this;
}

template <class T, class U> inline T
Instruction<T, U>::get(size_t k) const {
    return operands.at(k);
}

template <class T, class U>
template <class X> inline X
Instruction<T, U>::get(size_t k) const {
    return std::get<X>(operands.at(k));
}

template <class T, class U> inline void
Instruction<T, U>::put(std::string ann) {
    annotations.insert(ann);
}

template <class T, class U> inline void
Instruction<T, U>::put(std::string p, U v) {
    property_map[p] = v;
}

template <class T, class U> inline bool
Instruction<T, U>::has_annotation(std::string x) const {
    return annotations.count(x);
}

template <class T, class U> inline bool
Instruction<T, U>::has_property(std::string x) const {
    return property_map.count(x);
}

template <class T, class U> inline U
Instruction<T, U>::get_property(std::string x) const {
    return property_map.at(x);
}

template <class T, class U>
template <class X> inline X
Instruction<T, U>::get_property(std::string x) const {
    return std::get<X>(property_map.at(x));
}

template <class T, class U> inline void
Instruction<T, U>::join(const Instruction<T, U>& other) {
    // If the names are not equal, exit.
    if (name != other.get_name()) return;
    // Otherwise, good to go.
    operands.insert(operands.end(), other.operands.cbegin(), other.operands.cend());
}

template <class T, class U> inline std::string
Instruction<T, U>::get_name() const {
    return name;
}

template <class T, class U> inline std::vector<T>
Instruction<T, U>::get_operands() const {
    return operands;
}

template <class T, class U> inline std::set<annotation_t>
Instruction<T, U>::get_annotations() const {
    return annotations;
}

template <class T, class U> inline std::map<std::string, U>
Instruction<T, U>::get_property_map() const {
    return property_map;
}

template <class T, class U> std::string
print_inst(const Instruction<T, U>& inst, bool print_inline) {
    const std::string whitespace = print_inline ? " " : "\n";
    std::ostringstream sout;
    // Dump annotations and properties first.
    for (annotation_t x : inst.get_annotations()) {
        sout << "@annotation " << x << whitespace;
    }
    for (auto pair : inst.get_property_map()) {
        sout << "@property " << pair.first << " ";
        std::visit([&] (auto x) { sout << x; }, pair.second);
        sout << whitespace;
    }
    // Finally dump the instruction contents
    sout << inst.get_name() << "\t";
    bool first = true;
    for (U op : inst.get_operands()) {
        if (!first) sout << ", ";
        first = false;
        std::visit([&] (auto x) { sout << x; }, op);
    }
    sout << ";";
    return sout.str();
}

template <class T, class U> inline std::string
print_prog(const Program<T, U>& program) {
    std::string out;
    for (size_t i = 0; i < program.size(); i++) {
        if (i > 0) out += "\n";
        out += print_inst(program.at(i), false);
    }
    return out;
}

}   // qes
