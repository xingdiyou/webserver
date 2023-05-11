#include "event_handler.h"

EventHandler::~EventHandler() = default;

auto EventHandler::handleRead() -> int { return -1; };

auto EventHandler::handleWrite() -> int { return -1; };

void EventHandler::handleClose() {}
