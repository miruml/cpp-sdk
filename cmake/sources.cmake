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
    # private
    miru/http/details/client.hpp
    miru/http/details/socket_client.hpp
    miru/http/details/socket_client.cpp
    miru/http/details/socket_session.hpp
    miru/http/details/socket_session.cpp
    miru/http/details/utils.hpp
    miru/http/details/utils.cpp

    # public
    miru/http/errors.hpp
    miru/http/errors.cpp
)

set(HTTP_MODELS_SOURCES
    # private
    miru/http/details/models/BaseConcreteConfig.h
    miru/http/details/models/BaseConcreteConfig.cpp
    miru/http/details/models/Error.h
    miru/http/details/models/Error.cpp
    miru/http/details/models/ErrorResponse.h
    miru/http/details/models/ErrorResponse.cpp
    miru/http/details/models/HashSchemaSerializedRequest.h
    miru/http/details/models/HashSchemaSerializedRequest.cpp
    miru/http/details/models/HashSerializedConfigSchemaFormat.h
    miru/http/details/models/HashSerializedConfigSchemaFormat.cpp
    miru/http/details/models/Helpers.h
    miru/http/details/models/Helpers.cpp
    miru/http/details/models/RefreshLatestConcreteConfigRequest.h
    miru/http/details/models/RefreshLatestConcreteConfigRequest.cpp
    miru/http/details/models/SchemaDigestResponse.h
    miru/http/details/models/SchemaDigestResponse.cpp
)

set(PARAMS_SOURCES
    # private
    miru/params/details/composite.hpp
    miru/params/details/composite.cpp
    miru/params/details/iterator.hpp
    miru/params/details/parameter_fwd.hpp
    miru/params/details/parse.hpp
    miru/params/details/parse.cpp
    miru/params/details/scalar.hpp
    miru/params/details/scalar.cpp
    miru/params/details/utils.hpp
    miru/params/details/utils.cpp
    miru/params/details/value.hpp
    miru/params/details/value.cpp

    # public
    miru/params/errors.hpp
    miru/params/parameter.hpp
    miru/params/parameter.cpp
    miru/params/type.hpp
    miru/params/utils.hpp
    miru/params/utils.cpp
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