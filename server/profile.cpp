#include "profile.h"


#if ! ( defined(NDEBUG) || defined(NOPROFILE) )
std::atomic<unsigned int> debug::_CpuProfileGuard::cnt(0);
#endif
