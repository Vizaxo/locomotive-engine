#pragma once

template <typename T, typename... Ts>
struct Product {
	T a;
	Product<Ts...> rest;
};

template <typename T>
struct Product<T> {
	T a;
};

template <typename T, typename... Ts> 
T& fst(Product<T, Ts...> x) { return x.a; }

template <typename T, typename U, typename... Ts> 
U& snd(Product<T, U, Ts...> x) { return x.rest.a; }

template <typename T, typename U, typename V, typename... Ts> 
V& third(Product<T, U, V, Ts...> x) { return x.rest.rest.a; }
