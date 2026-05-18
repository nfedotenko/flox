/*
 * Flox Engine
 * Developed by FLOX Foundation (https://github.com/FLOX-Foundation)
 *
 * Copyright (c) 2025 FLOX Foundation
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#pragma once

#include "flox/engine/abstract_subsystem.h"
#include "flox/execution/exchange_capabilities.h"
#include "flox/execution/order.h"

#include <vector>

namespace flox
{

struct OCOParams
{
  Order order1;
  Order order2;
};

class IOrderExecutor : public ISubsystem
{
 public:
  virtual ~IOrderExecutor() = default;

  virtual void submitOrder(const Order& order) {}
  virtual void cancelOrder(OrderId orderId) {}
  virtual void cancelAllOrders(SymbolId symbol) {}
  virtual void replaceOrder(OrderId oldOrderId, const Order& newOrder) {}

  // Batch submit. Default impl loops `submitOrder`; venue executors
  // override this to coalesce into a single batch endpoint (e.g. OKX
  // `/api/v5/trade/batch-orders`) which halves API request count for
  // market-making strategies that emit bid+ask pairs each cycle.
  virtual void submitOrders(const std::vector<Order>& orders)
  {
    for (const auto& o : orders)
    {
      submitOrder(o);
    }
  }

  // OCO: one-cancels-other
  virtual void submitOCO(const OCOParams& params) {}

  // Capability discovery
  virtual ExchangeCapabilities capabilities() const { return ExchangeCapabilities::simulated(); }
};

}  // namespace flox
