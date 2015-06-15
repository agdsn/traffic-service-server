#ifndef __SERVER__PROFILE_H__
#define __SERVER__PROFILE_H__

#if ! ( defined(NDEBUG) || defined(NOPROFILE) )
#include <atomic>
#include <string>
#include <gperftools/profiler.h>

namespace debug {
class _CpuProfileGuard
{
	const std::string profile_name;
	static std::atomic<unsigned int> cnt;
public:
	_CpuProfileGuard(const std::string &name)
	:
		profile_name(std::string(name + "_" + std::to_string(cnt++)) )
	{
		ProfilerStart(profile_name.c_str());
	}

	~_CpuProfileGuard()
	{
		ProfilerFlush();
		ProfilerStop();
	}
};

}
#define CPU_PROFILE(X) debug::_CpuProfileGuard _cpu_profile_guard(X)
#else
#define CPU_PROFILE(X)
#endif


#endif
