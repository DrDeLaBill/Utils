/* Copyright © 2025 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include "gversion.h"


// Compare two versions with different major numbers returns correct ordering
TEST(UtilsFixture, test_compare_different_major_versions)
{
    gversion_t v1 = {2, 0, 0};
    gversion_t v2 = {1, 5, 5};

    int result = gversion_compare(&v1, &v2);

    ASSERT_TRUE(result == 1);
    ASSERT_TRUE(gversion_compare(&v2, &v1) == -1);
}

// Compare two versions with same major but different minor numbers returns correct ordering
TEST(UtilsFixture, compare_versions_with_different_minor_numbers)
{
    gversion_t v1 = {1, 2, 0};
    gversion_t v2 = {1, 1, 5};

    int result = gversion_compare(&v1, &v2);

    ASSERT_TRUE(result == 1);
    ASSERT_TRUE(gversion_compare(&v2, &v1) == -1);
}

// Convert valid version object to string in format "X.Y.Z"
TEST(UtilsFixture, test_convert_version_to_string)
{
    gversion_t version = {1, 2, 3};
    char* version_str = gversion_to_string(&version);

    ASSERT_TRUE(version_str != NULL);
    ASSERT_TRUE(strcmp(version_str, "1.2.3") == 0);
}

// Compare identical versions returns 0
TEST(UtilsFixture, compare_identical_versions)
{
    gversion_t v1 = {1, 2, 3};
    gversion_t v2 = {1, 2, 3};

    int result = gversion_compare(&v1, &v2);

    ASSERT_TRUE(result == 0);
}

// Parse valid version string in format "X.Y.Z" into version object
TEST(UtilsFixture, parse_valid_version_string)
{
    gversion_t version;
    const char* version_str = "1.2.3";
    unsigned size = strlen(version_str);

    bool result = gversion_from_string(version_str, size, &version);

    ASSERT_TRUE(result == true);
    ASSERT_TRUE(version.major == 1);
    ASSERT_TRUE(version.minor == 2);
    ASSERT_TRUE(version.patch == 3);
}

// Parse valid version string with 'v' prefix (vX.Y.Z) into version object
TEST(UtilsFixture, parse_version_string_with_v_prefix)
{
    gversion_t version;
    const char* version_str = "v1.2.3";
    unsigned size = strlen(version_str);

    bool result = gversion_from_string(version_str, size, &version);

    ASSERT_TRUE(result == true);
    ASSERT_TRUE(version.major == 1);
    ASSERT_TRUE(version.minor == 2);
    ASSERT_TRUE(version.patch == 3);
}

// Parse version string longer than specified size limit
TEST(UtilsFixture, test_parse_version_string_exceeds_size_limit)
{
    gversion_t version;
    const char* version_str = "1.2.3.4.5.6.7.8.9.10";
    unsigned size_limit = 10;

    bool result = gversion_from_string(version_str, size_limit, &version);

    ASSERT_TRUE(result == false);
}

// Parse malformed version strings (missing numbers, extra dots, invalid chars)
TEST(UtilsFixture, parse_malformed_version_strings)
{
    gversion_t version;
    bool result;

    // Test with missing numbers
    result = gversion_from_string("1..3", 5, &version);
    ASSERT_TRUE(result == false);

    // Test with extra dots
    result = gversion_from_string("1.2.3.4", 7, &version);
    ASSERT_TRUE(result == false);

    // Test with invalid characters
    result = gversion_from_string("1.a.3", 5, &version);
    ASSERT_TRUE(result == false);

    // Test with completely invalid string
    result = gversion_from_string("invalid", 7, &version);
    ASSERT_TRUE(result == false);
}

// Convert version with large numbers that may overflow buffer
TEST(UtilsFixture, test_gversion_from_string_with_large_numbers)
{
    gversion_t version;
    const char* large_version_str = "999999999.999999999.999999999";
    unsigned size = 30;

    bool result = gversion_from_string(large_version_str, size, &version);

    ASSERT_TRUE(result == true);
    ASSERT_TRUE(version.major == (uint8_t)999999999);
    ASSERT_TRUE(version.minor == (uint8_t)999999999);
    ASSERT_TRUE(version.patch == (uint8_t)999999999);
}

// Parse empty version string
TEST(UtilsFixture, parse_empty_version_string)
{
    gversion_t version;
    const char* empty_str = "";
    bool result = gversion_from_string(empty_str, strlen(empty_str), &version);
    
    ASSERT_TRUE(result == false);
}

// Compare versions with all zeros
TEST(UtilsFixture, compare_versions_with_all_zeros)
{
    gversion_t v1 = {0, 0, 0};
    gversion_t v2 = {0, 0, 0};

    int result = gversion_compare(&v1, &v2);

    ASSERT_TRUE(result == 0);
}

// Parse version string with alternative formats (X.Y, vX.Y)
TEST(UtilsFixture, parse_version_string_with_alternative_formats)
{
    gversion_t version;
    const char* version_str1 = "1.2.3";
    const char* version_str2 = "v1.2.3";

    bool result1 = gversion_from_string(version_str1, strlen(version_str1), &version);
    ASSERT_TRUE(result1 == true);
    ASSERT_TRUE(version.major == 1);
    ASSERT_TRUE(version.minor == 2);
    ASSERT_TRUE(version.patch == 3);

    bool result2 = gversion_from_string(version_str2, strlen(version_str2), &version);
    ASSERT_TRUE(result2 == true);
    ASSERT_TRUE(version.major == 1);
    ASSERT_TRUE(version.minor == 2);
    ASSERT_TRUE(version.patch == 3);
}

// Pass NULL pointers to any function
TEST(UtilsFixture, test_gversion_functions_with_null_pointers)
{
    gversion_t version = {};
    char* version_str = nullptr;
    bool result = false;

    // Testgversion_compare with NULL pointers
    ASSERT_TRUE(gversion_compare(NULL, &version) == GVERSION_ERROR);
    ASSERT_TRUE(gversion_compare(&version, NULL) == GVERSION_ERROR);
    ASSERT_TRUE(gversion_compare(NULL, NULL) == GVERSION_ERROR);

    // Test gversion_to_string with NULL pointer
    version_str = gversion_to_string(NULL);
    ASSERT_TRUE(version_str == NULL);

    // Testgversion_from_string with NULL pointers
    result = gversion_from_string(NULL, 10, &version);
    ASSERT_TRUE(result == false);

    result = gversion_from_string("1.0.0", 10, NULL);
    ASSERT_TRUE(result == false);
}