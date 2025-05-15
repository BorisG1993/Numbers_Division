#ifndef SUBPARTITION_GENERATOR_H 
#define SUBPARTITION_GENERATOR_H

#include <vector>




// allows to run partition generator with fixed prefix
template <typename T, typename BaseGeneratorIterator>
class PartitionGeneratorWrapper {

public:

    PartitionGeneratorWrapper(const std::vector<T>& prefix, BaseGeneratorIterator iter)
        : prefix_(prefix), base_gen_(iter) {};

    std::vector<T> operator*() const {
        std::vector<T> result = prefix_;
        const std::vector<T>& subpartition = *base_gen_;
        result.insert(result.end(), subpartition.begin(), subpartition.end());
        return result;
    }

    PartitionGeneratorWrapper& operator++() {
        ++base_gen_;
        return *this;
    }

    bool operator!=(const PartitionGeneratorWrapper & other) const {
        return base_gen_ != other.base_gen_;
    }
        
private:
    std::vector<T> prefix_;
    BaseGeneratorIterator base_gen_;
};


#endif // SUBPARTITION_GENERATOR_H

