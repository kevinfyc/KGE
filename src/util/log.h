#ifndef __LOG_HPP__
#define __LOG_HPP__


#define KGE_LOG_LVL_TRACE 1
#define KGE_LOG_LVL_DEBUG 10
#define KGE_LOG_LVL_INFO  20
#define KGE_LOG_LVL_WARN  30
#define KGE_LOG_LVL_ERROR 40
#define KGE_LOG_LVL_FATAL 50

#ifndef KGE_LOG_LVL
#   ifdef _RETAIL
#       define KGE_LOG_LVL KGE_LOG_WARN
#   else
#       define KGE_LOG_LVL KGE_LOG_LVL_TRACE
#   endif
#endif

namespace kge
{
	void kge_log(int lvl, const char* format, ...);
}

#if KGE_LOG_LVL_TRACE >= KGE_LOG_LVL
#   define KGE_LOG_TRACE(...) kge::kge_log(KGE_LOG_LVL_TRACE, __VA_ARGS__)
#else
#   define KGE_LOG_TRACE(...) (void)
#endif

#if KGE_LOG_LVL_DEBUG >= KGE_LOG_LVL
#   define KGE_LOG_DEBUG(...) kge::kge_log(KGE_LOG_LVL_DEBUG, __VA_ARGS__)
#else
#   define KGE_LOG_DEBUG(...) (void)
#endif

#if KGE_LOG_LVL_INFO >= KGE_LOG_LVL
#   define KGE_LOG_INFO(...) kge::kge_log(KGE_LOG_LVL_INFO, __VA_ARGS__)
#else
#   define KGE_LOG_INFO(...) (void)
#endif

#if KGE_LOG_LVL_WARN >= KGE_LOG_LVL
#   define KGE_LOG_WARN(...) kge::kge_log(KGE_LOG_LVL_WARN, __VA_ARGS__)
#else
#   define KGE_LOG_WARN(...) (void)
#endif

#if KGE_LOG_LVL_ERROR >= KGE_LOG_LVL
#   define KGE_LOG_ERROR(...) kge::kge_log(KGE_LOG_LVL_ERROR, __VA_ARGS__)
#else
#   define KGE_LOG_ERROR(...) (void)
#endif

#define KGE_LOG_FATAL(...) kge::kge_log(KGE_LOG_LVL_FATAL, __VA_ARGS__)

#define KGE_STACK_TRACE KGE_LOG_TRACE("%s() LINE:%d FILE:%s ", __FUNCTION__, __LINE__, __FILE__)

#define KGE_LOG_GL_ERROR()							\
    {												\
        int err = glGetError();						\
		if(err != 0)								\
            KGE_LOG_ERROR("glGetError: %d", err);	\
    }

#endif // __LOG_HPP__