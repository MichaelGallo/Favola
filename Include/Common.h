#pragma once

#include <QtGlobal>
#include <stdlib.h>
#include <stdint.h>

#define IN
#define OUT

#ifdef QT_DEBUG
#	define MSGASSERT(x, msg)		Q_ASSERT((x) && (msg))
#	define BREAK(msg)				Q_ASSERT(!(msg))
#	define VERIFY					Q_ASSERT
#else
#	define MSGASSERT(x, msg)		(void(0))
#	define BREAK(msg)				(void(0))
#	define VERIFY(x)				do { (void)x; } while (false)
#endif

#define SETMIN(a, b)				if ((b) < (a)) (a) = (b)
#define SETMAX(a, b)				if ((b) > (a)) (a) = (b)
#define CLAMP(a, l, h)				std::min(h, std::max(a, l))
#define SATURATE(a, l, h)			if ((a) < (l)) (a) = (l); else if ((a) > (h)) (a) = (h)
#define MINMAX(mn, mx, a, b, c)	do { if ((a) < (b)) if((c) < (a)) {(mn) = (c); (mx) = (b);} else {(mn) = (a); if((b) < (c)) (mx) = (c); else (mx) = (b);} \
										 else if ((c) < (b)) {(mn) = (c); (mx) = (a);} else {(mn) = (b); if ((a) < (c)) (mx) = (c); else (mx) = (a); } } while (false)

#define INIT(x)						m_##x(x)
#define COPY(x)						x(oOther.x)

#define UNUSED(x)

#define FILEUNIQUE3(base, line)		base##line
#define FILEUNIQUE2(base, line)		FILEUNIQUE3(base, line)
#define FILEUNIQUE(base)			FILEUNIQUE2(base, __LINE__)

template<class _Mutex> class unlock_guard
{
public:
	typedef _Mutex mutex_type;
	unlock_guard(_Mutex& _Mtx) : _MyMutex(_Mtx) { _MyMutex.unlock(); }
	~unlock_guard()								{ _MyMutex.lock(); }
	unlock_guard(const unlock_guard&) = delete;
	unlock_guard& operator=(const unlock_guard&) = delete;
private:
	_Mutex& _MyMutex;
};

#define SCOPEDMUTEX(mtx)				std::lock_guard<std::mutex> FILEUNIQUE(oScopedMutex) (mtx)
#define SCOPEDUNLOCK(lck)				unlock_guard<std::unique_lock<std::mutex>> FILEUNIQUE(oScopedMutexUnlock) (lck)
#define SCOPEDMUTEXUNLOCK(mtx)			unlock_guard<std::mutex> FILEUNIQUE(oScopedMutexUnlock) (mtx)
#define SCOPEDRECURSIVEMUTEX(mtx)		std::lock_guard<std::recursive_mutex> FILEUNIQUE(oScopedMutex) (mtx)
#define SCOPEDRECURMUTEXUNLOCK(mtx)		unlock_guard<std::recursive_mutex> FILEUNIQUE(oScopedMutexUnlock) (mtx)

#define DECLARESHAREDPTR(ClassName)		class ClassName; typedef std::shared_ptr<ClassName> ClassName##Ptr;

#define SORT(vec)						std::sort((vec).begin(), (vec).end())
#define SORTBY(type, mem)				bool operator < (const type& oOther) const	{ return mem < oOther.mem; }
#define SORTREVBY(type, mem)			bool operator < (const type& oOther) const	{ return mem > oOther.mem; }
#define SORTBY2(type, mem1, mem2)		bool operator < (const type& oOther) const	{ return (mem1 != oOther.mem1) ? (mem1 < oOther.mem1) : (mem2 < oOther.mem2); }
#define SORTREVBY2(type, mem1, mem2)	bool operator < (const type& oOther) const	{ return (mem1 != oOther.mem1) ? (mem1 > oOther.mem1) : (mem2 > oOther.mem2); }

#define STREQ(str1, str2)				((str1).compare(QString(str2), Qt::CaseInsensitive) == 0)
