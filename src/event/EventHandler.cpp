#include "EventHandler.h"

EventHandler::EventHandler() = default;

EventHandler::~EventHandler() = default;

bool EventHandler::onEvent(Event &event) { return false; }
