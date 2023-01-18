#include<iostream>
#include<vector>
#include "vector/Vector.h"

using namespace std;

class DynClass
{
private:
	int* x;
public:
	DynClass(int val = 0) noexcept
	{
		std::cout << "Constructor 1 arg" << endl;
		x = new int{ val };
	}

	DynClass(const DynClass& rhs) noexcept
	{
		std::cout << "Copy Constructor" << endl;
		x = new int{ *rhs.x };
	}

	DynClass& operator=(const DynClass& rhs) noexcept
	{
		if (this != &rhs)
		{
			cout << "Copy assignment operator" << endl;
			x = new int{ *rhs.x };
		}

		return *this;
	}

	DynClass(DynClass&& rhs) noexcept
	{
		cout << "Move constructor" << endl;
		x = rhs.x;
		rhs.x = nullptr;
	}

	DynClass& operator=(DynClass&& rhs) noexcept
	{
		cout << "Move assignment operator" << endl;
		if (this != &rhs)
		{
			x = rhs.x;
			rhs.x = nullptr;
		}

		return *this;
	}

	~DynClass()
	{
		std::cout << "Destructor" << std::endl;
		delete x;
	}

	friend std::ostream& operator<<(std::ostream& os, const DynClass& dyn) noexcept
	{
		os << *dyn.x;
		return os;
	}

	bool operator==(const DynClass& rhs)
	{
		return x == rhs.x;
	}
};

int main()
{
	Vector<DynClass> V1;
	Vector<DynClass> V2(10);
	Vector<DynClass> V3(10, 2);
	Vector<DynClass> V4({ 1, 2, 3, 4, 5, 6, 7, 8 });
	Vector<DynClass> V5(V2.cbegin() + 3, V2.cend() - 4);
	Vector<DynClass> V6(V1);
	Vector<DynClass> V7 = std::move(V4);
	vector<DynClass> V8D({ 1, 2, 3, 4 });
	Vector<DynClass> V8(V8D.begin(), V8D.end());
	V1.push_back(3);
	V2.pop_back();
	V3.insert(V3.begin(), 3);
	V5.clear();
	V6 = std::move(V8);
	V7 = V8;
	V6.emplace(V6.end(), 1);
	V6.erase(V6.begin(), V6.end());
	V4.emplace_back(3);
	V5.fill(1);
	V8.reserve(90);
	V8.shrink_to_fit();
	V3.swap(V8);
	V2.resize(1, 2);
	V5.assign(V8D.begin(), V8D.end());

	cout << endl << "V1: " << endl;
	for (auto& val : V1) cout << val;
	cout << endl << "V2:" << endl;
	for (auto& val : V2) cout << val;
	cout << endl << "V3:" << endl;
	for (auto& val : V3) cout << val;
	cout << endl << "V4:" << endl;
	for (auto& val : V4) cout << val;
	cout << endl << "V5:" << endl;
	for (auto& val : V5) cout << val;
	cout << endl << "V6:" << endl;
	for (auto& val : V6) cout << val;
	cout << endl << "V7:" << endl;
	for (auto& val : V7) cout << val;
	cout << endl << "V8:" << endl;
	for (auto& val : V8) cout << val;
	cout << endl;
}