#include "PCH.h"

#include "EventQueue.h"
#include <queue>

std::queue<EventQueue::Event> eventQueue;

void EventQueue::pushEvent(EventQueue::Event ev) {
	eventQueue.push(ev);
}

std::optional<EventQueue::Event> EventQueue::popEvent() {
	if (eventQueue.empty()) return std::nullopt;

	EventQueue::Event ev = eventQueue.front();
	eventQueue.pop();

	// TODO: write event to log

	return ev;
}

void EventQueue::frameBeginEvent(i32 frameNumber) {
	Event ev;
	ev.type = EventQueue::EventType::Frame;
	ev.frameEvent.frameNumber = frameNumber;
	ev.frameEvent.type = Event::FrameEvent::Ty::Begin;
	pushEvent(ev);
}

void EventQueue::frameEndEvent(i32 frameNumber) {
	Event ev;
	ev.type = EventQueue::EventType::Frame;
	ev.frameEvent.frameNumber = frameNumber;
	ev.frameEvent.type = EventQueue::Event::FrameEvent::Ty::End;
	pushEvent(ev);
}

void EventQueue::mouseMoveEvent(v2i pos) {
	Event ev;
	ev.type = EventQueue::EventType::MouseMove;
	ev.mouseMoveEvent.pos = pos;
	pushEvent(ev);
}

void EventQueue::mouseButtonDownEvent(Mouse::Button b) {
	Event ev;
	ev.type = EventQueue::EventType::MouseButton;
	ev.mouseButtonEvent.b = b;
	ev.mouseButtonEvent.type = EventQueue::ButtonEventType::Down;
	pushEvent(ev);
}

void EventQueue::mouseButtonUpEvent(Mouse::Button b) {
	Event ev;
	ev.type = EventQueue::EventType::MouseButton;
	ev.mouseButtonEvent.b = b;
	ev.mouseButtonEvent.type = EventQueue::ButtonEventType::Up;
	pushEvent(ev);
}

void EventQueue::keyDownEvent(Keyboard::Key k) {
	Event ev;
	ev.type = EventQueue::EventType::Keyboard;
	ev.keyboardEvent.k = k;
	ev.keyboardEvent.type = EventQueue::ButtonEventType::Down;
	pushEvent(ev);
}

void EventQueue::keyUpEvent(Keyboard::Key k) {
	Event ev;
	ev.type = EventQueue::EventType::Keyboard;
	ev.keyboardEvent.k = k;
	ev.keyboardEvent.type = EventQueue::ButtonEventType::Up;
	pushEvent(ev);
}
