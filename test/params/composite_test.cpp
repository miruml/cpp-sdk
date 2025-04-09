// internal
#include <miru/params/parameter.hpp>
#include <miru/params/exceptions.hpp>
#include <miru/params/scalar.hpp>
#include <miru/utils.hpp>
#include <test/utils_test.hpp>

// external
#include <gtest/gtest.h>

namespace test::params {

// =============================== OBJECT CONSTRUCTOR ============================= //
class MapConstructor : public ::testing::Test {
protected:
};

TEST_F(MapConstructor, empty_map) {
    EXPECT_THROW(
        miru::params::Map(std::vector<miru::params::Parameter>()),
        std::invalid_argument
    );
}

TEST_F(MapConstructor, duplicate_field_names) {
    miru::params::Parameter param1("field1", miru::params::Scalar("value1"));
    miru::params::Parameter param2("field1", miru::params::Scalar("value2"));
    EXPECT_THROW(
        miru::params::Map(std::vector<miru::params::Parameter>({param1, param2})),
        std::invalid_argument
    );
    try {
        miru::params::Map(std::vector<miru::params::Parameter>({param1, param2}));
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

}  // namespace test::params
