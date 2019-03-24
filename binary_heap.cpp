#include <iostream>
#include <vector>
#include <stdlib.h>
typedef unsigned int uint32_t;

template <typename T>
class BinaryHeap
{	
public:
	BinaryHeap(bool (*IsCorrectOrder)(const T&, const T&)): elementCount(0), IsCorrectOrder(IsCorrectOrder) {}
	
	bool GetTop(T& element)
	{
		if (!elementCount) {
			return false;
		}
		element = elementList[0];
		return true;
	}
	
	void Push(const T& element)
	{
		elementList.resize(elementList.size() + 1);
		++elementCount;
		size_t nodeNumber = elementCount;
		size_t parentNodeNumber = nodeNumber >> 1;
		while (parentNodeNumber) {
			if (IsCorrectOrder(elementList[parentNodeNumber - 1], element)) {
				break;
			}
			elementList[nodeNumber - 1] = elementList[parentNodeNumber - 1];
			nodeNumber = parentNodeNumber;
			parentNodeNumber = nodeNumber >> 1;
		}
		elementList[nodeNumber - 1] = element;
	}
	
	bool Pop(T& element)
	{
		if (!elementCount) {
			return false;
		}
		element = elementList[0];
		T temp = elementList[elementCount - 1];
		elementList.pop_back();
		--elementCount;
		if (!elementCount) {
			return true;
		}
		
		size_t nodeNumber = 1;
		size_t leftNodeNumber = nodeNumber << 1;
		while (leftNodeNumber <= elementCount) {
			if (leftNodeNumber == elementCount) {
				if (IsCorrectOrder(elementList[leftNodeNumber - 1], temp)) {
					elementList[nodeNumber - 1] = elementList[leftNodeNumber - 1];
					nodeNumber = leftNodeNumber;
				}
				break;
			}
			size_t rightNodeNumber = leftNodeNumber + 1;
			if (IsCorrectOrder(elementList[leftNodeNumber - 1], temp)) {
				if (IsCorrectOrder(elementList[rightNodeNumber - 1], temp)) {
					if (IsCorrectOrder(elementList[leftNodeNumber - 1], elementList[rightNodeNumber - 1])) {
						elementList[nodeNumber - 1] = elementList[leftNodeNumber - 1];
						nodeNumber = leftNodeNumber;
					}
					else {
						elementList[nodeNumber - 1] = elementList[rightNodeNumber - 1];
						nodeNumber = rightNodeNumber;
					}
				}
				else {
					elementList[nodeNumber - 1] = elementList[leftNodeNumber - 1];
					nodeNumber = leftNodeNumber;
				}
			}
			else {
				if (IsCorrectOrder(elementList[rightNodeNumber - 1], temp)) {
					elementList[nodeNumber - 1] = elementList[rightNodeNumber - 1];
					nodeNumber = rightNodeNumber;
				}
				else {
					break;
				}
			}
			leftNodeNumber = nodeNumber << 1;
		}
		elementList[nodeNumber - 1] = temp;
		return true;
	}
	
	size_t Size()
	{
		return elementCount;
	}
	
private:
	std::vector<T> elementList;
	bool (*IsCorrectOrder)(const T&, const T&);
	size_t elementCount;
};

bool IsCorrectOrder(const uint32_t& a, const uint32_t& b)
{
	return a < b;
}

#define ASSERT(x,e) if (!(x)) { std::cout << "assert failed: " << e << std::endl; exit(1); }

void Test()
{
	srand(42);
	const uint32_t testCount = 1000000;
#	if 1
#	define STEP 1
#	else
#	define STEP rand() % 2
#	endif	
	std::vector<uint32_t> testDataListInorder;
	for (uint32_t i = 0; i < testCount; ++i) {
		testDataListInorder.push_back(i + 1);
	}
	std::vector<uint32_t> testDataListDisorder(testDataListInorder);
	for (uint32_t i = 2; i < 100; ++i) {
		for (uint32_t j = 0; j < testCount; ++j) {
			if (rand() % i == 0) {
				uint32_t temp = testDataListDisorder[0];
				testDataListDisorder[0] = testDataListDisorder[j];
				testDataListDisorder[j] = temp;
			}
		}
	}
	
	BinaryHeap<uint32_t> heap(IsCorrectOrder);
	for (uint32_t i = 0; i < testCount; ++i) {
		heap.Push(testDataListDisorder[i]);
		ASSERT(heap.Size() == i + 1, "wrong size 1")
	}
	for (uint32_t i = 0; i < testCount; ++i) {
		ASSERT(heap.Size() == testCount - i, "wrong size 2");
		uint32_t temp = 0;
		bool ret = heap.GetTop(temp);
		ASSERT(ret, "GetTop failed");
		ASSERT(temp == testDataListInorder[i], "wrong top element 1");
		temp = 0;
		ret = heap.Pop(temp);
		ASSERT(temp == testDataListInorder[i], "wrong top element 2");
	}
	ASSERT(heap.Size() == 0, "size not 0");
	uint32_t temp = 0;
	bool ret = heap.GetTop(temp);
	ASSERT(!ret, "GetTop not failed");
	ret = heap.Pop(temp);
	ASSERT(!ret, "Pop not failed");
}

int main()
{
	Test();
	std::cout << "end" << std::endl;
	return 0;
}
