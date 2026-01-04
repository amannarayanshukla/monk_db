//
// Created by aman shukla on 02/01/26.
//

#ifndef MONKDB_HASHMAP_H
#define MONKDB_HASHMAP_H

#include <string>
#include <vector>
#include <optional>

namespace monkdb {

class HashMap {
	private:
		enum State { EMPTY = -1, OCCUPIED = 1, DELETED = -2 };
		struct Entry {
            std::string key;
			std::string value;
			State state = EMPTY;
		};
		std::vector<Entry> table_;
		std::size_t size_ = 0;
		std::size_t capacity_ = 0;

		std::size_t FindSlot(const std::string &key);
		void Resize(std::size_t new_capacity);
	public:
		explicit HashMap(size_t capacity = 16);

		// Operations
        void Put(const std::string &key, const std::string &value);
		std::optional<std::string> Get(const std::string &key);
		bool Delete(const std::string &key);
};

}

#endif //MONKDB_HASHMAP_H