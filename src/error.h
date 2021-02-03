#pragma once

/**
 * Definition of result of an operation, either OK or an error value.
 */
enum class Result {
    /** The operation was completed successfully. */
    OK,

    /** We don't know (yet) the speaker address. */
    ERROR_NO_SPEAKER_ADDRESS,

    /** The speaker didn't reply on time, e.g. it's off or its IP address has changed. */
    ERROR_HTTP_TIMEOUT,

    /** The speaker's HTTP server replied with something different than 200 OK. */
    ERROR_HTTP_NON_OK_RESPONSE,

    /** The speaker's HTTP response couldn't be understood. */
    ERROR_PARSE_FAILED
};

/**
 * Implements a simple error propagation. To be used in a function returning Result, when
 * invoking another function returning Result. If the returned result is not OK, this macro
 * will propagate the error to the calling function.
 */
#define RETURN_IF_ERROR(x) { Result result = (x); if (result != Result::OK) return result; }