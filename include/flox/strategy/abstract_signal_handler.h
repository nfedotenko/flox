/*
 * Flox Engine
 * Developed by FLOX Foundation (https://github.com/FLOX-Foundation)
 *
 * Copyright (c) 2025 FLOX Foundation
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#pragma once

#include "flox/strategy/signal.h"

#include <vector>

namespace flox
{

class ISignalHandler
{
 public:
  virtual ~ISignalHandler() = default;

  virtual void onSignal(const Signal& signal) = 0;

  // Default batch path: loop onSignal. Live bridges override this to
  // coalesce a tick's bid+ask quote pair into a single batch submit
  // (e.g. OKX /api/v5/trade/batch-orders), halving the API request
  // count and shaving ~one RTT off the effective quote cycle.
  virtual void onSignalBatch(const std::vector<Signal>& signals)
  {
    for (const auto& s : signals)
    {
      onSignal(s);
    }
  }
};

}  // namespace flox
