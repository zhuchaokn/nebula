/* Copyright (c) 2019 - present, VE Software Inc. All rights reserved
 *
 * This source code is licensed under Apache 2.0 License
 *  (found in the LICENSE.Apache file in the root directory)
 */

#include <string>
#include "base/Base.h"
#include "dataman/RowReader.h"
#include "dataman/RowWriter.h"
#include "dataman/SchemaWriter.h"
#include "NebulaCodecImpl.h"

/**
 * Report error message
 */
#define PRINT_ERROR_MESSAGE(code, value) \
    if (ResultType::SUCCEEDED == code) { \
        result[field] = value; \
    } else { \
        LOG(ERROR) << "ResultType : " << static_cast<int>(code) \
                   << " Value " << value << std::endl; \
    }

namespace nebula {
namespace dataman {

std::string NebulaCodecImpl::encode(std::vector<Value> values) {
    RowWriter writer(nullptr);
    for (auto&  value : values) {
        if (value.type() == typeid(int)) {
            writer <<  boost::any_cast<int>(value);
        } else if (value.type() == typeid(std::string)) {
            writer <<  boost::any_cast<std::string>(value);
        } else if (value.type() == typeid(double)) {
            writer <<  boost::any_cast<double>(value);
        } else if (value.type() == typeid(float)) {
            writer <<  boost::any_cast<float>(value);
        } else if (value.type() == typeid(bool)) {
            writer <<  boost::any_cast<bool>(value);
        } else {
            LOG(ERROR) << "Value Type :" << value.type().name() << std::endl;
        }
    }
    std::string result = writer.encode();
    return result;
}

StatusOr<std::unordered_map<std::string, Value>>
NebulaCodecImpl::decode(std::string encoded,
                        std::vector<std::pair<std::string, cpp2::SupportedType>> fields) {
    if (encoded.empty()) {
        return Status::Error("encoded string is empty");
    }
    if (fields.size() == 0) {
        return Status::Error("fields is not set");
    }

    auto schema = std::make_shared<SchemaWriter>();
    for (auto iter = fields.begin(); iter != fields.end(); iter++) {
        schema->appendCol(iter->first, iter->second);
    }

    folly::StringPiece piece;
    ResultType code;
    auto reader = RowReader::getRowReader(encoded, schema);
    std::unordered_map<std::string, Value> result;
    for (auto iter = fields.begin(); iter != fields.end(); iter++) {
        auto field = iter->first;
        switch (iter->second) {
            case cpp2::SupportedType::BOOL:
                bool b;
                code = reader->getBool(field, b);
                PRINT_ERROR_MESSAGE(code, b);
                break;
            case cpp2::SupportedType::INT:
                int32_t i;
                code = reader->getInt(field, i);
                PRINT_ERROR_MESSAGE(code, i);
                break;
            case cpp2::SupportedType::STRING:
                code = reader->getString(field, piece);
                PRINT_ERROR_MESSAGE(code, piece.toString());
                break;
            case cpp2::SupportedType::VID:
                int64_t v;
                code = reader->getVid(field, v);
                PRINT_ERROR_MESSAGE(code, v);
                break;
            case cpp2::SupportedType::FLOAT:
                float f;
                code = reader->getFloat(field, f);
                PRINT_ERROR_MESSAGE(code, f);
                break;
            case cpp2::SupportedType::DOUBLE:
                double d;
                code = reader->getDouble(field, d);
                PRINT_ERROR_MESSAGE(code, d)
                break;
            case cpp2::SupportedType::TIMESTAMP:
                // TODO(darion) Support TIMESTAMP
                break;
            case cpp2::SupportedType::YEAR:
                // TODO(darion) Support YEAR
                break;
            case cpp2::SupportedType::YEARMONTH:
                // TODO(darion) Support YEARMONTH
                break;
            case cpp2::SupportedType::DATE:
                // TODO(darion) Support DATE
                break;
            case cpp2::SupportedType::DATETIME:
                // TODO(darion) Support DATETIME
                break;
            case cpp2::SupportedType::PATH:
                // TODO(darion) Support PATH
                break;
            default:
                // UNKNOWN
                break;
        }
    }
    return result;
}
}  // namespace dataman
}  // namespace nebula