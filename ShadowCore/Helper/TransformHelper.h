#pragma once
#include "ssi.h"

template<class T>
class vectorCB {
private:
	using CallBackFunctionPtr = std::function<void(std::shared_ptr<T>)>;
	std::vector<std::shared_ptr<T>> elements;
	CallBackFunctionPtr cb_func = nullptr;
public:
	vectorCB(CallBackFunctionPtr func) : cb_func(func) {}
	void push_back(const std::shared_ptr<T> el) {
		cb_func(el);
		elements.push_back(el);
	}
};