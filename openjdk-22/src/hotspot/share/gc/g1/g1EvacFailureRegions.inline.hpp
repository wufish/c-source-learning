/*
 * Copyright (c) 2021, 2022, Huawei Technologies Co., Ltd. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#ifndef SHARE_GC_G1_G1EVACFAILUREREGIONS_INLINE_HPP
#define SHARE_GC_G1_G1EVACFAILUREREGIONS_INLINE_HPP

#include "gc/g1/g1CollectedHeap.inline.hpp"
#include "gc/g1/g1EvacFailureRegions.hpp"
#include "runtime/atomic.hpp"

bool G1EvacFailureRegions::record(uint region_idx) {
  bool success = _regions_failed_evacuation.par_set_bit(region_idx,
                                                        memory_order_relaxed);
  if (success) {
    size_t offset = Atomic::fetch_then_add(&_evac_failure_regions_cur_length, 1u);
    _evac_failure_regions[offset] = region_idx;

    G1CollectedHeap* g1h = G1CollectedHeap::heap();
    HeapRegion* hr = g1h->region_at(region_idx);
    hr->note_evacuation_failure();
  }
  return success;
}

#endif //SHARE_GC_G1_G1EVACFAILUREREGIONS_INLINE_HPP
