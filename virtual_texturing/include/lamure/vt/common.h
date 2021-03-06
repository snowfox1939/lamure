// Copyright (c) 2014-2018 Bauhaus-Universitaet Weimar
// This Software is distributed under the Modified BSD License, see license.txt.
//
// Virtual Reality and Visualization Research Group
// Faculty of Media, Bauhaus-Universitaet Weimar
// http://www.uni-weimar.de/medien/vr

#ifndef VT_COMMON_H
#define VT_COMMON_H

#include <cstddef>
#include <cstdint>
#include <lamure/config.h>
#include <map>
#include <set>
#include <condition_variable>
#include <thread>
#include <vector>
#include <atomic>
#include <fstream>
#include <unordered_map>
#include <mutex>

// #define RASTERIZATION_COUNT

namespace vt
{
typedef uint64_t id_type;
typedef std::set<id_type> cut_type;

struct mem_slot_type
{
    size_t position = SIZE_MAX;
    id_type tile_id = UINT64_MAX;
    uint8_t* pointer = nullptr;
    bool locked = false;
    bool updated = false;
};

typedef std::vector<mem_slot_type> mem_slots_type;
typedef std::map<id_type, size_t> mem_slots_index_type;

class ContextFeedback;
typedef std::map<uint16_t, ContextFeedback*> context_feedback_map_type;
class CutDecision;
typedef std::map<uint64_t, CutDecision*> cut_decision_map_type;

typedef std::set<id_type> id_set_type;
typedef std::pair<id_type, float> prioritized_tile;
typedef std::pair<uint64_t, prioritized_tile> prioritized_tile_from_cut;
typedef std::set<prioritized_tile> prioritized_tile_set_type;

typedef std::map<uint32_t, const std::string> dataset_map_type;
typedef std::pair<uint32_t, const std::string> dataset_map_entry_type;

typedef std::set<uint16_t> view_set_type;
typedef std::set<uint16_t> context_set_type;

class Cut;
typedef std::map<uint64_t, Cut*> cut_map_type;
typedef std::pair<uint64_t, Cut*> cut_map_entry_type;
} // namespace vt

#endif // VT_COMMON_H
