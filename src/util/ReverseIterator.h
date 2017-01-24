#pragma once


template <typename T>
class ReverseIterator
{
	T &x;

	public:
		ReverseIterator(T &x) : x(x) {}
		auto begin() const -> decltype(this->x.rbegin()){
			return x.rbegin();
		}

		auto end() const -> decltype(this->x.rend()){
			return x.rend();
		}
};

template <typename T>
ReverseIterator<T> reverse(T &x)
{
    return ReverseIterator<T>(x);
}
