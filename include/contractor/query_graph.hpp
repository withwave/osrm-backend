#ifndef OSRM_CONTRACTOR_QUERY_GRAPH_HPP
#define OSRM_CONTRACTOR_QUERY_GRAPH_HPP

#include "contractor/query_edge.hpp"

#include "util/static_graph.hpp"
#include "util/typedefs.hpp"

#include <tuple>

namespace osrm
{
namespace contractor
{

namespace detail
{
template <bool UseSharedMemmory>
using QueryGraph = util::StaticGraph<typename QueryEdge::EdgeData, UseSharedMemmory>;
}

using QueryGraph = detail::QueryGraph<false>;
using QueryGraphView = detail::QueryGraph<true>;
}
}

#endif // QUERYEDGE_HPP
