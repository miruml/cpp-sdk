// These tests were originally taken from the rclcpp parameter tests:
// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/test/rclcpp/test_parameter.cpp 
// and modified to work with miru::params::Parameter

// std
#include <string>

// internal
#include "miru/params/exceptions.hpp"
#include "miru/params/parameter.hpp"

// external
#include <gtest/gtest.h>

// ================================ CONSTRUCTORS =================================== //
class ParameterConstructors : public ::testing::Test {
protected:
};

TEST_F(ParameterConstructors, not_set_variant) {
    miru::params::Parameter not_set_variant1 = miru::params::Parameter();
    miru::params::Parameter not_set_variant2 = miru::params::Parameter();
    EXPECT_EQ(not_set_variant1, not_set_variant2);
}

TEST_F(ParameterConstructors, bool_variant) {
    miru::params::ParameterValue bool_value(true);
    miru::params::Parameter bool_variant1 = miru::params::Parameter(
        "bool_param", bool_value
    );
    miru::params::Parameter bool_variant2 = miru::params::Parameter(
        "bool_param", true
    );
    EXPECT_EQ(bool_variant1, bool_variant2);
}

TEST_F(ParameterConstructors, int_variant) {
    miru::params::ParameterValue int_value(4);
    miru::params::Parameter int_variant1 = miru::params::Parameter(
        "int_param", int_value
    );
    miru::params::Parameter int_variant2 = miru::params::Parameter(
        "int_param", 4
    );
    EXPECT_EQ(int_variant1, int_variant2);
}

TEST_F(ParameterConstructors, double_variant) {
    miru::params::ParameterValue double_value(4.0);
    miru::params::Parameter double_variant1 = miru::params::Parameter(
        "double_param", double_value
    );
    miru::params::Parameter double_variant2 = miru::params::Parameter(
        "double_param", 4.0
    );
    EXPECT_EQ(double_variant1, double_variant2);
}

TEST_F(ParameterConstructors, string_variant) {
    miru::params::ParameterValue string_value("test");
    miru::params::Parameter string_variant1 = miru::params::Parameter(
        "string_param", string_value
    );
    miru::params::Parameter string_variant2 = miru::params::Parameter(
        "string_param", "test"
    );
    EXPECT_EQ(string_variant1, string_variant2);
}

TEST_F(ParameterConstructors, bool_array_variant) {
    miru::params::ParameterValue bool_array_value(
        std::vector<bool>{true, false, true}
    );
    miru::params::Parameter bool_array_variant1 = miru::params::Parameter(
        "bool_array_param", bool_array_value
    );
    miru::params::Parameter bool_array_variant2 = miru::params::Parameter(
        "bool_array_param", std::vector<bool>{true, false, true}
    );
    EXPECT_EQ(bool_array_variant1, bool_array_variant2);
}

TEST_F(ParameterConstructors, int_array_variant) {
    miru::params::ParameterValue int_array_value(
        std::vector<int>{1, 2, 3}
    );
    miru::params::Parameter int_array_variant1 = miru::params::Parameter(
        "int_array_param", int_array_value
    );
    miru::params::Parameter int_array_variant2 = miru::params::Parameter(
        "int_array_param", std::vector<int>{1, 2, 3}
    );
    EXPECT_EQ(int_array_variant1, int_array_variant2);
}

TEST_F(ParameterConstructors, double_array_variant) {
    miru::params::ParameterValue double_array_value(
        std::vector<double>{1.0, 2.0, 3.0}
    );
    miru::params::Parameter double_array_variant1 = miru::params::Parameter(
        "double_array_param", double_array_value
    );
    miru::params::Parameter double_array_variant2 = miru::params::Parameter(
        "double_array_param", std::vector<double>{1.0, 2.0, 3.0}
    );
    EXPECT_EQ(double_array_variant1, double_array_variant2);
}

TEST_F(ParameterConstructors, string_array_variant) {
    miru::params::ParameterValue string_array_value(
        std::vector<std::string>{"test1", "test2", "test3"}
    );
    miru::params::Parameter string_array_variant1 = miru::params::Parameter(
        "string_array_param", string_array_value
    );
    miru::params::Parameter string_array_variant2 = miru::params::Parameter(
        "string_array_param", std::vector<std::string>{"test1", "test2", "test3"}
    );
    EXPECT_EQ(string_array_variant1, string_array_variant2);
}

TEST_F(ParameterConstructors, null_variant) {
    miru::params::Parameter null_variant1 = miru::params::Parameter(
        "null_param", nullptr
    );
    miru::params::Parameter null_variant2 = miru::params::Parameter(
        "null_param", miru::params::ParameterValue(nullptr)
    );
    EXPECT_EQ(null_variant1, null_variant2);
}

TEST_F(ParameterConstructors, scalar_variant) {
    miru::params::ParameterValue scalar_value(miru::params::Scalar("test"));
    miru::params::Parameter scalar_variant1 = miru::params::Parameter(
        "scalar_param", scalar_value
    );
    miru::params::Parameter scalar_variant2 = miru::params::Parameter(
        "scalar_param", miru::params::Scalar("test")
    );
    EXPECT_EQ(scalar_variant1, scalar_variant2);
}

TEST_F(ParameterConstructors, scalar_array_variant) {
    miru::params::ParameterValue scalar_array_value(
        std::vector<miru::params::Scalar>{
            miru::params::Scalar("true"),
            miru::params::Scalar("false"),
            miru::params::Scalar("true")
        }
    );
    miru::params::Parameter scalar_array_variant1 = miru::params::Parameter(
        "scalar_array_param", scalar_array_value
    );
    miru::params::Parameter scalar_array_variant2 = miru::params::Parameter(
        "scalar_array_param", std::vector<miru::params::Scalar>{
            miru::params::Scalar("true"),
            miru::params::Scalar("false"),
            miru::params::Scalar("true")
        }
    );
    EXPECT_EQ(scalar_array_variant1, scalar_array_variant2);
}

TEST_F(ParameterConstructors, nested_array_variant) {
    miru::params::ParameterValue nested_array_value(
        miru::params::NestedArray{
            std::vector<miru::params::Parameter>{
                miru::params::Parameter("nested_param1", miru::params::Scalar("test1")),
                miru::params::Parameter("nested_param2", miru::params::Scalar("test2"))
            }
        }
    );
    miru::params::Parameter nested_array_variant1 = miru::params::Parameter(
        "nested_array_param", nested_array_value
    );
    miru::params::Parameter nested_array_variant2 = miru::params::Parameter(
        "nested_array_param", miru::params::NestedArray{
            std::vector<miru::params::Parameter>{
                miru::params::Parameter("nested_param1", miru::params::Scalar("test1")),
                miru::params::Parameter("nested_param2", miru::params::Scalar("test2"))
            }
        }
    );
    EXPECT_EQ(nested_array_variant1, nested_array_variant2);
}

TEST_F(ParameterConstructors, object_variant) {
    miru::params::ParameterValue object_value(
        miru::params::Object{
            std::vector<miru::params::Parameter>{
                miru::params::Parameter("object_param1", miru::params::Scalar("test1")),
                miru::params::Parameter("object_param2", miru::params::Scalar("test2"))
            }
        }
    );
    miru::params::Parameter object_variant1 = miru::params::Parameter(
        "object_param", object_value
    );
    miru::params::Parameter object_variant2 = miru::params::Parameter(
        "object_param", miru::params::Object{
            std::vector<miru::params::Parameter>{
                miru::params::Parameter("object_param1", miru::params::Scalar("test1")),
                miru::params::Parameter("object_param2", miru::params::Scalar("test2"))
            }
        }
    );
    EXPECT_EQ(object_variant1, object_variant2);
}

TEST_F(ParameterConstructors, object_array_variant) {
    miru::params::ParameterValue object_array_value(
        miru::params::ObjectArray{
            std::vector<miru::params::Parameter>{
                miru::params::Object{
                    std::vector<miru::params::Parameter>{
                        miru::params::Parameter("object_param1", miru::params::Scalar("test1")),
                        miru::params::Parameter("object_param2", miru::params::Scalar("test2"))
                    }
                }
            }
        }
    );
    miru::params::Parameter object_array_variant1 = miru::params::Parameter(
        "object_array_param", object_array_value
    );
    miru::params::Parameter object_array_variant2 = miru::params::Parameter(
        "object_array_param", miru::params::ObjectArray{
            std::vector<miru::params::Parameter>{
                miru::params::Object{
                    std::vector<miru::params::Parameter>{
                        miru::params::Parameter("object_param1", miru::params::Scalar("test1")),
                        miru::params::Parameter("object_param2", miru::params::Scalar("test2"))
                    }
                }
            }
        }
    );
    EXPECT_EQ(object_array_variant1, object_array_variant2);
}

// ================================= CONVERSIONS =================================== //
void test_conversion_funcs_throw_type_exceptions(
    const miru::params::Parameter & param,
    std::vector<std::function<void()>> conversion_funcs,
    const miru::params::ParameterType target_type 
) {
    for (const auto & conversion_func : conversion_funcs) {
        EXPECT_THROW(
            conversion_func(),
            miru::params::InvalidParameterTypeWithParamName
        ) << "Failed to throw InvalidParameterTypeWithParamName for target parameter type '" << target_type << "'";
    }
}

void test_conversion_type_exceptions(
    const miru::params::Parameter & param,
    const std::vector<miru::params::ParameterType> & exclude 
) {
    // boolean
    if (std::find(exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_BOOL) == exclude.end()) {
        test_conversion_funcs_throw_type_exceptions(
            param,
            {
                [&]() { param.as_bool(); },
                [&]() { param.get_value<miru::params::ParameterType::PARAMETER_BOOL>(); },
                [&]() { param.get_value<bool>(); }
            },
            miru::params::ParameterType::PARAMETER_BOOL
        );
    }

    // integer
    if (std::find(exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_INTEGER) == exclude.end()) {
        test_conversion_funcs_throw_type_exceptions(
            param,
            {
                [&]() { param.as_int(); },
                [&]() { param.get_value<miru::params::ParameterType::PARAMETER_INTEGER>(); },
                [&]() { param.get_value<int>(); }
            },
            miru::params::ParameterType::PARAMETER_INTEGER
        );
    }

    // double
    if (std::find(exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_DOUBLE) == exclude.end()) {
        test_conversion_funcs_throw_type_exceptions(
            param,
            {
                [&]() { param.as_double(); },
                [&]() { param.get_value<miru::params::ParameterType::PARAMETER_DOUBLE>(); },
                [&]() { param.get_value<double>(); }
            },
            miru::params::ParameterType::PARAMETER_DOUBLE
        );
    }

    // string
    if (std::find(exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_STRING) == exclude.end()) {
        test_conversion_funcs_throw_type_exceptions(
            param,
            {
                [&]() { param.as_string(); },
                [&]() { param.get_value<miru::params::ParameterType::PARAMETER_STRING>(); },
                [&]() { param.get_value<std::string>(); }
            },
            miru::params::ParameterType::PARAMETER_STRING
        );
    }

    // bool array
    if (std::find(exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_BOOL_ARRAY) == exclude.end()) {
        test_conversion_funcs_throw_type_exceptions(
            param,
            {
                [&]() { param.as_bool_array(); },
                [&]() { param.get_value<miru::params::ParameterType::PARAMETER_BOOL_ARRAY>(); },
                [&]() { param.get_value<std::vector<bool>>(); }
            },
            miru::params::ParameterType::PARAMETER_BOOL_ARRAY
        );
    }

    // integer array
    if (std::find(exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_INTEGER_ARRAY) == exclude.end()) {
        test_conversion_funcs_throw_type_exceptions(
            param,
            {
                [&]() { param.as_integer_array(); },
                [&]() { param.get_value<miru::params::ParameterType::PARAMETER_INTEGER_ARRAY>(); },
                [&]() { param.get_value<std::vector<int>>(); }
            },
            miru::params::ParameterType::PARAMETER_INTEGER_ARRAY
        );
    }

    // double array
    if (std::find(exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY) == exclude.end()) {
        test_conversion_funcs_throw_type_exceptions(
            param,
            {
                [&]() { param.as_double_array(); },
                [&]() { param.get_value<miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY>(); },
                [&]() { param.get_value<std::vector<double>>(); }
            },
            miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY
        );
    }

    // string array
    if (std::find(exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_STRING_ARRAY) == exclude.end()) {
        test_conversion_funcs_throw_type_exceptions(
            param,
            {
                [&]() { param.as_string_array(); },
                [&]() { param.get_value<miru::params::ParameterType::PARAMETER_STRING_ARRAY>(); },
                [&]() { param.get_value<std::vector<std::string>>(); }
            },
            miru::params::ParameterType::PARAMETER_STRING_ARRAY
        );
    }

    // null
    if (std::find(exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_NULL) == exclude.end()) {
        test_conversion_funcs_throw_type_exceptions(
            param,
            {
                [&]() { param.as_null(); },
                [&]() { param.get_value<miru::params::ParameterType::PARAMETER_NULL>(); }
            },
            miru::params::ParameterType::PARAMETER_NULL
        );
    }

    // scalar
    if (std::find(exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_SCALAR) == exclude.end()) {
        test_conversion_funcs_throw_type_exceptions(
            param,
            {
                [&]() { param.as_scalar(); },
                [&]() { param.get_value<miru::params::ParameterType::PARAMETER_SCALAR>(); },
                [&]() { param.get_value<miru::params::Scalar>(); }
            },
            miru::params::ParameterType::PARAMETER_SCALAR
        );
    }

    // scalar array
    if (std::find(exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_SCALAR_ARRAY) == exclude.end()) {
        test_conversion_funcs_throw_type_exceptions(
            param,
            {
                [&]() { param.as_scalar_array(); },
                [&]() { param.get_value<miru::params::ParameterType::PARAMETER_SCALAR_ARRAY>(); },
                [&]() { param.get_value<std::vector<miru::params::Scalar>>(); }
            },
            miru::params::ParameterType::PARAMETER_SCALAR_ARRAY
        );
    }
    // nested array
    if (std::find(exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_NESTED_ARRAY) == exclude.end()) {
        test_conversion_funcs_throw_type_exceptions(
            param,
            {
                [&]() { param.as_nested_array(); },
                [&]() { param.get_value<miru::params::ParameterType::PARAMETER_NESTED_ARRAY>(); },
                [&]() { param.get_value<miru::params::NestedArray>(); }
            },
            miru::params::ParameterType::PARAMETER_NESTED_ARRAY
        );
    }
    // object
    if (std::find(exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_OBJECT) == exclude.end()) {
        test_conversion_funcs_throw_type_exceptions(
            param,
            {
                [&]() { param.as_object(); },
                [&]() { param.get_value<miru::params::ParameterType::PARAMETER_OBJECT>(); },
                [&]() { param.get_value<miru::params::Object>(); }
            },
            miru::params::ParameterType::PARAMETER_OBJECT
        );
    }
    // object array
    if (std::find(exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_OBJECT_ARRAY) == exclude.end()) {
        test_conversion_funcs_throw_type_exceptions(
            param,
            {
                [&]() { param.as_object_array(); },
                [&]() { param.get_value<miru::params::ParameterType::PARAMETER_OBJECT_ARRAY>(); },
                [&]() { param.get_value<miru::params::ObjectArray>(); }
            },
            miru::params::ParameterType::PARAMETER_OBJECT_ARRAY
        );
    }
}

class ParameterInvalidTypeConversions : public ::testing::Test {
protected:
};

TEST_F(ParameterInvalidTypeConversions, not_set_variant) {
    miru::params::Parameter not_set_variant = miru::params::Parameter();
    test_conversion_type_exceptions(not_set_variant, {});
}

TEST_F(ParameterInvalidTypeConversions, bool_variant) {
    miru::params::Parameter bool_variant = miru::params::Parameter(
        "bool_param", true
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_BOOL,
    };
    test_conversion_type_exceptions(bool_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, int_variant) {
    miru::params::Parameter int_variant = miru::params::Parameter(
        "int_param", 4
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_INTEGER,
    };
    test_conversion_type_exceptions(int_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, double_variant) {
    miru::params::Parameter double_variant = miru::params::Parameter(
        "double_param", 4.0
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_DOUBLE,
    };
    test_conversion_type_exceptions(double_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, string_variant) {
    miru::params::Parameter string_variant = miru::params::Parameter(
        "string_param", "test"
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_STRING,
    };
    test_conversion_type_exceptions(string_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, bool_array_variant) {
    miru::params::Parameter bool_array_variant = miru::params::Parameter(
        "bool_array_param", std::vector<bool>{true, false, true}
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_BOOL_ARRAY,
    };
    test_conversion_type_exceptions(bool_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, int_array_variant) {
    miru::params::Parameter int_array_variant = miru::params::Parameter(
        "int_array_param", std::vector<int>{1, 2, 3}
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_INTEGER_ARRAY,
    };
    test_conversion_type_exceptions(int_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, double_array_variant) {
    miru::params::Parameter double_array_variant = miru::params::Parameter(
        "double_array_param", std::vector<double>{1.0, 2.0, 3.0}
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY,
    };
    test_conversion_type_exceptions(double_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, string_array_variant) {
    miru::params::Parameter string_array_variant = miru::params::Parameter(
        "string_array_param", std::vector<std::string>{"test1", "test2", "test3"}
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_STRING_ARRAY,
    };
    test_conversion_type_exceptions(string_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, null_variant) {
    miru::params::Parameter null_variant = miru::params::Parameter(
        "null_param", nullptr
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_NULL,
    };
    test_conversion_type_exceptions(null_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_bool_variant) {
    miru::params::Parameter scalar_variant = miru::params::Parameter(
        "scalar_param", miru::params::Scalar("true")
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_SCALAR,
        miru::params::ParameterType::PARAMETER_STRING,
        miru::params::ParameterType::PARAMETER_BOOL,
    };
    test_conversion_type_exceptions(scalar_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_int_variant) {
    miru::params::Parameter scalar_variant = miru::params::Parameter(
        "scalar_param", miru::params::Scalar("4")
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_SCALAR,
        miru::params::ParameterType::PARAMETER_STRING,
        miru::params::ParameterType::PARAMETER_INTEGER,
        miru::params::ParameterType::PARAMETER_DOUBLE,
    };
    test_conversion_type_exceptions(scalar_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_double_variant) {
    miru::params::Parameter scalar_variant = miru::params::Parameter(
        "scalar_param", miru::params::Scalar("1.1")
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_SCALAR,
        miru::params::ParameterType::PARAMETER_STRING,
        miru::params::ParameterType::PARAMETER_DOUBLE,
    };
    test_conversion_type_exceptions(scalar_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_string_variant) {
    miru::params::Parameter scalar_variant = miru::params::Parameter(
        "scalar_param", miru::params::Scalar("test")
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_SCALAR,
        miru::params::ParameterType::PARAMETER_STRING,
    };
    test_conversion_type_exceptions(scalar_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_array_boolean_variant) {
    miru::params::Parameter scalar_array_variant = miru::params::Parameter(
        "scalar_array_param", std::vector<miru::params::Scalar>{miru::params::Scalar("true"), miru::params::Scalar("false"), miru::params::Scalar("true")}
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_SCALAR_ARRAY,
        miru::params::ParameterType::PARAMETER_BOOL_ARRAY,
        miru::params::ParameterType::PARAMETER_STRING_ARRAY,
    };
    test_conversion_type_exceptions(scalar_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_array_integer_variant) {
    miru::params::Parameter scalar_array_variant = miru::params::Parameter(
        "scalar_array_param", std::vector<miru::params::Scalar>{miru::params::Scalar("1"), miru::params::Scalar("2"), miru::params::Scalar("3")}
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_SCALAR_ARRAY,
        miru::params::ParameterType::PARAMETER_INTEGER_ARRAY,
        miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY,
        miru::params::ParameterType::PARAMETER_STRING_ARRAY,
    };
    test_conversion_type_exceptions(scalar_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_array_double_variant) {
    miru::params::Parameter scalar_array_variant = miru::params::Parameter(
        "scalar_array_param", std::vector<miru::params::Scalar>{miru::params::Scalar("1.1"), miru::params::Scalar("2.2"), miru::params::Scalar("3.3")}
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_SCALAR_ARRAY,
        miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY,
        miru::params::ParameterType::PARAMETER_STRING_ARRAY,
    };
    test_conversion_type_exceptions(scalar_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_array_string_variant) {
    miru::params::Parameter scalar_array_variant = miru::params::Parameter(
        "scalar_array_param", std::vector<miru::params::Scalar>{miru::params::Scalar("test1"), miru::params::Scalar("test2"), miru::params::Scalar("test3")}
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_SCALAR_ARRAY,
        miru::params::ParameterType::PARAMETER_STRING_ARRAY,
    };
    test_conversion_type_exceptions(scalar_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, nested_array_variant) {
    miru::params::Parameter nested_array_variant = miru::params::Parameter(
        "nested_array_param", miru::params::NestedArray{
            std::vector<miru::params::Parameter>{
                miru::params::Parameter("nested_param1", miru::params::Scalar("test1")),
                miru::params::Parameter("nested_param2", miru::params::Scalar("test2"))
            }
        }
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_NESTED_ARRAY,
    };
    test_conversion_type_exceptions(nested_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, object_variant) {
    miru::params::Parameter object_variant = miru::params::Parameter(
        "object_param", miru::params::Object{
            std::vector<miru::params::Parameter>{
                miru::params::Parameter("object_param1", miru::params::Scalar("test1")),
                miru::params::Parameter("object_param2", miru::params::Scalar("test2"))
            }
        }
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_OBJECT,
    };
    test_conversion_type_exceptions(object_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, object_array_variant) {
    miru::params::Parameter object_array_variant = miru::params::Parameter(
        "object_array_param", miru::params::ObjectArray{
            std::vector<miru::params::Parameter>{
                miru::params::Parameter("object_param1", miru::params::Scalar("test1")),
                miru::params::Parameter("object_param2", miru::params::Scalar("test2"))
            }
        }
    );
    std::vector<miru::params::ParameterType> exclude = {
        miru::params::ParameterType::PARAMETER_OBJECT_ARRAY,
    };
    test_conversion_type_exceptions(object_array_variant, exclude);
}
