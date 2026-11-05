// Development-only ABI interposer for libairsimros.so 69d5ea... .
// Serializes its three exported frequency-map entry points, preserving
// exceptions with RAII; it does not protect any unknown direct map accesses.
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <fcntl.h>
#include <mutex>
#include <pthread.h>
#include <unistd.h>

namespace {
std::mutex guard;
std::atomic<unsigned long long> calls{0}, contended{0};
int log_fd=-1;
pthread_once_t log_once=PTHREAD_ONCE_INIT;
void init_log() {
 const char *path=std::getenv("RMUA_FREQ_GUARD_LOG");
 if(path && *path) log_fd=open(path,O_WRONLY|O_CREAT|O_APPEND|O_CLOEXEC,0644);
}
struct Lock {
 Lock() {
  if(!guard.try_lock()){contended.fetch_add(1,std::memory_order_relaxed);guard.lock();}
  auto n=calls.fetch_add(1,std::memory_order_relaxed)+1;
  pthread_once(&log_once,init_log);
  if(log_fd>=0 && (n==1 || n%1000==0)) {
   char b[160];int length=std::snprintf(b,sizeof b,"{\"calls\":%llu,\"contended\":%llu}\n",n,contended.load(std::memory_order_relaxed));
   if(length>0 && length<(int)sizeof b){auto ignored=write(log_fd,b,length);(void)ignored;}
  }
 }
 ~Lock(){guard.unlock();}
};
void *resolve(const char *name) {
 void *p=dlsym(RTLD_NEXT,name);
 if(!p){const char *s="frequency guard: original symbol not found\n";auto ignored=write(STDERR_FILENO,s,std::strlen(s));(void)ignored;_exit(127);}
 return p;
}
}
#define RECORD_SYM "_ZN10airsim_ros21record_frequency_dataERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE"
#define FREQ_SYM "_ZN10airsim_ros19get_topic_frequencyERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE"
#define FREQ_ONE_SYM "_ZN10airsim_ros27get_topic_frequency_one_secERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE"
extern "C" void record_frequency_guard(const void *) __asm__(RECORD_SYM);
extern "C" float get_frequency_guard(const void *) __asm__(FREQ_SYM);
extern "C" float get_frequency_one_guard(const void *) __asm__(FREQ_ONE_SYM);
extern "C" void record_frequency_guard(const void *key) {
 using Fn=void(*)(const void*);static Fn original=reinterpret_cast<Fn>(resolve(RECORD_SYM));Lock lock;original(key);
}
extern "C" float get_frequency_guard(const void *key) {
 using Fn=float(*)(const void*);static Fn original=reinterpret_cast<Fn>(resolve(FREQ_SYM));Lock lock;return original(key);
}
extern "C" float get_frequency_one_guard(const void *key) {
 using Fn=float(*)(const void*);static Fn original=reinterpret_cast<Fn>(resolve(FREQ_ONE_SYM));Lock lock;return original(key);
}
