#include <vector>

#include "gtest/gtest.h"

#include "parallel_hashmap/phmap_dump.h"

namespace phmap {
namespace container_internal {
namespace {

using ::phmap::flat_hash_set;
using ::phmap::flat_hash_map;
using ::phmap::parallel_flat_hash_map;
using ::phmap::BinaryOutputArchive;
using ::phmap::BinaryInputArchive;
using ::phmap::MmapOutputArchive;
using ::phmap::MmapInputArchive;
using ::phmap::MmapAllocator;

TEST(DumpLoad, FlatHashSet_uin32) {
    flat_hash_set<uint32_t> st1;
    BinaryOutputArchive ar_out("./dump.data");

    st1.insert(1991);
    st1.insert(1202);

    EXPECT_TRUE(st1.dump(ar_out));
    flat_hash_set<uint32_t> st2;
    BinaryInputArchive ar_in("./dump.data");

    EXPECT_TRUE(st2.load(ar_in));

    EXPECT_EQ(2, st2.size());
    EXPECT_TRUE(st2.count(1991));
    EXPECT_TRUE(st2.count(1202));
}

TEST(DumpLoad, FlatHashMap_uint64_uint32) {
    flat_hash_map<uint64_t, uint32_t> mp1;
    BinaryOutputArchive ar_out("./dump.data");

    mp1[78731] = 99;
    mp1[13141] = 299;
    mp1[2651] = 101;

    EXPECT_TRUE(mp1.dump(ar_out));
    flat_hash_map<uint64_t, uint32_t> mp2;
    BinaryInputArchive ar_in("./dump.data");

    EXPECT_TRUE(mp2.load(ar_in));

    EXPECT_EQ(3, mp2.size());
    EXPECT_TRUE(mp2.count(78731));
    EXPECT_TRUE(mp2.count(13141));
    EXPECT_EQ(99, mp2.at(78731));
    EXPECT_EQ(101, mp2.at(2651));
}

TEST(DumpLoad, ParallelFlatHashMap_uint64_uint32) {
    parallel_flat_hash_map<uint64_t, uint32_t> mp1;
    BinaryOutputArchive ar_out("./dump.data");

    mp1[100] = 99;
    mp1[300] = 299;
    mp1[101] = 992;
    mp1[1300] = 2991;
    mp1[1130] = 299;
    mp1[2130] = 1299;

    EXPECT_TRUE(mp1.dump(ar_out));
    parallel_flat_hash_map<uint64_t, uint32_t> mp2;
    BinaryInputArchive ar_in("./dump.data");

    EXPECT_TRUE(mp2.load(ar_in));
    EXPECT_EQ(6, mp2.size());
    EXPECT_EQ(99, mp2[100]);
    EXPECT_EQ(299, mp2[300]);
    EXPECT_EQ(299, mp2[1130]);
    EXPECT_EQ(1299, mp2[2130]);
}

#if defined(__linux__)

TEST(MmapDumpLoad, FlatHashMap_uint64_uint32) {
    using MapType = flat_hash_map<uint64_t, uint32_t,
            phmap::container_internal::hash_default_hash<uint64_t>,
            phmap::container_internal::hash_default_eq<uint64_t>,
            phmap::MmapAllocator<
                        phmap::container_internal::Pair<const uint64_t, uint32_t>>>;
    
    MapType mp1;
    MmapOutputArchive ar_out("./dump.data");

    mp1[78731] = 99;
    mp1[13141] = 299;
    mp1[2651] = 101;

    EXPECT_TRUE(mp1.mmap_dump(ar_out));
    MapType mp2;
    MmapInputArchive ar_in("./dump.data");

    EXPECT_TRUE(mp2.mmap_load(ar_in));

    EXPECT_EQ(3, mp2.size());
    EXPECT_TRUE(mp2.count(78731));
    EXPECT_TRUE(mp2.count(13141));
    EXPECT_EQ(99, mp2.at(78731));
    EXPECT_EQ(101, mp2.at(2651));
}

TEST(MmapDumpLoad, ParallelFlatHashMap_uint64_uint32) {
    using MapType = parallel_flat_hash_map<uint64_t, uint32_t,
            phmap::container_internal::hash_default_hash<uint64_t>,
            phmap::container_internal::hash_default_eq<uint64_t>,
            phmap::MmapAllocator<
                        phmap::container_internal::Pair<const uint64_t, uint32_t>>,
            4,
            phmap::NullMutex>;
    
    MapType mp1;
    MmapOutputArchive ar_out("./dump.data");

    mp1[100] = 99;
    mp1[300] = 299;
    mp1[101] = 992;
    mp1[1300] = 2991;
    mp1[1130] = 299;
    mp1[2130] = 1299;

    EXPECT_TRUE(mp1.mmap_dump(ar_out));
    MapType mp2;
    MmapInputArchive ar_in("./dump.data");

    EXPECT_TRUE(mp2.mmap_load(ar_in));
    EXPECT_EQ(6, mp2.size());
    EXPECT_EQ(99, mp2[100]);
    EXPECT_EQ(299, mp2[300]);
    EXPECT_EQ(299, mp2[1130]);
    EXPECT_EQ(1299, mp2[2130]);
}
#endif

}
}
}

