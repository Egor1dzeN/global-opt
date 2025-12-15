#include <iostream>
#include "gtest/gtest.h"
//#include "gtest/gtest.h"



int main() {
    testing::InitGoogleTest();
    int res = RUN_ALL_TESTS();
    if (res != 0){
        std::cerr<<"Some error with test execution!\n";
    }
    return 0;
}
