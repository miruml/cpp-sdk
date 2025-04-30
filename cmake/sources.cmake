set(CONFIG_SOURCES
    miru/config/config.hpp
    miru/config/config.cpp
    miru/config/errors.hpp
)

set(FILESYS_SOURCES
    # private
    miru/filesys/details/dir.hpp
    miru/filesys/details/dir.cpp
    miru/filesys/details/file.hpp
    miru/filesys/details/file.cpp
    miru/filesys/details/path.hpp
    miru/filesys/details/path.cpp

    # public
    miru/filesys/errors.hpp
)

set(HTTP_SOURCES
    miru/http/client.hpp
    miru/http/errors.hpp
    miru/http/errors.cpp
    miru/http/socket_client.hpp
    miru/http/socket_client.cpp
    miru/http/socket_session.hpp
    miru/http/socket_session.cpp
    miru/http/utils.hpp
    miru/http/utils.cpp
)

set(HTTP_MODELS_SOURCES
    miru/http/models/BaseConcreteConfig.h
    miru/http/models/BaseConcreteConfig.cpp
    miru/http/models/Error.h
    miru/http/models/Error.cpp
    miru/http/models/ErrorResponse.h
    miru/http/models/ErrorResponse.cpp
    miru/http/models/HashSchemaSerializedRequest.h
    miru/http/models/HashSchemaSerializedRequest.cpp
    miru/http/models/HashSerializedConfigSchemaFormat.h
    miru/http/models/HashSerializedConfigSchemaFormat.cpp
    miru/http/models/Helpers.h
    miru/http/models/Helpers.cpp
    miru/http/models/RefreshLatestConcreteConfigRequest.h
    miru/http/models/RefreshLatestConcreteConfigRequest.cpp
    miru/http/models/SchemaDigestResponse.h
    miru/http/models/SchemaDigestResponse.cpp
)

set(PARAMS_SOURCES
    miru/params/composite.hpp
    miru/params/composite.cpp
    miru/params/errors.hpp
    miru/params/iterator.hpp
    miru/params/parameter_fwd.hpp
    miru/params/parameter.hpp
    miru/params/parameter.cpp
    miru/params/parse.hpp
    miru/params/parse.cpp
    miru/params/scalar.hpp
    miru/params/scalar.cpp
    miru/params/type.hpp
    miru/params/utils.hpp
    miru/params/utils.cpp
    miru/params/value.hpp
    miru/params/value.cpp
)

set(QUERY_SOURCES
    miru/query/errors.hpp
    miru/query/filter.hpp
    miru/query/filter.cpp
    miru/query/query.hpp
    miru/query/query.cpp
    miru/query/ros2_fwd.hpp
    miru/query/ros2.hpp
    miru/query/ros2.cpp
)

set(CORE_SOURCES
    miru/errors.hpp
    miru/errors.cpp
    miru/utils.hpp
    miru/utils.cpp
)

set(MIRU_SOURCES
    ${CONFIG_SOURCES}
    ${FILESYS_SOURCES}
    ${HTTP_SOURCES}
    ${HTTP_MODELS_SOURCES}
    ${PARAMS_SOURCES}
    ${QUERY_SOURCES}
    ${CORE_SOURCES}
)