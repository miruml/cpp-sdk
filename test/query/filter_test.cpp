// std
#include <execinfo.h>

// internal
#include <miru/params/composite.hpp>
#include <miru/params/parse.hpp>
#include <miru/query/query.hpp>
#include <test/test_utils/testdata.hpp>
#include <test/test_utils/utils.hpp>

// external
#include <gtest/gtest.h>
#include <boost/stacktrace.hpp>

namespace test::params {

// ========================== SEARCH PARAM FILTER BUILDER ========================== //
class SearchParamFiltersBuilderTest : public ::testing::Test {
};

TEST_F(SearchParamFiltersBuilderTest, Default) {
    miru::query::SearchParamFiltersBuilder builder;
    auto filters = builder.build();
    EXPECT_TRUE(filters.param_names.empty());
    EXPECT_TRUE(filters.prefixes.empty());
    EXPECT_TRUE(filters.leaves_only);
}

TEST_F(SearchParamFiltersBuilderTest, WithParamName) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_param_name("test");
    auto filters = builder.build();
    EXPECT_EQ(filters.param_names.size(), 1);
    EXPECT_EQ(filters.param_names[0], "test");
    EXPECT_TRUE(filters.prefixes.empty());
    EXPECT_TRUE(filters.leaves_only);
}

TEST_F(SearchParamFiltersBuilderTest, WithParamNames) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_param_names({"test", "test2"});
    auto filters = builder.build();
    EXPECT_EQ(filters.param_names.size(), 2);
    EXPECT_EQ(filters.param_names[0], "test");
    EXPECT_EQ(filters.param_names[1], "test2");
}

TEST_F(SearchParamFiltersBuilderTest, WithParamNamesEmpty) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_param_names({});
    auto filters = builder.build();
    EXPECT_TRUE(filters.param_names.empty());
}

TEST_F(SearchParamFiltersBuilderTest, WithParamNamesMultipleCalls) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_param_names({"test", "test2"});
    builder.with_param_names({"test3", "test4"});
    auto filters = builder.build();
    EXPECT_EQ(filters.param_names.size(), 4);
    EXPECT_EQ(filters.param_names[0], "test");
    EXPECT_EQ(filters.param_names[1], "test2");
    EXPECT_EQ(filters.param_names[2], "test3");
    EXPECT_EQ(filters.param_names[3], "test4");
}

TEST_F(SearchParamFiltersBuilderTest, WithPrefix) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_prefix("test");
    auto filters = builder.build();
    EXPECT_EQ(filters.prefixes.size(), 1);
    EXPECT_EQ(filters.prefixes[0], "test");
}

TEST_F(SearchParamFiltersBuilderTest, WithPrefixes) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_prefixes({"test", "test2"});
    auto filters = builder.build();
    EXPECT_EQ(filters.prefixes.size(), 2);
    EXPECT_EQ(filters.prefixes[0], "test");
}

TEST_F(SearchParamFiltersBuilderTest, WithPrefixesEmpty) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_prefixes({});
    auto filters = builder.build();
    EXPECT_TRUE(filters.prefixes.empty());
}

TEST_F(SearchParamFiltersBuilderTest, WithPrefixesMultipleCalls) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_prefixes({"test", "test2"});
    builder.with_prefixes({"test3", "test4"});
    auto filters = builder.build();
    EXPECT_EQ(filters.prefixes.size(), 4);
    EXPECT_EQ(filters.prefixes[0], "test");
    EXPECT_EQ(filters.prefixes[1], "test2");
    EXPECT_EQ(filters.prefixes[2], "test3");
    EXPECT_EQ(filters.prefixes[3], "test4");
}

TEST_F(SearchParamFiltersBuilderTest, WithLeavesOnly) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_leaves_only(true);
    auto filters = builder.build();
    EXPECT_TRUE(filters.leaves_only);
}

TEST_F(SearchParamFiltersBuilderTest, WithLeavesOnlyFalse) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_leaves_only(false);
    auto filters = builder.build();
    EXPECT_FALSE(filters.leaves_only);
}

// ============================== MATCHING OPERATIONS ============================== //
class SearchParamFiltersMatchingTest : public ::testing::Test {};

TEST_F(SearchParamFiltersMatchingTest, MatchesParamName) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_param_names({"test", "test2"});
    auto filters = builder.build();

    // matches
    EXPECT_TRUE(filters.matches_param_name("test"));
    EXPECT_TRUE(filters.matches_param_name("test2"));

    // doesn't match
    EXPECT_FALSE(filters.matches_param_name("test3"));  
    EXPECT_FALSE(filters.matches_param_name("test4"));
}

TEST_F(SearchParamFiltersMatchingTest, EmptyParamNames) {
    miru::query::SearchParamFiltersBuilder builder;
    auto filters = builder.build();

    // everything matches
    EXPECT_TRUE(filters.matches_param_name("test"));
    EXPECT_TRUE(filters.matches_param_name("test2"));
    EXPECT_TRUE(filters.matches_param_name("test3"));  
    EXPECT_TRUE(filters.matches_param_name("test4"));
}

TEST_F(SearchParamFiltersMatchingTest, MatchesPrefix) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_prefix("prefix");
    builder.with_prefix("otherprefix");
    auto filters = builder.build();
    
    // matches
    EXPECT_TRUE(filters.matches_prefix("prefix/arglebargle"));
    EXPECT_TRUE(filters.matches_prefix("prefix/hello/world"));
    EXPECT_TRUE(filters.matches_prefix("otherprefix/arglebargle"));

    // doesn't match
    EXPECT_FALSE(filters.matches_prefix("_prefix/arglebargle"));
    EXPECT_FALSE(filters.matches_prefix(" otherprefix/arglebargle"));
}

TEST_F(SearchParamFiltersMatchingTest, EmptyPrefixes) {
    miru::query::SearchParamFiltersBuilder builder;
    auto filters = builder.build();

    // everything matches
    EXPECT_TRUE(filters.matches_prefix("test"));
    EXPECT_TRUE(filters.matches_prefix("test2"));
    EXPECT_TRUE(filters.matches_prefix("test3"));
    EXPECT_TRUE(filters.matches_prefix("test4"));
}

TEST_F(SearchParamFiltersMatchingTest, MatchesLeavesOnly) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_leaves_only(true);
    auto filters = builder.build();

    // matches
    EXPECT_TRUE(filters.matches_leaves_only(miru::params::Parameter("test")));
    EXPECT_TRUE(filters.matches_leaves_only(miru::params::Parameter("test/hello")));

    // doesn't match
    miru::params::Parameter map = miru::params::Parameter("parent", miru::params::Map({
        miru::params::Parameter("parent.field", miru::params::Scalar("value"))
    }));
    EXPECT_FALSE(filters.matches_leaves_only(map));
}

TEST_F(SearchParamFiltersMatchingTest, MatchesComplex1) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_param_names({"prefix.test", "prefix.test2"});
    builder.with_leaves_only(true);
    auto filters = builder.build();

    // matches
    EXPECT_TRUE(filters.matches(
        miru::params::Parameter("prefix.test")
    ));
    EXPECT_TRUE(filters.matches(
        miru::params::Parameter("prefix.test2")
    ));

    // doesn't match
    EXPECT_FALSE(filters.matches(
        miru::params::Parameter("prefix.test.hello")
    ));
    miru::params::Parameter map = miru::params::Parameter("prefix.test", miru::params::Map({
        miru::params::Parameter("prefix.test.hello", miru::params::Scalar("value"))
    }));
    EXPECT_FALSE(filters.matches(map));
}

TEST_F(SearchParamFiltersMatchingTest, MatchesComplex2) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_prefix("prefix.test");
    builder.with_leaves_only(true);
    auto filters = builder.build();

    // matches
    EXPECT_TRUE(filters.matches(
        miru::params::Parameter("prefix.test")
    ));
    EXPECT_TRUE(filters.matches(
        miru::params::Parameter("prefix.test2")
    ));
    EXPECT_TRUE(filters.matches(
        miru::params::Parameter("prefix.test.hello")
    ));

    // doesn't match
    miru::params::Parameter map = miru::params::Parameter("prefix.test", miru::params::Map({
        miru::params::Parameter("prefix.test.hello", miru::params::Scalar("value"))
    }));
    EXPECT_FALSE(filters.matches(map));
}

TEST_F(SearchParamFiltersMatchingTest, MatchesComplex3) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_prefix("prefix.test");
    builder.with_leaves_only(false);
    auto filters = builder.build();

    // matches
    EXPECT_TRUE(filters.matches(
        miru::params::Parameter("prefix.test")
    ));
    EXPECT_TRUE(filters.matches(
        miru::params::Parameter("prefix.test2")
    ));
    EXPECT_TRUE(filters.matches(
        miru::params::Parameter("prefix.test.hello")
    ));
    miru::params::Parameter map = miru::params::Parameter("prefix.test", miru::params::Map({
        miru::params::Parameter("prefix.test.hello", miru::params::Scalar("value"))
    }));
    EXPECT_TRUE(filters.matches(map));
}

// ========================== CONTINUE SEARCH OPERATIONS ========================== //
class SearchParamFiltersContinueSearchTest : public ::testing::Test {};

TEST_F(SearchParamFiltersContinueSearchTest, ContinueSearchParamName) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_param_names({"l1.l2.l3", "l1.l2.l4"});
    auto filters = builder.build();

    // matches
    EXPECT_TRUE(filters.child_might_match_param_name("l1"));
    EXPECT_TRUE(filters.child_might_match_param_name("l1.l2"));
    EXPECT_TRUE(filters.child_might_match_param_name("l1.l2.l3"));
    EXPECT_TRUE(filters.child_might_match_param_name("l1.l2.l4"));

    // doesn't match
    EXPECT_FALSE(filters.child_might_match_param_name("l1.l3"));
    EXPECT_FALSE(filters.child_might_match_param_name("l1.l2.l3.l4"));
    EXPECT_FALSE(filters.child_might_match_param_name("l1.l2.l5"));
    EXPECT_FALSE(filters.child_might_match_param_name("l1.l5"));
    EXPECT_FALSE(filters.child_might_match_param_name("l5"));
}

TEST_F(SearchParamFiltersContinueSearchTest, ContinueSearchPrefix) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_prefixes({"l1.l2", "l1.l3"});
    auto filters = builder.build();

    // matches
    EXPECT_TRUE(filters.child_might_match_prefix("l1"));
    EXPECT_TRUE(filters.child_might_match_prefix("l1.l2"));
    EXPECT_TRUE(filters.child_might_match_prefix("l1.l3"));
    EXPECT_TRUE(filters.child_might_match_prefix("l1.l2.l3"));
    EXPECT_TRUE(filters.child_might_match_prefix("l1.l2.l4"));
    EXPECT_TRUE(filters.child_might_match_prefix("l1.l2.l5"));
    EXPECT_TRUE(filters.child_might_match_prefix("l1.l2.l3.l4"));

    // doesn't match
    EXPECT_FALSE(filters.child_might_match_prefix("l1.l5"));
    EXPECT_FALSE(filters.child_might_match_prefix("l5"));
}

TEST_F(SearchParamFiltersContinueSearchTest, ContinueSearchLeaves) {
    miru::query::SearchParamFiltersBuilder builder;
    auto filters = builder.build();

    // don't continue search if it's a leaf
    EXPECT_FALSE(filters.continue_search(miru::params::Parameter("test")));

    // continue search if it's not a leaf
    miru::params::Parameter map = miru::params::Parameter("prefix.test", miru::params::Map({
        miru::params::Parameter("prefix.test.hello", miru::params::Scalar("value"))
    }));
    EXPECT_TRUE(filters.continue_search(map));
}

TEST_F(SearchParamFiltersContinueSearchTest, ContinueSearchComplex1) {
    miru::query::SearchParamFiltersBuilder builder;
    builder.with_param_names({"prefix.test.hello.1", "prefix.test.hello.2"});
    builder.with_prefixes({"prefix.test"});
    auto filters = builder.build();

    // could have a match
    miru::params::Parameter map1 = miru::params::Parameter("prefix.test", miru::params::Map({
        miru::params::Parameter("prefix.test.doesnt_matter", miru::params::Scalar("value"))
    }));
    EXPECT_TRUE(filters.continue_search(map1));

    miru::params::Parameter map2 = miru::params::Parameter("prefix.test.hello", miru::params::Map({
        miru::params::Parameter("prefix.test.hello.doesnt_matter", miru::params::Scalar("value"))
    }));
    EXPECT_TRUE(filters.continue_search(map2));

    miru::params::Parameter map3 = miru::params::Parameter("prefix", miru::params::Map({
        miru::params::Parameter("prefix.doesnt_matter", miru::params::Scalar("value"))
    }));
    EXPECT_TRUE(filters.continue_search(map3));

    // cannot have a match
    EXPECT_FALSE(filters.continue_search(miru::params::Parameter("prefix.test.hello.2")));
    miru::params::Parameter map4 = miru::params::Parameter(" prefix", miru::params::Map({
        miru::params::Parameter(" prefix.doesnt_matter", miru::params::Scalar("value"))
    }));
    EXPECT_FALSE(filters.continue_search(map4));
}

}  // namespace test::params