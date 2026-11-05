// Development probe: tracks overlapping calls on identical deque addresses.
// No target mutex; wrapper overlap is not proof of simultaneous memory access.
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include <fcntl.h>
#include <pthread.h>
#include <string>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>
namespace {
struct Slot {std::atomic<uintptr_t> ptr{0};std::atomic<unsigned> readers{0},writers{0};};
Slot slots[256];
thread_local unsigned freq_depth=0,local_readers[256]={},local_writers[256]={};
std::atomic<unsigned long long> operations{0},overlaps{0},untracked{0};
pthread_once_t once=PTHREAD_ONCE_INIT;int fd=-1;
void init() {const char *p=getenv("RMUA_FREQ_GUARD_LOG");if(p&&*p)fd=open(p,O_WRONLY|O_CREAT|O_APPEND|O_CLOEXEC,0644);}
void *resolve(const char *n) {void *p=dlsym(RTLD_NEXT,n);if(!p)_exit(127);return p;}
struct FreqScope {FreqScope(){++freq_depth;} ~FreqScope(){--freq_depth;}};
struct DequeScope {
 Slot *slot=nullptr;size_t index=0;bool writer;
 DequeScope(const void *object,bool w,const char *op):writer(w) {
  if(!freq_depth)return;
  uintptr_t key=(uintptr_t)object;size_t start=(key>>4)%256;
  for(size_t i=0;i<256;i++) {Slot &s=slots[(start+i)%256];uintptr_t old=s.ptr.load();
   if(!old){uintptr_t expected=0;s.ptr.compare_exchange_strong(expected,key);old=s.ptr.load();}
   if(old==key){slot=&s;index=(start+i)%256;break;}}
  bool conflict=false;unsigned nr=0,nw=0;
  if(slot) {
   if(writer){nw=slot->writers.fetch_add(1);nr=slot->readers.load();conflict=nw>local_writers[index] || nr>local_readers[index];++local_writers[index];}
   else {nr=slot->readers.fetch_add(1);nw=slot->writers.load();conflict=nw>local_writers[index];++local_readers[index];}
  }else untracked.fetch_add(1);
  auto n=operations.fetch_add(1)+1;auto o=conflict?overlaps.fetch_add(1)+1:overlaps.load();
  pthread_once(&once,init);
  if(fd>=0&&(n==1||n%1000==0||conflict)) {
   timespec ts{};clock_gettime(CLOCK_MONOTONIC,&ts);
   char b[320];int len=snprintf(b,sizeof b,"{\"operations\":%llu,\"overlaps\":%llu,\"untracked\":%llu,\"object\":\"%p\",\"op\":\"%s\",\"tid\":%ld,\"mono_ns\":%llu,\"prior_readers\":%u,\"prior_writers\":%u,\"overlap\":%s}\n",n,o,untracked.load(),object,op,syscall(SYS_gettid),(unsigned long long)ts.tv_sec*1000000000ull+(unsigned long long)ts.tv_nsec,nr,nw,conflict?"true":"false");
   if(len>0&&len<(int)sizeof b){auto ignored=write(fd,b,len);(void)ignored;}
  }
 }
 ~DequeScope(){if(slot){if(writer){--local_writers[index];slot->writers.fetch_sub(1);}else{--local_readers[index];slot->readers.fetch_sub(1);}}}
};
}
#define R "_ZN10airsim_ros21record_frequency_dataERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE"
#define G "_ZN10airsim_ros19get_topic_frequencyERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE"
#define O "_ZN10airsim_ros27get_topic_frequency_one_secERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE"
#define P "_ZNSt5dequeImSaImEE9push_backERKm"
#define Q "_ZNSt5dequeImSaImEE9pop_frontEv"
#define S "_ZNKSt5dequeImSaImEE4sizeEv"
extern "C" void r(const std::string&) __asm__(R);
extern "C" float g(const std::string&) __asm__(G);
extern "C" float o(const std::string&) __asm__(O);
extern "C" void push(void*,const uint64_t&) __asm__(P);
extern "C" void pop(void*) __asm__(Q);
extern "C" size_t size(const void*) __asm__(S);
extern "C" void r(const std::string &key){using Fn=void(*)(const std::string&);static Fn f=(Fn)resolve(R);FreqScope scope;f(key);}
extern "C" float g(const std::string &key){using Fn=float(*)(const std::string&);static Fn f=(Fn)resolve(G);FreqScope scope;return f(key);}
extern "C" float o(const std::string &key){using Fn=float(*)(const std::string&);static Fn f=(Fn)resolve(O);FreqScope scope;return f(key);}
extern "C" void push(void *p,const uint64_t &x){using Fn=void(*)(void*,const uint64_t&);static Fn f=(Fn)resolve(P);DequeScope scope(p,true,"push_back");f(p,x);}
extern "C" void pop(void *p){using Fn=void(*)(void*);static Fn f=(Fn)resolve(Q);DequeScope scope(p,true,"pop_front");f(p);}
extern "C" size_t size(const void *p){using Fn=size_t(*)(const void*);static Fn f=(Fn)resolve(S);DequeScope scope(p,false,"size");return f(p);}
