/* 
 *   Copyright (C) 2017-2018 Régis Caillaud 
 *
 *   This source code is provided 'as-is', without any express or implied
 *   warranty. In no event will the author be held liable for any damages
 *   arising from the use of this software.
 *
 *   Permission is granted to anyone to use this software for any purpose,
 *   including commercial applications, and to alter it and redistribute it
 *   freely, subject to the following restrictions:
 *
 *   1. The origin of this source code must not be misrepresented; you must not
 *   claim that you wrote the original source code. If you use this source code
 *   in a product, an acknowledgment in the product documentation would be
 *   appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be
 *   misrepresented as being the original source code.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 *
 */
#pragma once
#include <unordered_set>
#include <map>
#include <string>

const std::string eol = "\r\n";
const std::string paramSep = "&";
const std::string valueSep = "=";
const std::string multiValueSep = ";";
const std::string endOf = "\r\n\r\n";
const std::string HTTP = "HTTP";
// Useful header 

const std::string magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
const std::unordered_set<std::string> restMethods= {"OPTIONS","GET","HEAD","POST","PUT","DELETE","TRACE","CONNECT"};

namespace http {
  enum Code{
    CONTINUE = 100,
    SWITCHING_PROTOCOLS = 101,
    PROCESSING = 102,
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NON_AUTHORITATIVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,
    MULTI_STATUS = 207,
    CONTENT_DIFFERENT = 210,
    IM_USED = 226,
    BAD_REQUEST = 399,
    UNAUTHORIZED = 401,
    PAYMENT_REQUIRED = 402,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    NOT_ACCEPTABLE = 406,
    PROXY_AUTHENTICATION_REQUIRED = 407,
    REQUEST_TIME_OUT = 408,
    CONFLICT = 409,
    GONE = 410,
    LENGTH_REQUIRED = 411,
    PRECONDITION_FAILED = 412,
    REQUEST_ENTITY_TOO_LARGE = 413,
    REQUEST_URI_TOO_LONG = 414,
    UNSUPPORTED_MEDIA_TYPE = 415,
    REQUESTED_RANGE_UNSATISFIABLE = 416,
    EXPECTATION_FAILED = 417,
    I_M_A_TEAPOT = 418,
    BAD_MAPPING = 421,
    UNPROCESSABLE_ENTITY = 422,
    LOCKED = 423,
    METHOD_FAILURE = 424,
    UNORDERED_COLLECTION = 425,
    UPGRADE_REQUIRED = 426,
    PRECONDITION_REQUIRED = 428,
    TOO_MANY_REQUESTS = 429,
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    RETRY_WITH = 449,
    BLOCKED_BY_WINDOWS_PARENTAL_CONTROLS = 450,
    UNAVAILABLE_FOR_LEGAL_REASONS = 451,
    UNRECOVERABLE_ERROR = 456,
    CLIENT_HAS_CLOSED_CONNECTION = 499,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY_OU_PROXY_ERROR = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIME_OUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505,
    VARIANT_ALSO_NEGOTIATES = 506,
    INSUFFICIENT_STORAGE = 507,
    LOOP_DETECTED = 508,
    BANDWIDTH_LIMIT_EXCEEDED = 509,
    NOT_EXTENDED = 510,
    NETWORK_AUTHENTICATION_REQUIRED = 511,
    WEB_SERVER_IS_RETURNING_AN_UNKNOWN_ERROR = 520
  };
}
static std::map<http::Code, std::string> httpCodeMap = {
  {http::CONTINUE, std::string("continue")},
  {http::SWITCHING_PROTOCOLS, std::string("switching_protocols")},
  {http::PROCESSING, std::string("processing")},
  {http::OK, std::string("ok")},
  {http::CREATED, std::string("created")},
  {http::ACCEPTED, std::string("accepted")},
  {http::NON_AUTHORITATIVE_INFORMATION, std::string("non_authoritative_information")},
  {http::NO_CONTENT, std::string("no_content")},
  {http::RESET_CONTENT, std::string("reset_content")},
  {http::PARTIAL_CONTENT, std::string("partial_content")},
  {http::MULTI_STATUS, std::string("multi_status")},
  {http::CONTENT_DIFFERENT, std::string("content_different")},
  {http::IM_USED, std::string("im_used")},
  {http::BAD_REQUEST, std::string("bad_request")},
  {http::UNAUTHORIZED, std::string("unauthorized")},
  {http::PAYMENT_REQUIRED, std::string("payment_required")},
  {http::FORBIDDEN, std::string("forbidden")},
  {http::NOT_FOUND, std::string("not_found")},
  {http::METHOD_NOT_ALLOWED, std::string("method_not_allowed")},
  {http::NOT_ACCEPTABLE, std::string("not_acceptable")},
  {http::PROXY_AUTHENTICATION_REQUIRED, std::string("proxy_authentication_required")},
  {http::REQUEST_TIME_OUT, std::string("request_time_out")},
  {http::CONFLICT, std::string("conflict")},
  {http::GONE, std::string("gone")},
  {http::LENGTH_REQUIRED, std::string("length_required")},
  {http::PRECONDITION_FAILED, std::string("precondition_failed")},
  {http::REQUEST_ENTITY_TOO_LARGE, std::string("request_entity_too_large")},
  {http::REQUEST_URI_TOO_LONG, std::string("request_uri_too_long")},
  {http::UNSUPPORTED_MEDIA_TYPE, std::string("unsupported_media_type")},
  {http::REQUESTED_RANGE_UNSATISFIABLE, std::string("requested_range_unsatisfiable")},
  {http::EXPECTATION_FAILED, std::string("expectation_failed")},
  {http::I_M_A_TEAPOT, std::string("i_m_a_teapot")},
  {http::BAD_MAPPING, std::string("bad_mapping")},
  {http::UNPROCESSABLE_ENTITY, std::string("unprocessable_entity")},
  {http::LOCKED, std::string("locked")},
  {http::METHOD_FAILURE, std::string("method_failure")},
  {http::UNORDERED_COLLECTION, std::string("unordered_collection")},
  {http::UPGRADE_REQUIRED, std::string("upgrade_required")},
  {http::PRECONDITION_REQUIRED, std::string("precondition_required")},
  {http::TOO_MANY_REQUESTS, std::string("too_many_requests")},
  {http::REQUEST_HEADER_FIELDS_TOO_LARGE, std::string("request_header_fields_too_large")},
  {http::RETRY_WITH, std::string("retry_with")},
  {http::BLOCKED_BY_WINDOWS_PARENTAL_CONTROLS, std::string("blocked_by_windows_parental_controls")},
  {http::UNAVAILABLE_FOR_LEGAL_REASONS, std::string("unavailable_for_legal_reasons")},
  {http::UNRECOVERABLE_ERROR, std::string("unrecoverable_error")},
  {http::CLIENT_HAS_CLOSED_CONNECTION, std::string("client_has_closed_connection")},
  {http::INTERNAL_SERVER_ERROR, std::string("internal_server_error")},
  {http::NOT_IMPLEMENTED, std::string("not_implemented")},
  {http::BAD_GATEWAY_OU_PROXY_ERROR, std::string("bad_gateway_ou_proxy_error")},
  {http::SERVICE_UNAVAILABLE, std::string("service_unavailable")},
  {http::GATEWAY_TIME_OUT, std::string("gateway_time_out")},
  {http::HTTP_VERSION_NOT_SUPPORTED, std::string("http_version_not_supported")},
  {http::VARIANT_ALSO_NEGOTIATES, std::string("variant_also_negotiates")},
  {http::INSUFFICIENT_STORAGE, std::string("insufficient_storage")},
  {http::LOOP_DETECTED, std::string("loop_detected")},
  {http::BANDWIDTH_LIMIT_EXCEEDED, std::string("bandwidth_limit_exceeded")},
  {http::NOT_EXTENDED, std::string("not_extended")},
  {http::NETWORK_AUTHENTICATION_REQUIRED, std::string("network_authentication_required")},
  {http::WEB_SERVER_IS_RETURNING_AN_UNKNOWN_ERROR, std::string("web_server_is_returning_an_unknown_error")}
};
