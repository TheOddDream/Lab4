// main.cpp

#include <gtest/gtest.h>
#include "hash_dictionary.hpp"

class HashDictionaryTest : public ::testing::Test {
protected:
    HashDictionary<std::string, int> dict;
};

TEST_F(HashDictionaryTest, DefaultConstructor) {
    EXPECT_EQ(dict.size(), 0);
}

TEST_F(HashDictionaryTest, InsertAndSize) {
    dict.insert("hello", 1);
    EXPECT_EQ(dict.size(), 1);
    dict.insert("hello", 2);
    EXPECT_EQ(dict.size(), 1);
}

TEST_F(HashDictionaryTest, RemoveAndSize) {
    dict.insert("hello", 1);
    EXPECT_EQ(dict.size(), 1);
    dict.remove("hello");
    EXPECT_EQ(dict.size(), 0);
}

TEST_F(HashDictionaryTest, ClearEquals) {
    dict.insert("hello", 1);
    dict.clear();
    HashDictionary<std::string, int> emptyDict;
    EXPECT_EQ(dict, emptyDict);
}

TEST_F(HashDictionaryTest, IntersectionWithEmpty) {
    dict.insert("hello", 1);
    HashDictionary<std::string, int> emptyDict;
    auto result = dict && emptyDict;
    EXPECT_EQ(result.size(), 0);
}

TEST_F(HashDictionaryTest, IntersectionWithSelf) {
    dict.insert("hello", 1);
    dict.insert("world", 2);
    auto result = dict && dict;
    EXPECT_EQ(result, dict);
}

TEST_F(HashDictionaryTest, CopyConstructor) {
    dict.insert("hello", 1);
    HashDictionary<std::string, int> copy(dict);
    EXPECT_EQ(copy, dict);
    copy.insert("world", 2);
    EXPECT_NE(copy, dict);
}

TEST_F(HashDictionaryTest, FileIO) {
    dict.insert("hello", 1);
    dict.insert("world", 2);
    dict.saveToFile("test.bin");

    HashDictionary<std::string, int> loaded;
    loaded.loadFromFile("test.bin");
    EXPECT_EQ(loaded, dict);
}

TEST_F(HashDictionaryTest, WordRepetitions) {
    dict.insert("hello", 3);
    dict.insert("world", 2);
    dict.insert("test", 1);

    EXPECT_EQ(dict.countWordsWithMinRepetitions(2), 2);
    EXPECT_EQ(dict.countWordsWithMinRepetitions(3), 1);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
