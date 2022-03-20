#include "iostream"

#define TEST(x, y) { if (x != y) std::cerr << "\b" << __FILE__ << " failed on line " << __LINE__  << ":"<< std::endl << "received " << x << ", wanted " << y << std::endl; }

#include "../../squarecell.h"

int main(int argc, char const *argv[])
{
    Square s1 = {5, 2, 5, 0};
    TEST(get_coordinate_x(s1), 5);
    TEST(get_coordinate_y(s1), 2);

    Square s2 = {5, 5, 3, 1};
    TEST(get_coordinate_x(s2), 4);
    TEST(get_coordinate_y(s2), 4);

    Square s3 = {3, 5, 10, 1};
    TEST(get_coordinate_x(s3), -1);
    TEST(get_coordinate_y(s3), 1);

    return 0;
}
