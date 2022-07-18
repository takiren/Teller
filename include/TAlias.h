#ifndef _TELLER_H_TALIAS_
#define _TELLER_H_TALIAS_

#include<map>

namespace teller {
	template<class KEY, class DATA>
	using HashMap = std::unordered_map<KEY, DATA>;
}

#endif // !_TELLER_H_TALIAS_
