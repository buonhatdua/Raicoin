#pragma once

#include <boost/filesystem.hpp>
#include <lmdb/libraries/liblmdb/lmdb.h>
#include <rai/common/errors.hpp>
#include <rai/common/numbers.hpp>

namespace rai
{
class MdbEnv
{
public:
    MdbEnv(rai::ErrorCode&, const boost::filesystem::path&, int);
    ~MdbEnv();
    operator MDB_env*() const;
    MDB_env* env_;
};

class MdbVal
{
public:
    MdbVal();
    MdbVal(const MDB_val&);
    MdbVal(size_t, void*);
    MdbVal(const rai::uint128_union&);
    MdbVal(const rai::uint256_union&);
    const uint8_t* Data() const;
    size_t Size() const;
    rai::uint256_union uint256_union() const;
    operator MDB_val*() const;
    operator const MDB_val&() const;

    MDB_val value_;
};


class MdbTransaction
{
public:
    MdbTransaction(rai::ErrorCode&, rai::MdbEnv&, MDB_txn*, bool);
    MdbTransaction(const rai::MdbTransaction&) = delete;
    ~MdbTransaction();
    rai::MdbTransaction& operator=(const rai::MdbTransaction&) = delete;
    operator MDB_txn*() const;
    void Abort();

    MDB_txn* handle_;
    rai::MdbEnv& env_;
};

class StoreIterator
{
public:
    StoreIterator(MDB_txn*, MDB_dbi);
    StoreIterator(std::nullptr_t);
    StoreIterator(MDB_txn*, MDB_dbi, const MDB_val&);
    StoreIterator(rai::StoreIterator&&);
    StoreIterator(const rai::StoreIterator&) = delete;
    ~StoreIterator();

    rai::StoreIterator& operator++();
    void NextDup();
    rai::StoreIterator& operator=(rai::StoreIterator&&);
    rai::StoreIterator& operator=(const rai::StoreIterator&) = delete;
    bool operator==(const rai::StoreIterator&) const;
    bool operator!=(const rai::StoreIterator&) const;
    std::pair<rai::MdbVal, rai::MdbVal>* operator->();
    const std::pair<rai::MdbVal, rai::MdbVal>* operator->() const;
    void Clear();

private:
    MDB_cursor* cursor_;
    std::pair<rai::MdbVal, rai::MdbVal> current_;
};
};