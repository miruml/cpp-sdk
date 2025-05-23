/**
* Miru API
* No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
*
* The version of the OpenAPI document: 0.1.0
* 
*
* NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
* https://openapi-generator.tech
* Do not edit the class manually.
*/


#include "SchemaDigestResponse.h"
#include "Helpers.h"

namespace org::openapitools::server::model
{

nlohmann::json SchemaDigestResponse::to_json() const
{
    nlohmann::json j;
    ::org::openapitools::server::model::to_json(j, *this);
    return j;
}

SchemaDigestResponse SchemaDigestResponse::from_json(const nlohmann::json& j)
{
    SchemaDigestResponse o{};
    ::org::openapitools::server::model::from_json(j, o);
    return o;
}

void SchemaDigestResponse::validate() const
{
    std::stringstream msg;
    if (!validate(msg))
    {
        throw org::openapitools::server::helpers::ValidationException(msg.str());
    }
}

bool SchemaDigestResponse::validate(std::stringstream& msg) const
{
    return validate(msg, "");
}

bool SchemaDigestResponse::validate(std::stringstream& msg, const std::string& pathPrefix) const
{
    bool success = true;
    const std::string _pathPrefix = pathPrefix.empty() ? "SchemaDigestResponse" : pathPrefix;

        
    return success;
}

bool SchemaDigestResponse::operator==(const SchemaDigestResponse& other) const
{
    return
    
    digest == other.digest;
}

bool SchemaDigestResponse::operator!=(const SchemaDigestResponse& other) const
{
    return !(*this == other);
}

void to_json(nlohmann::json& j, const SchemaDigestResponse& o)
{
    j = nlohmann::json::object();
    j["digest"] = o.digest;
    
}

void from_json(const nlohmann::json& j, SchemaDigestResponse& o)
{
    j.at("digest").get_to(o.digest);
    
}

} // namespace org::openapitools::server::model

