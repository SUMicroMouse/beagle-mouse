#ifndef __SORTS_H__
#define __SORTS_H__

#include <vector>

namespace Sorting
{
	template<typename T>
	class Sorts
	{
	public:
		static void swapvals(T &val1, T &val2)
		{
			T temp = val1;
			val1 = val2;
			val2 = temp;
		}

		static void reverse(std::vector<T> &arr)
		{
			int l = 0, u = arr.size() - 1;

			while (l < u)
			{
				swapvals(arr[l], arr[u]);
				l++;
				u--;
			}
		}

	private:
		// quicksort
		static int partition(std::vector<T> &arr, int l, int r)
		{
			int i = l - 1, j = l;

			while (j < r)
			{
				if (arr[j] <= arr[r])
				{
					i++;
					swapvals(arr[j], arr[i]);	// swap a[i] & a[j]
				}
				j++;
			}

			swapvals(arr[i + 1], arr[r]);

			return i + 1;
		}

	public:
		// Quicksort
		static void Sorts::quicksort(std::vector<T> &arr, int l, int r)
		{
			if (l >= r)
				return;

			int index = partition(arr, l, r);

			quicksort(arr, l, index - 1);		// left
			quicksort(arr, index + 1, r);		// right
		}
	};
}

#endif