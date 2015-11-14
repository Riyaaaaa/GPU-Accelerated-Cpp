#include<amp.h>
#include<>

template<class T,std::size_t Rank,int... Args>
T& accessArrayByIndex(concurrency::array_view<T,Rank>& a,Args... indexes) restrict(amp)
{
	static_assert(sizeof...(indexes) == Rank,"number of index is incorrect");

	concurrency::index<Rank> idx(indexes...);
	return a[idx];
}


