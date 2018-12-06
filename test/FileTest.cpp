#include <Stream.h>
#include <gmock/gmock.h>
#include <fstream>

using namespace testing;
using namespace stream;
using namespace stream::op;


bool is_even_fn(int x) {
    return x % 2 == 0;
}

int square(int num) {
    return num * num;
}

std::vector<int> get_data() {
    int s = 6;
    std::vector<int> student_marks(s);
    std::ifstream fin("num.txt");
    for (std::vector<int>::size_type i = 0; i < s; i++)
    {
        fin >> student_marks[i];
    }
    return student_marks;
}


struct Number {
    Number(int v) : value(v) {}

    int get_value_const() const { return value; }
    int get_value() { return value; }

    bool operator== (const Number& other) const {
        return value == other.value;
    }

    int value = 0;
};

TEST(FileTest, Function) {
    std::vector<int> sm = get_data();
       //EXPECT_THAT(MakeStream::from(sm) | filter(is_even_fn) | to_vector(),
       EXPECT_THAT(MakeStream::from(sm) | map_(square) | to_vector(),
                ElementsAre(0, 1, 4, 9, 16, 25));
}

TEST(FileTest, MemberFunction) {
    EXPECT_THAT(MakeStream::from({Number(1), Number(2), Number(3)})
                    | map_(&Number::get_value)
                    | to_vector(),
                ElementsAre(1, 2, 3));

    EXPECT_THAT(MakeStream::from({Number(1), Number(2), Number(3)})
                    | map_(&Number::get_value_const)
                    | to_vector(),
                ElementsAre(1, 2, 3));
}
