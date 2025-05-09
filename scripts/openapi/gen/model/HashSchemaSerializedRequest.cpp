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


#include "HashSchemaSerializedRequest.h"
#include "Helpers.h"

namespace org::openapitools::server::model
{

nlohmann::json HashSchemaSerializedRequest::to_json() const
{
    nlohmann::json j;
    ::org::openapitools::server::model::to_json(j, *this);
    return j;
}

HashSchemaSerializedRequest HashSchemaSerializedRequest::from_json(const nlohmann::json& j)
{
    HashSchemaSerializedRequest o{};
    ::org::openapitools::server::model::from_json(j, o);
    return o;
}

void HashSchemaSerializedRequest::validate() const
{
    std::stringstream msg;
    if (!validate(msg))
    {
        throw org::openapitools::server::helpers::ValidationException(msg.str());
    }
}

bool HashSchemaSerializedRequest::validate(std::stringstream& msg) const
{
    return validate(msg, "");
}

bool HashSchemaSerializedRequest::validate(std::stringstream& msg, const std::string& pathPrefix) const
{
    bool success = true;
    const std::string _pathPrefix = pathPrefix.empty() ? "HashSchemaSerializedRequest" : pathPrefix;

            
    return success;
}

bool HashSchemaSerializedRequest::operator==(const HashSchemaSerializedRequest& other) const
{
    return
    
    format == other.format && schema == other.schema;
}

bool HashSchemaSerializedRequest::operator!=(const HashSchemaSerializedRequest& other) const
{
    return !(*this == other);
}

void to_json(nlohmann::json& j, const HashSchemaSerializedRequest& o)
{
    j = nlohmann::json::object();
    j["format"] = o.format;
    j["schema"] = o.schema;
    
}

void from_json(const nlohmann::json& j, HashSchemaSerializedRequest& o)
{
    j.at("format").get_to(o.format);
    j.at("schema").get_to(o.schema);
    
}

} // namespace org::openapitools::server::model

