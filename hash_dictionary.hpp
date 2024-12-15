// hash_dictionary.hpp

#ifndef HASH_DICTIONARY_HPP
#define HASH_DICTIONARY_HPP

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <functional>

template<typename K, typename V>
class HashDictionary {
private:
    static const size_t INITIAL_SIZE = 16;
    static constexpr double LOAD_FACTOR_THRESHOLD = 0.75;

    struct Entry {
        K key;
        V value;
        bool isOccupied;
        bool isDeleted;

        Entry() : isOccupied(false), isDeleted(false) {}
    };

    std::vector<Entry> table;
    size_t elementCount;

    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % table.size();
    }

    void resize() {
        std::vector<Entry> oldTable = table;
        table = std::vector<Entry>(table.size() * 2);
        elementCount = 0;

        for (const auto& entry : oldTable) {
            if (entry.isOccupied && !entry.isDeleted) {
                insert(entry.key, entry.value);
            }
        }
    }

    size_t findPosition(const K& key) const {
        size_t pos = hash(key);
        size_t originalPos = pos;

        do {
            if (!table[pos].isOccupied ||
                (table[pos].isOccupied && !table[pos].isDeleted &&
                 table[pos].key == key)) {
                return pos;
            }
            pos = (pos + 1) % table.size();
        } while (pos != originalPos);

        throw std::runtime_error("Hash table is full");
    }

public:
    HashDictionary() : table(INITIAL_SIZE), elementCount(0) {}

    HashDictionary(const HashDictionary& other) :
        table(other.table), elementCount(other.elementCount) {}

    ~HashDictionary() {
        clear();
    }

    void clear() {
        table = std::vector<Entry>(INITIAL_SIZE);
        elementCount = 0;
    }

    size_t size() const {
        return elementCount;
    }

    void insert(const K& key, const V& value) {
        if (static_cast<double>(elementCount + 1) / table.size() >=
            LOAD_FACTOR_THRESHOLD) {
            resize();
        }

        size_t pos = findPosition(key);
        if (!table[pos].isOccupied || table[pos].isDeleted) {
            table[pos].key = key;
            table[pos].value = value;
            table[pos].isOccupied = true;
            table[pos].isDeleted = false;
            elementCount++;
        } else {
            table[pos].value = value;
        }
    }

    bool remove(const K& key) {
        size_t pos = findPosition(key);
        if (table[pos].isOccupied && !table[pos].isDeleted) {
            table[pos].isDeleted = true;
            elementCount--;
            return true;
        }
        return false;
    }

    V& operator[](const K& key) {
        size_t pos = findPosition(key);
        if (!table[pos].isOccupied || table[pos].isDeleted) {
            insert(key, V{});
            pos = findPosition(key);
        }
        return table[pos].value;
    }

    const V& operator[](const K& key) const {
        size_t pos = findPosition(key);
        if (!table[pos].isOccupied || table[pos].isDeleted) {
            throw std::out_of_range("Key not found");
        }
        return table[pos].value;
    }

    bool contains(const K& key) const {
        size_t pos = findPosition(key);
        return table[pos].isOccupied && !table[pos].isDeleted;
    }

    HashDictionary operator&&(const HashDictionary& other) const {
        HashDictionary result;
        for (const auto& entry : table) {
            if (entry.isOccupied && !entry.isDeleted) {
                if (other.contains(entry.key)) {
                    result.insert(entry.key, entry.value);
                }
            }
        }
        return result;
    }

    bool operator==(const HashDictionary& other) const {
        if (elementCount != other.elementCount) return false;

        for (const auto& entry : table) {
            if (entry.isOccupied && !entry.isDeleted) {
                if (!other.contains(entry.key)) {
                    return false;
                }
                try {
                    if (other[entry.key] != entry.value) {
                        return false;
                    }
                } catch (const std::out_of_range&) {
                    return false;
                }
            }
        }
        return true;
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Cannot open file for writing");
        }

        file.write(reinterpret_cast<const char*>(&elementCount),
                  sizeof(elementCount));

        for (const auto& entry : table) {
            if (entry.isOccupied && !entry.isDeleted) {
                size_t keySize = entry.key.size();
                file.write(reinterpret_cast<const char*>(&keySize),
                          sizeof(keySize));
                file.write(entry.key.c_str(), keySize);
                file.write(reinterpret_cast<const char*>(&entry.value),
                          sizeof(entry.value));
            }
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Cannot open file for reading");
        }

        clear();
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(count));

        for (size_t i = 0; i < count; ++i) {
            size_t keySize;
            file.read(reinterpret_cast<char*>(&keySize), sizeof(keySize));

            std::string key(keySize, '\0');
            file.read(&key[0], keySize);

            V value;
            file.read(reinterpret_cast<char*>(&value), sizeof(value));

            insert(key, value);
        }
    }

    size_t countWordsWithMinRepetitions(size_t minRepetitions) const {
        size_t count = 0;
        for (const auto& entry : table) {
            if (entry.isOccupied && !entry.isDeleted &&
                entry.value >= minRepetitions) {
                count++;
            }
        }
        return count;
    }
};

#endif
