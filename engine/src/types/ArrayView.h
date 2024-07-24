#pragma once

template <typename T>
struct ArrayView {
	RefPtr<T> data;
	size_t count;
};
