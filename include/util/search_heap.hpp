#ifndef OSRM_UTIL_SEARCH_HEAP_HPP
#define OSRM_UTIL_SEARCH_HEAP_HPP

#include <boost/heap/d_ary_heap.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index_container.hpp>

namespace osrm
{
namespace util
{

template <typename NodeID, typename Weight, typename Data = void> struct SearchHeap
{

    using WeightType = Weight;
    using DataType = Data;

    using HeapData = std::pair<WeightType, NodeID>;
    using HeapContainer = boost::heap::d_ary_heap<HeapData,
                                                  boost::heap::arity<4>,
                                                  boost::heap::mutable_<true>,
                                                  boost::heap::compare<std::greater<HeapData>>>;
    using HeapHandle = typename HeapContainer::handle_type;

    struct NodeData
    {
        NodeID node;
        WeightType weight;
        DataType data;
        HeapHandle handle;
    };
    using DataIndex = boost::multi_index::indexed_by<
        boost::multi_index::hashed_unique<BOOST_MULTI_INDEX_MEMBER(NodeData, NodeID, node)>>;
    using DataContainer = boost::multi_index_container<NodeData, DataIndex>;

    DataContainer data;
    HeapContainer heap;

    SearchHeap() {}

    void Clear()
    {
        heap.clear();
        data.clear();
    }

    std::size_t Size() const { return heap.size(); }

    bool Empty() const { return heap.empty(); }

    void Insert(NodeID node, WeightType weight, DataType node_data)
    {
        auto handle = heap.push(std::make_pair(weight, node));
        data.insert(NodeData{node, weight, std::move(node_data), handle});
    }

    DataType const &GetData(NodeID node) const { return data.find(node)->data; }

    WeightType const &GetKey(NodeID node) const { return data.find(node)->weight; }

    bool WasRemoved(const NodeID node) const { return data.find(node)->handle == HeapHandle(); }

    bool WasInserted(const NodeID node) const { return data.find(node) != data.end(); }

    WeightType MinKey() const
    {
        BOOST_ASSERT(!heap.empty());
        return heap.top().first;
    }

    NodeID Min() const
    {
        BOOST_ASSERT(!heap.empty());
        return heap.top().second;
    }

    NodeID DeleteMin()
    {
        BOOST_ASSERT(!heap.empty());

        auto node = heap.top().second;
        heap.pop();
        data.modify(data.find(node), [&](auto &d) { d.handle = HeapHandle(); });

        return node;
    }

    void DeleteAll()
    {
        for (auto it = data.begin(); it != data.end(); ++it)
            data.modify(it, [&](auto &d) { d.handle = HeapHandle(); });
        heap.clear();
    }

    void DecreaseKey(NodeID node, WeightType weight, DataType node_data)
    {
        auto reference = data.find(node);
        heap.increase(reference->handle, std::make_pair(weight, node));
        data.modify(reference, [&](auto &d) {
            d.weight = weight;
            d.data = node_data;
        });
    }
};
}
}

#endif // OSRM_UTIL_SEARCH_HEAP_HPP
