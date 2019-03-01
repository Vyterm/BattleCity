#ifndef VYTERM_ALGORITHM_HPP_INCLUDED
#define VYTERM_ALGORITHM_HPP_INCLUDED

#include <algorithm>
#include <map>

namespace vyt
{
#pragma region Search

	template <typename TSTL, typename TIter, typename TValue>
	TIter Find(const TSTL &stl, const TValue &value)
	{
		return std::find(stl.begin(), stl.end(), value);
	}
	template <typename TIter, typename TKey, typename TValue>
	TIter FindByKey(const std::map<TKey, TValue> &map, const TKey &key)
	{
		return map.find(key);
	}
	template <typename TSTL, typename TValue>
	int IndexOf(const TSTL &stl, const TValue &value)
	{
		return std::distance(stl.begin(), vyt::Find(stl, value));
	}
	template <typename TKey, typename TValue>
	int IndexOfKey(const std::map<TKey, TValue> &map, const TKey &key)
	{
		return int(std::distance(map.begin(), map.find(key)));
	}

#pragma endregion

#pragma region Append

	template <typename TSTL, typename TValue>
	void Append(TSTL &stl, const TValue &value)
	{
		stl.push_back(value);
	}

#pragma endregion

#pragma region Remove

	template <typename TSTL, typename TValue>
	bool Remove(TSTL &stl, const TValue &value)
	{
		auto iter = std::find(stl.begin(), stl.end(), value);
		if (iter == stl.end()) return false;
		stl.erase(iter);
		return true;
	}
	template <typename TKey, typename TValue>
	bool RemoveByKey(std::map<TKey, TValue> &map, const TKey &key)
	{
		auto iter = map.find(key);
		if (iter == map.end()) return false;
		map.erase(iter);
		return true;
	}

#pragma endregion

}

#endif