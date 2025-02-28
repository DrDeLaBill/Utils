/* Copyright © 2025 Georgy E. All rights reserved. */

#include <gtest/gtest.h>

#include "circle_buf_gc.h"



class UtilsFixture: public testing::Test
{
public:
    void SetUp() override {}

    void TearDown() override {}
};


// Initialize buffer with valid parameters and verify initial state
TEST(UtilsFixture, test_circle_buf_init_valid_params)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[100];
    const unsigned unit_size = 4;
    const unsigned length = 25;

    // Act
    circle_buf_gc_init(&buf, data, unit_size, length);

    // Assert
    ASSERT_TRUE(buf.m_data == data);
    ASSERT_TRUE(buf.m_unit_size == unit_size); 
    ASSERT_TRUE(buf.m_length == length);
    ASSERT_TRUE(buf.m_read_idx == 0);
    ASSERT_TRUE(buf.m_write_cnt == 0);
    ASSERT_TRUE(circle_buf_gc_empty(&buf) == true);
    ASSERT_TRUE(circle_buf_gc_full(&buf) == false);
}

// Initialize buffer with null pointer parameters
TEST(UtilsFixture, test_circle_buf_init_null_params)
{
    // Arrange
    circle_buf_gc_t buf;
    const unsigned unit_size = 4;
    const unsigned length = 25;

    // Act & Assert
    circle_buf_gc_init(NULL, NULL, unit_size, length);  // Should not crash
    
    circle_buf_gc_init(&buf, NULL, unit_size, length);
    ASSERT_TRUE(buf.m_data == NULL);
    ASSERT_TRUE(buf.m_unit_size == 0);
    ASSERT_TRUE(buf.m_length == 0);
    ASSERT_TRUE(buf.m_read_idx == 0); 
    ASSERT_TRUE(buf.m_write_cnt == 0);
}

// Verify behavior at exact buffer capacity
TEST(UtilsFixture, test_circle_buf_gc_full_at_capacity)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[100];
    const unsigned unit_size = 4;
    const unsigned length = 25;
    circle_buf_gc_init(&buf, data, unit_size, length);
    
    uint8_t sample_data[4] = {1, 2, 3, 4};

    // Act
    for (unsigned i = 0; i < length; ++i) {
        circle_buf_gc_push_back(&buf, sample_data);
    }

    // Assert
    ASSERT_TRUE(circle_buf_gc_full(&buf) == true);
    ASSERT_TRUE(circle_buf_gc_count(&buf) == length);
}

// Buffer maintains LIFO order when using push_back/pop_back
TEST(UtilsFixture, test_circle_buf_lifo_order_with_push_back_and_pop_back)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[12];
    const unsigned unit_size = 4;
    const unsigned length = 3;
    uint8_t item1[4] = {1, 2, 3, 4};
    uint8_t item2[4] = {5, 6, 7, 8};
    uint8_t item3[4] = {9, 10, 11, 12};
    uint8_t* popped_item;

    circle_buf_gc_init(&buf, data, unit_size, length);

    // Act
    circle_buf_gc_push_back(&buf, item1);
    circle_buf_gc_push_back(&buf, item2);
    circle_buf_gc_push_back(&buf, item3);

    // Assert LIFO order
    popped_item = circle_buf_gc_pop_back(&buf);
    ASSERT_TRUE(memcmp(popped_item, item3, unit_size) == 0);

    popped_item = circle_buf_gc_pop_back(&buf);
    ASSERT_TRUE(memcmp(popped_item, item2, unit_size) == 0);

    popped_item = circle_buf_gc_pop_back(&buf);
    ASSERT_TRUE(memcmp(popped_item, item1, unit_size) == 0);

    ASSERT_TRUE(circle_buf_gc_empty(&buf) == true);
}

// Verify unit size parameter is respected for all operations
TEST(UtilsFixture, verify_unit_size_respected_in_operations)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[100];
    const unsigned unit_size = 4;
    const unsigned length = 25;
    uint8_t input_data[4] = {1, 2, 3, 4};
    uint8_t* output_data;

    // Act
    circle_buf_gc_init(&buf, data, unit_size, length);
    circle_buf_gc_push_back(&buf, input_data);
    output_data = circle_buf_gc_pop_front(&buf);

    // Assert
    ASSERT_TRUE(buf.m_unit_size == unit_size);
    ASSERT_TRUE(memcmp(output_data, input_data, unit_size) == 0);
}

// Buffer maintains FIFO order when using push_back/pop_front
TEST(UtilsFixture, test_circle_buf_fifo_order)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[100];
    const unsigned unit_size = 4;
    const unsigned length = 5;
    uint8_t item1[4] = {1, 2, 3, 4};
    uint8_t item2[4] = {5, 6, 7, 8};
    uint8_t item3[4] = {9, 10, 11, 12};
    uint8_t* popped_item;

    circle_buf_gc_init(&buf, data, unit_size, length);

    // Act
    circle_buf_gc_push_back(&buf, item1);
    circle_buf_gc_push_back(&buf, item2);
    circle_buf_gc_push_back(&buf, item3);

    // Assert FIFO order
    popped_item = circle_buf_gc_pop_front(&buf);
    ASSERT_TRUE(memcmp(popped_item, item1, unit_size) == 0);

    popped_item = circle_buf_gc_pop_front(&buf);
    ASSERT_TRUE(memcmp(popped_item, item2, unit_size) == 0);

    popped_item = circle_buf_gc_pop_front(&buf);
    ASSERT_TRUE(memcmp(popped_item, item3, unit_size) == 0);

    // Ensure buffer is empty after pops
    ASSERT_TRUE(circle_buf_gc_empty(&buf) == true);
}

// Access front/back of empty buffer returns null and asserts
TEST(UtilsFixture, test_circle_buf_access_empty_buffer)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[100];
    const unsigned unit_size = 4;
    const unsigned length = 25;
    circle_buf_gc_init(&buf, data, unit_size, length);

    // Act & Assert
    ASSERT_TRUE(circle_buf_gc_front(&buf) == NULL);
    ASSERT_TRUE(circle_buf_gc_back(&buf) == NULL);
}

// Pop from empty buffer returns null and asserts
TEST(UtilsFixture, test_circle_buf_pop_from_empty)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[100];
    const unsigned unit_size = 4;
    const unsigned length = 25;
    circle_buf_gc_init(&buf, data, unit_size, length);

    // Act
    uint8_t* result_front = circle_buf_gc_pop_front(&buf);
    uint8_t* result_back = circle_buf_gc_pop_back(&buf);

    // Assert
    ASSERT_TRUE(result_front == NULL);
    ASSERT_TRUE(result_back == NULL);
}

// Push null data pointer returns without effect
TEST(UtilsFixture, test_push_back_null_data_pointer_no_effect)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[100];
    const unsigned unit_size = 4;
    const unsigned length = 25;
    circle_buf_gc_init(&buf, data, unit_size, length);
    unsigned initial_write_cnt = buf.m_write_cnt;

    // Act
    circle_buf_gc_push_back(&buf, NULL);

    // Assert
    ASSERT_TRUE(buf.m_write_cnt == initial_write_cnt);
    ASSERT_TRUE(circle_buf_gc_empty(&buf) == true);
}

// Free buffer and verify it can be reused
TEST(UtilsFixture, test_free_buffer_and_reuse)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[100];
    const unsigned unit_size = 4;
    const unsigned length = 25;
    circle_buf_gc_init(&buf, data, unit_size, length);
    
    uint8_t sample_data[4] = {1, 2, 3, 4};
    circle_buf_gc_push_back(&buf, sample_data);
    
    // Act
    circle_buf_gc_free(&buf);
    
    // Assert
    ASSERT_TRUE(buf.m_read_idx == 0);
    ASSERT_TRUE(buf.m_write_cnt == 0);
    ASSERT_TRUE(circle_buf_gc_empty(&buf) == true);
    ASSERT_TRUE(circle_buf_gc_full(&buf) == false);
    
    // Reuse the buffer
    circle_buf_gc_push_back(&buf, sample_data);
    
    // Assert reuse
    ASSERT_TRUE(circle_buf_gc_empty(&buf) == false);
    ASSERT_TRUE(circle_buf_gc_count(&buf) == 1);
}

// Mixed sequence of push/pop operations maintain data integrity
TEST(UtilsFixture, test_circle_buf_mixed_push_pop_operations)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[100];
    const unsigned unit_size = 4;
    const unsigned length = 5;
    uint8_t item1[4] = {1, 2, 3, 4};
    uint8_t item2[4] = {5, 6, 7, 8};
    uint8_t item3[4] = {9, 10, 11, 12};
    uint8_t item4[4] = {13, 14, 15, 16};
    uint8_t item5[4] = {17, 18, 19, 20};
    uint8_t item6[4] = {21, 22, 23, 24};

    circle_buf_gc_init(&buf, data, unit_size, length);

    // Act
    circle_buf_gc_push_back(&buf, item1);
    circle_buf_gc_push_back(&buf, item2);
    circle_buf_gc_push_back(&buf, item3);
    circle_buf_gc_push_back(&buf, item4);
    circle_buf_gc_push_back(&buf, item5);

    // Assert
    ASSERT_TRUE(circle_buf_gc_full(&buf) == true);
    ASSERT_TRUE(memcmp(circle_buf_gc_front(&buf), item1, unit_size) == 0);
    ASSERT_TRUE(memcmp(circle_buf_gc_back(&buf), item5, unit_size) == 0);

    // Act
    circle_buf_gc_pop_front(&buf);
    circle_buf_gc_push_back(&buf, item6);

    // Assert
    ASSERT_TRUE(circle_buf_gc_full(&buf) == true);
    ASSERT_TRUE(memcmp(circle_buf_gc_front(&buf), item2, unit_size) == 0);
    ASSERT_TRUE(memcmp(circle_buf_gc_back(&buf), item6, unit_size) == 0);

    // Act
    circle_buf_gc_pop_back(&buf);

    // Assert
    ASSERT_TRUE(circle_buf_gc_full(&buf) == false);
    ASSERT_TRUE(memcmp(circle_buf_gc_back(&buf), item5, unit_size) == 0);
}

// Pop elements from front and back of non-empty buffer
TEST(UtilsFixture, test_pop_elements_from_front_and_back_of_non_empty_buffer)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[20];
    const unsigned unit_size = 1;
    const unsigned length = 10;
    uint8_t element1 = 1;
    uint8_t element2 = 2;
    uint8_t element3 = 3;
    uint8_t* popped_element;

    circle_buf_gc_init(&buf, data, unit_size, length);

    // Act
    circle_buf_gc_push_back(&buf, &element1);
    circle_buf_gc_push_back(&buf, &element2);
    circle_buf_gc_push_back(&buf, &element3);

    // Assert
    ASSERT_TRUE(circle_buf_gc_empty(&buf) == false);
    ASSERT_TRUE(circle_buf_gc_full(&buf) == false);

    // Pop front
    popped_element = circle_buf_gc_pop_front(&buf);
    ASSERT_TRUE(*popped_element == element1);

    // Pop back
    popped_element = circle_buf_gc_pop_back(&buf);
    ASSERT_TRUE(*popped_element == element3);

    // Check remaining element
    popped_element = circle_buf_gc_front(&buf);
    ASSERT_TRUE(*popped_element == element2);
}

// Push elements to front of non-full buffer and verify correct storage
TEST(UtilsFixture, test_push_front_non_full_buffer)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[100];
    const unsigned unit_size = 4;
    const unsigned length = 25;
    uint8_t element[4] = {1, 2, 3, 4};
    uint8_t expected[4] = {1, 2, 3, 4};

    circle_buf_gc_init(&buf, data, unit_size, length);

    // Act
    circle_buf_gc_push_front(&buf, element);

    // Assert
    ASSERT_TRUE(circle_buf_gc_count(&buf) == 1);
    // ASSERT_TRUE(memcmp(buf.m_data + _ptr_index_from_read(&buf, 0), expected, unit_size) == 0);
    ASSERT_TRUE(circle_buf_gc_empty(&buf) == false);
    ASSERT_TRUE(circle_buf_gc_full(&buf) == false);
}

// Access front and back elements of non-empty buffer
TEST(UtilsFixture, test_access_front_and_back_elements_of_non_empty_buffer)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[100];
    const unsigned unit_size = 4;
    const unsigned length = 25;
    uint8_t element1[4] = {1, 2, 3, 4};
    uint8_t element2[4] = {5, 6, 7, 8};
    
    circle_buf_gc_init(&buf, data, unit_size, length);
    
    // Act
    circle_buf_gc_push_back(&buf, element1);
    circle_buf_gc_push_back(&buf, element2);
    
    uint8_t* front_element = circle_buf_gc_front(&buf);
    uint8_t* back_element = circle_buf_gc_back(&buf);
    
    // Assert
    ASSERT_TRUE(memcmp(front_element, element1, unit_size) == 0);
    ASSERT_TRUE(memcmp(back_element, element2, unit_size) == 0);
}

// Alternating push front/back operations work correctly
TEST(UtilsFixture, test_circle_buf_alternating_push_front_back)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[100];
    const unsigned unit_size = 4;
    const unsigned length = 5;
    uint8_t element1[4] = {1, 2, 3, 4};
    uint8_t element2[4] = {5, 6, 7, 8};
    uint8_t element3[4] = {9, 10, 11, 12};
    uint8_t element4[4] = {13, 14, 15, 16};
    uint8_t element5[4] = {17, 18, 19, 20};
    uint8_t* result;

    circle_buf_gc_init(&buf, data, unit_size, length);

    // Act
    circle_buf_gc_push_back(&buf, element1);
    circle_buf_gc_push_front(&buf, element2);
    circle_buf_gc_push_back(&buf, element3);
    circle_buf_gc_push_front(&buf, element4);
    circle_buf_gc_push_back(&buf, element5);

    // Assert
    result = circle_buf_gc_pop_front(&buf);
    ASSERT_TRUE(memcmp(result, element4, unit_size) == 0);

    result = circle_buf_gc_pop_front(&buf);
    ASSERT_TRUE(memcmp(result, element2, unit_size) == 0);

    result = circle_buf_gc_pop_front(&buf);
    ASSERT_TRUE(memcmp(result, element1, unit_size) == 0);

    result = circle_buf_gc_pop_front(&buf);
    ASSERT_TRUE(memcmp(result, element3, unit_size) == 0);

    result = circle_buf_gc_pop_front(&buf);
    ASSERT_TRUE(memcmp(result, element5, unit_size) == 0);

    ASSERT_TRUE(circle_buf_gc_empty(&buf) == true);
}

// Push to full buffer causes oldest element to be overwritten
TEST(UtilsFixture, test_push_to_full_buffer_overwrites_oldest_element)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[12];
    const unsigned unit_size = 4;
    const unsigned length = 3;
    uint8_t element1[4] = {1, 2, 3, 4};
    uint8_t element2[4] = {5, 6, 7, 8};
    uint8_t element3[4] = {9, 10, 11, 12};
    uint8_t new_element[4] = {13, 14, 15, 16};

    circle_buf_gc_init(&buf, data, unit_size, length);

    // Act
    circle_buf_gc_push_back(&buf, element1);
    circle_buf_gc_push_back(&buf, element2);
    circle_buf_gc_push_back(&buf, element3);
    circle_buf_gc_push_back(&buf, new_element);

    // Assert
    uint8_t* front_element = circle_buf_gc_front(&buf);
    ASSERT_TRUE(memcmp(front_element, element2, unit_size) == 0);
    ASSERT_TRUE(circle_buf_gc_count(&buf) == length);
}

// Verify circular wrapping behavior when buffer reaches boundaries
TEST(UtilsFixture, test_circle_buf_gc_circular_wrapping)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[10];
    const unsigned unit_size = 1;
    const unsigned length = 10;
    circle_buf_gc_init(&buf, data, unit_size, length);
    
    uint8_t input_data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // Act
    for (unsigned i = 0; i < length; ++i) {
        circle_buf_gc_push_back(&buf, &input_data[i]);
    }
    
    // Assert buffer is full
    ASSERT_TRUE(circle_buf_gc_full(&buf) == true);
    
    // Act - push one more element to trigger circular wrapping
    uint8_t new_data = 10;
    circle_buf_gc_push_back(&buf, &new_data);
    
    // Assert - first element should be overwritten
    uint8_t* front = circle_buf_gc_front(&buf);
    ASSERT_TRUE(*front == 1);
    
    // Assert - last element should be the new data
    uint8_t* back = circle_buf_gc_back(&buf);
    ASSERT_TRUE(*back == new_data);
}

// Check count increases/decreases correctly with push/pop operations
TEST(UtilsFixture, test_circle_buf_count_with_push_pop_operations)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[100];
    const unsigned unit_size = 4;
    const unsigned length = 25;
    uint8_t sample_data[4] = {1, 2, 3, 4};
    
    circle_buf_gc_init(&buf, data, unit_size, length);
    
    // Act & Assert
    ASSERT_TRUE(circle_buf_gc_count(&buf) == 0);
    
    circle_buf_gc_push_back(&buf, sample_data);
    ASSERT_TRUE(circle_buf_gc_count(&buf) == 1);
    
    circle_buf_gc_push_back(&buf, sample_data);
    ASSERT_TRUE(circle_buf_gc_count(&buf) == 2);
    
    circle_buf_gc_pop_front(&buf);
    ASSERT_TRUE(circle_buf_gc_count(&buf) == 1);
    
    circle_buf_gc_pop_back(&buf);
    ASSERT_TRUE(circle_buf_gc_count(&buf) == 0);
}

// Push elements to back of non-full buffer and verify correct storage
TEST(UtilsFixture, test_push_back_to_non_full_buffer)
{
    // Arrange
    circle_buf_gc_t buf;
    uint8_t data[100];
    const unsigned unit_size = 4;
    const unsigned length = 25;
    uint8_t element[4] = {1, 2, 3, 4};
    uint8_t expected[4] = {1, 2, 3, 4};

    circle_buf_gc_init(&buf, data, unit_size, length);

    // Act
    circle_buf_gc_push_back(&buf, element);

    // Assert
    ASSERT_TRUE(circle_buf_gc_count(&buf) == 1);
    ASSERT_TRUE(memcmp(buf.m_data, expected, unit_size) == 0);
    ASSERT_TRUE(circle_buf_gc_empty(&buf) == false);
    ASSERT_TRUE(circle_buf_gc_full(&buf) == false);
}

TEST(UtilsFixture, test_index_basic)
{
    const unsigned TEST_BUFFER_SIZE = 5;
    const unsigned TEST_UNIT_SIZE = 4;
    uint8_t buffer[TEST_BUFFER_SIZE * TEST_UNIT_SIZE];
    circle_buf_gc_t cb;

    circle_buf_gc_init(&cb, buffer, TEST_UNIT_SIZE, TEST_BUFFER_SIZE);

    // Заполнение буфера
    uint32_t data[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        circle_buf_gc_push_back(&cb, (uint8_t*)&data[i]);
    }

    // Проверка индексов
    for (int i = 0; i < 5; i++) {
        uint32_t* item = (uint32_t*)circle_buf_gc_index(&cb, i);
        ASSERT_TRUE(item && *item == data[i]);
    }
}

TEST(UtilsFixture, test_index_out_of_bounds)
{
    const unsigned TEST_BUFFER_SIZE = 5;
    const unsigned TEST_UNIT_SIZE = 4;
    uint8_t buffer[TEST_BUFFER_SIZE * TEST_UNIT_SIZE];
    circle_buf_gc_t cb;

    circle_buf_gc_init(&cb, buffer, TEST_UNIT_SIZE, TEST_BUFFER_SIZE);

    // Попытка получить элемент из пустого буфера
    ASSERT_TRUE(circle_buf_gc_index(&cb, 0) == NULL);

    // Добавление одного элемента
    uint32_t val = 100;
    circle_buf_gc_push_back(&cb, (uint8_t*)&val);

    // Проверка некорректных индексов
    ASSERT_TRUE(circle_buf_gc_index(&cb, 1) == NULL);
    ASSERT_TRUE(circle_buf_gc_index(&cb, 100) == NULL);
}

TEST(UtilsFixture, test_index_wraparound)
{
    const unsigned TEST_BUFFER_SIZE = 5;
    const unsigned TEST_UNIT_SIZE = 4;
    uint8_t buffer[TEST_BUFFER_SIZE * TEST_UNIT_SIZE];
    circle_buf_gc_t cb;

    circle_buf_gc_init(&cb, buffer, TEST_UNIT_SIZE, TEST_BUFFER_SIZE);

    // Создаем ситуацию с перемещением read_idx
    uint32_t data[TEST_BUFFER_SIZE + 2] = {1, 2, 3, 4, 5, 6, 7};
    for (int i = 0; i < TEST_BUFFER_SIZE + 2; i++) {
        circle_buf_gc_push_back(&cb, (uint8_t*)&data[i]);
    }

    // Буфер должен содержать [3,4,5,6,7]
    ASSERT_TRUE(*(uint32_t*)circle_buf_gc_index(&cb, 0) == 3);
    ASSERT_TRUE(*(uint32_t*)circle_buf_gc_index(&cb, 2) == 5);
    ASSERT_TRUE(*(uint32_t*)circle_buf_gc_index(&cb, 4) == 7);
}

TEST(UtilsFixture, test_index_after_operations) 
{
    const unsigned TEST_BUFFER_SIZE = 5;
    const unsigned TEST_UNIT_SIZE = 4;
    uint8_t buffer[TEST_BUFFER_SIZE * TEST_UNIT_SIZE];
    circle_buf_gc_t cb;

    circle_buf_gc_init(&cb, buffer, TEST_UNIT_SIZE, TEST_BUFFER_SIZE);

    // Добавляем и удаляем элементы
    uint32_t values[] = {100, 200, 300};
    for (int i = 0; i < 3; i++) {
        circle_buf_gc_push_back(&cb, (uint8_t*)&values[i]);
    }
    circle_buf_gc_pop_front(&cb);

    // Должно остаться [200, 300]
    ASSERT_TRUE(*(uint32_t*)circle_buf_gc_index(&cb, 0) == 200);
    ASSERT_TRUE(*(uint32_t*)circle_buf_gc_index(&cb, 1) == 300);
}