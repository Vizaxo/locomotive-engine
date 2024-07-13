#pragma once

#include "core/Assert.h"

template <typename A, typename B>
class Either {
	union {
		A a;
		B b;
	};
	enum Tag : u8 {
		Fst, Snd,
	} tag;
public:
	Either(A inA) : a(inA), tag(Fst) {}
	Either(B inB) : b(inB), tag(Snd) {}
	Tag getTag() { return tag; }
	A& getFst() { ASSERT(tag == Fst, "Accessing first element of Either; second is stored."); return a; }
	B& getSnd() { ASSERT(tag == Snd, "Accessing second element of Either; first is stored."); return b; }
	~Either() {
		switch (tag) {
		case Fst: a.~A(); break;
		case Snd: b.~B(); break;
		}
	}
	A& operator*() { return getFst(); }
	operator bool () { return tag == Fst; }
};
