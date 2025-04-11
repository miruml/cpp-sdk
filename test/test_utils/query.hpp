// std
#include <string>
#include <vector>

// internal
#include <miru/params/parameter.hpp>
#include <miru/params/parse.hpp>
#include <miru/query/filter.hpp>

namespace test::test_utils {

struct Filter {
    std::vector<std::string> param_names;
    std::vector<std::string> prefixes;
    bool leaves_only;

    static Filter from_json(const nlohmann::json& json) {
        Filter filter;
        filter.param_names = json.at("param_names").get<std::vector<std::string>>();
        filter.prefixes = json.at("prefixes").get<std::vector<std::string>>();
        filter.leaves_only = json.at("leaves_only").get<bool>();
        return filter;
    }
};

struct Result {
    std::string param_name;
    miru::params::ParameterValue value;

    static Result from_json(const nlohmann::json& json) {
        Result result;
        result.param_name = json.at("param_name").get<std::string>();
        result.value = miru::params::parse_json_node(
            result.param_name,
            json.at("value")
        ).get_parameter_value();
        return result;
    }
};

struct Query {
    Filter filter;
    std::vector<Result> results;

    static Query from_json(const nlohmann::json& json) {
        Query query;
        query.filter = Filter::from_json(json.at("filter"));
        for (const auto& result_json : json.at("results")) {
            query.results.push_back(Result::from_json(result_json));
        }
        return query;
    }
};

struct QueryTestSet {
    std::string description;
    std::string config_slug;
    miru::params::Parameter data;
    std::vector<Query> queries;

    static QueryTestSet from_json(const nlohmann::json& json) {
        QueryTestSet test;
        test.description = json.at("description").get<std::string>();
        test.config_slug = json.at("config-slug").get<std::string>();
        test.data = miru::params::parse_json_node(
            test.config_slug,
            json.at("data")
        );

        std::vector<Query> queries;
        for (const auto& query_json : json.at("queries")) {
            queries.push_back(Query::from_json(query_json));
        }
        test.queries = queries;
        return test;
    }
};



} // namespace test::test_utils