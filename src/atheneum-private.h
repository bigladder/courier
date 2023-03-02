/* Copyright (c) 2019 Big Ladder Software LLC. All rights reserved.
 * See the LICENSE file for additional terms and conditions. */

#pragma once

#include "logging-base.h"

namespace Atheneum {

class AtheneumPrivate : public LoggingBase {
  public:
    AtheneumPrivate(LoggingCallbackFunction logging_callback_function_ = nullptr,
                    void* logging_callback_context_ = nullptr);
    int answer();
};

} // namespace Atheneum
