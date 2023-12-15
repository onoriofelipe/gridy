#include <iostream>
#include <boost/signals2.hpp>

int main(){
    boost::signals2::signal<void (int a)> emitter;
    auto lambda = [](int x){ std::cout << x << std::endl; };
    emitter.connect(lambda);
    std::cout << "aaaaaaaaaaaaaaaaaa" << std::endl;
    emitter(23);
    emitter(14);
}
