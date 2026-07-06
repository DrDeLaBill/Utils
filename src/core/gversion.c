/* Copyright © 2024 Georgy E. All rights reserved. */

#include "gversion.h"

#include <stdio.h>
#include <string.h>


int gversion_compare(const gversion_t* v1, const gversion_t* v2)
{
    if (!v1 || !v2) {
        return GVERSION_ERROR;
    }
    if (v1->major > v2->major) return GVERSION_HIGHER;
    if (v1->major < v2->major) return GVERSION_LOWER;
    if (v1->minor > v2->minor) return GVERSION_HIGHER;
    if (v1->minor < v2->minor) return GVERSION_LOWER;
    if (v1->patch > v2->patch) return GVERSION_HIGHER;
    if (v1->patch < v2->patch) return GVERSION_LOWER;
    return GVERSION_OK;
}

char* gversion_to_string(const gversion_t* version)
{
    if (!version) {
        return NULL;
    }
    static char version_str[20] = "";
    memset(version_str, 0, sizeof(version_str));
    return snprintf(version_str, sizeof(version_str) - 1, "%d.%d.%d", version->major, version->minor, version->patch) == -1 ? NULL : version_str;
}

bool gversion_from_string(const char* str, const unsigned size, gversion_t* version)
{
    if (!str || !version || strlen(str) > size) {
        return false;
    }
    const char* p = str;
    if (*p == 'v') {
        p++;
    }
    int major = 0, minor = 0, patch = 0;
    while (*p >= '0' && *p <= '9') {
        major = major * 10 + (*p++ - '0');
    }
    if (*p++ != '.') {
        return false;
    }
    if (*p < '0' || *p > '9') {
        return false;
    }
    while (*p >= '0' && *p <= '9') {
        minor = minor * 10 + (*p++ - '0');
    }
    if (*p++ != '.') {
        return false;
    }
    if (*p < '0' || *p > '9') {
        return false;
    }
    while (*p >= '0' && *p <= '9') {
        patch = patch * 10 + (*p++ - '0');
    }
    if (*p != '\0' && *p != '\0') {
        return false;
    }
    version->major = (uint8_t)major;
    version->minor = (uint8_t)minor;
    version->patch = (uint8_t)patch;
    return true;
}

