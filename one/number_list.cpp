#include <number_list.hpp>

NumberList::NumberList(std::string fname) :
  counts{} {
    std::ifstream st(fname);
    if(! st.is_open() ) {
        throw FileNotFoundError(fname);
    }
    st >> std::skipws;
    int x;
    while(! st.eof()) {
        st >> x;
        if(st.fail() && ! st.eof()) {
            throw BadInputError();
        }
        numbers.push_back(x);
    }
}

void NumberList::printStemAndLeaf() {
    for(auto q: numbers) {
        if(q > 255 || q < 0) {
            throw BadNumberError(q);
        }
        size_t index = std::floor(q / 4.0);
        counts[index] = counts[index] + 1;
    }
    for(auto q: counts) {
        std::cout << q << std::endl;
    }
}
