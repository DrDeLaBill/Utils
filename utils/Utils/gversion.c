/* Copyright © 2024 Georgy E. All rights reserved. */

#include "gversion.h"

#include <stdio.h>


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
    int major, minor, patch;
    if (!str || !version) {
        return false;
    }
    if (strlen(str) > size) {
        return false;
    }
    char ch = 0;
    int res = sscanf(str, "%d.%d.%d%c", &major, &minor, &patch, &ch);
    if (res >= 3 && ch) {
        return false;
    }
    res = sscanf(str, "%d.%d.%d", &major, &minor, &patch);
    if (res != 3) {
        res = sscanf(str, "v%d.%d.%d", &major, &minor, &patch);
    }
    if (res != 3) {
        return false;
    }
    version->major = major;
    version->minor = minor;
    version->patch = patch;
    return true;
}

