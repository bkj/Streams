#include <Stream.h>
#include <gmock/gmock.h>
#include <fstream>

using namespace testing;
using namespace stream;
using namespace stream::op;

std::vector<int> make_vector() {
    std::vector<int> x(4);
    for (int i = 0; i < 4; i++) {
        x[i] = i;
    }
    return x;
}

auto square = map_([](auto&& x) { return x * x; });

TEST(FileTest, Function) {
    std::vector<int> data = make_vector();
    EXPECT_THAT(
        MakeStream::from(data) | square | to_vector(),
        ElementsAre(0, 1, 4, 9)
    );
}
