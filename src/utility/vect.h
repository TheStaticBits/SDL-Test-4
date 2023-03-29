#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <nlohmann/json.hpp>

/**
 * Stores a pair of variables of the given type
 * Contains methods for printing, overloading operators, and casting
 */
template<typename T>
struct Vect
{
	T x, y;

	// Constructors
	Vect() : x(0), y(0) { }
	Vect(T x, T y) : x(x), y(y) { }
	Vect(const nlohmann::json& json) : x(json[0].get<T>()), y(json[1].get<T>()) { } // Create from JSON
	Vect(const Vect<T>& other) : x(other.x), y(other.y) { } // Copy constructor

	// Utility
	SDL_Rect toRect(const Vect<T> other) const { return { x, y, other.x, other.y };  }
	void print() const { std::cout << "(" << x << ", " << y << ")" << std::endl; }

	// Casting
	template<typename U> Vect<U> cast() const { return Vect<U>(static_cast<U>(x), static_cast<U>(y)); }
	template<typename U> U xCast() const { return static_cast<U>(x); }
	template<typename U> U yCast() const { return static_cast<U>(y); }

	
	// Copy assignment operator	
	Vect<T>& operator=(const Vect<T>& other) { x = other.x; y = other.y; return *this; }

	// Operators
	Vect<T> operator+(const Vect<T>& other) const { return Vect<T>(x + other.x, y + other.y); }
	Vect<T> operator-(const Vect<T>& other) const { return Vect<T>(x - other.x, y - other.y); }
	Vect<T> operator*(const Vect<T>& other) const { return Vect<T>(x * other.x, y * other.y); }
	Vect<T> operator/(const Vect<T>& other) const { return Vect<T>(x / other.x, y / other.y); }
	
	Vect<T> operator/(T other) const { return Vect<T>(x / other, y / other); }
	Vect<T> operator+(T other) const { return Vect<T>(x + other, y + other); }
	Vect<T> operator-(T other) const { return Vect<T>(x - other, y - other); }
	Vect<T> operator*(T other) const { return Vect<T>(x * other, y * other); }
	
	// Operator set equal to
	void operator+=(const Vect<T>& other) { x += other.x; y += other.y; }
	void operator-=(const Vect<T>& other) { x -= other.x; y -= other.y; }
	void operator*=(const Vect<T>& other) { x *= other.x; y *= other.y; }
	void operator/=(const Vect<T>& other) { x /= other.x; y /= other.y; }
	
	void operator+=(T other) { x += other; y += other; }
	void operator-=(T other) { x -= other; y -= other; }
	void operator*=(T other) { x *= other; y *= other; }
	void operator/=(T other) { x /= other; y /= other; }
	
	// Booleans
	bool operator==(const Vect<T>& other) const { return x == other.x && y == other.y; }
	bool operator!=(const Vect<T>& other) const { return x != other.x || y != other.y; }
	bool operator>=(const Vect<T>& other) const { return x >= other.x && y >= other.y; }
	bool operator<=(const Vect<T>& other) const { return x <= other.x && y <= other.y; }
	
	bool operator>(const Vect<T>& other) const { return x > other.x && y > other.y; }
	bool operator<(const Vect<T>& other) const { return x < other.x&& y < other.y; }
};