#pragma once

#include <interactive/filter.hpp>

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <span>
#include <vector>

#include <nlohmann/json.hpp>

template <typename T> class CollectionBase {
protected:
  CollectionBase() = default;

public:
  virtual void initialize() = 0;
  virtual std::vector<T> getAll() const = 0;
  virtual bool addNew(const T &item) = 0;
  virtual bool remove(const T &item) = 0;
  virtual bool edit(const T &item) = 0;
  virtual bool has(const T &item) const = 0;

  std::vector<T> fetchMatching(std::span<const Filter<T>> filters) const {
    std::vector<T> all = getAll();

    if (filters.empty())
      return all;

    std::vector<T> matching;

    auto matchesAllFilters = [&filters](const T &item) -> bool {
      for (const auto &filter : filters) {
        if (!filter.passes(item))
          return false;
      }
      return true;
    };

    std::copy_if(
        all.begin(), all.end(), std::back_inserter(matching), matchesAllFilters
    );

    return matching;
  }
};

template <typename T> using Collection = std::shared_ptr<CollectionBase<T>>;

template <typename T> class JsonCollectionBase : public CollectionBase<T> {
public:
  JsonCollectionBase(const std::filesystem::path &jsonFilePath)
      : mItems(),
        mJsonFilePath(jsonFilePath) {}

  void initialize() override {
    std::ifstream file(mJsonFilePath);

    assert(file.is_open());

    nlohmann::json jsonItems;

    file >> jsonItems;

    file.close();

    mItems.clear();

    for (const auto &jsonItem : jsonItems) {
      mItems.emplace_back(T::fromJson(jsonItem));
    }
  }

  std::vector<T> getAll() const {
    return mItems;
  }

  bool addNew(const T &item) {
    if (has(item))
      return false;
    mItems.emplace_back(item);
    return true;
  }

  bool remove(const T &item) {
    if (!has(item))
      return false;
    mItems.erase(
        std::remove_if(
            mItems.begin(), mItems.end(),
            [&](const T &x) { return x.getId() == item.getId(); }
        ),
        mItems.end()
    );
    return true;
  }

  bool edit(const T &item) {
    if (!remove(item))
      return false;
    return addNew(item);
  }

  bool has(const T &item) const {
    return std::find_if(mItems.begin(), mItems.end(), [&, this](const T &x) {
             return x.getId() == item.getId();
           }) != mItems.end();
  }

private:
  std::vector<T> mItems;

  std::filesystem::path mJsonFilePath;
};

template <typename T>
Collection<T> JsonCollection(const std::filesystem::path &jsonFilePath) {
  return std::make_shared<JsonCollectionBase<T>>(jsonFilePath);
}
