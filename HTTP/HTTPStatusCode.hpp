#ifndef HTTPSTATUSCODE_HPP
#define HTTPSTATUSCODE_HPP

enum HTTPStatus
{
	DEFAULT,
	// 1xx information
	CONTINUE = 100,
	SWITCHING_PROTOCOLS,
	PROCESSING,
	// 2xx success
	OK = 200,
	CREATED,
	ACCEPTED,
	NON_AUTHORITATIVE_INFORMATION,
	NO_CONTENT,
	RESET_CONTENT,
	PARTIAL_CONTENT,
	MULTI_STATUS,
	ALREADY_REPORTED,
	// 3xx redirect
	MULTIPLE_CHOICES = 300,
	MOVED_PERMANENTLY,
	FOUND,
	SEE_OTHER,
	NOT_MODIFIED,
	USE_PROXY,
	Unused,
	TEMPORARY_REDIRECT,
	PERMANENT_REDIRECT,
	// 4xx client error
	BAD_REQUEST = 400,
	UNAUTHORIZED,
	PAYMENT_REQUIRED,
	FORBIDDEN,
	NOT_FOUND,
	METHOD_NOT_ALLOWED,
	NOT_ACCEPTABLE,
	PROXY_AUTHENTICATION_REQUIRED,
	REQUEST_TIMEOUT,
	CONFLICT,
	GONE,
	LENGTH_REQUIRED,
	PRECONDITION_FAILED,
	PAYLOAD_TOO_LARGE,
	URI_TOO_LONG,
	UNSUPPORTED_MEDIA_TYPE,
	RANGE_NOT_SATISIFIABLE,
	EXPECTAION_FAILED,
	IM_A_TEAPOT,
	MISDIRECTED_REQUEST = 421,
	UNPROCESSABLE_CONTENT,
	LOCKED,
	FAILED_DEPENDENCY,
	TOO_EARLY,
	UPGRADE_REQUIRED,
	PRECONDITION_REQUIRED = 428,
	TOO_MANY_REQUREST,
	REQUEST_HEADER_FIELD_TOO_LARGE = 431,
	UNAVAILABLE_FOR_REASON = 451,
	// 5xx server error
	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPREMENTED,
	BAD_GATEWAY,
	SERVICE_UNAVAILABLE,
	HTTP_VERSION_NOT_SUPPORTED = 505,
	VARIANT_ALSO_NEGOTIATES,
	INSUFFICIENT_STORAGE,
	LOOP_DETECTED,
};

#endif /* HTTPSTATUSCODE_HPP */
