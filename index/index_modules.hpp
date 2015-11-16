#ifndef __INDEX_MODULES_H__
#define __INDEX_MODULES_H__

#include<amp.h>

/* Variadic template parameter arguments require dimention indexes */
template<class T,int Rank,typename... Args>
T& accessArrayByIndex(const concurrency::array_view<T,Rank>& a,Args... indexes) restrict(amp)
{
	static_assert(sizeof...(indexes) == Rank,"number of index is incorrect");

	concurrency::index<Rank> idx(indexes...);
	return a[idx];
}

template<class T,int Rank,typename... Args>
T& accessArrayByIndex(const concurrency::array_view<T,Rank>& a,Args... indexes) restrict(cpu)
{
	static_assert(sizeof...(indexes) == Rank,"number of index is incorrect");

	concurrency::index<Rank> idx(indexes...);
	return a[idx];
}

#endif
