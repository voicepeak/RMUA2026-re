// Diagnostic only: do not serialize the three original frequency functions.
// A same-key entry/return overlap is not proof of overlapping deque accesses.
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <fcntl.h>
#include <pthread.h>
#include <string>
#include <unistd.h>
namespace {
struct Slot { std::atomic<uint64_t> hash{0}; std::atomic<unsigned> readers{0}, writers{0}; };
Slot slots[128];
thread_local unsigned local_readers[128]={}, local_writers[128]={};
std::atomic<unsigned long long> calls{0}, overlaps{0}, untracked{0};
pthread_once_t once=PTHREAD_ONCE_INIT; int fd=-1;
void init() { const char *p=getenv("RMUA_FREQ_GUARD_LOG"); if(p&&*p) fd=open(p,O_WRONLY|O_CREAT|O_APPEND|O_CLOEXEC,0644); }
void *resolve(const char *n) { void *p=dlsym(RTLD_NEXT,n); if(!p) _exit(127); return p; }
uint64_t hash_key(const std::string &s) { uint64_t h=14695981039346656037ull; for(unsigned char c:s) {h^=c;h*=1099511628211ull;} return h ? h : 1; }
struct Scope {
 Slot *slot=nullptr; bool writer; size_t index=0;
 Scope(const std::string &key,bool w):writer(w) {
  uint64_t h=hash_key(key); size_t start=h%128;
  for(size_t i=0;i<128;i++) { Slot &s=slots[(start+i)%128]; uint64_t old=s.hash.load();
   if(old==0) { uint64_t expected=0; s.hash.compare_exchange_strong(expected,h); old=s.hash.load(); }
   if(old==h) {slot=&s;index=(start+i)%128;break;}
  }
  pthread_once(&once,init); bool overlap=false; unsigned nr=0,nw=0;
  if(slot) {
   if(writer) {nw=slot->writers.fetch_add(1); nr=slot->readers.load(); overlap=nw>local_writers[index] || nr>local_readers[index]; ++local_writers[index];}
   else {nr=slot->readers.fetch_add(1);nw=slot->writers.load();overlap=nw>local_writers[index]; ++local_readers[index];}
  } else untracked.fetch_add(1);
  auto n=calls.fetch_add(1)+1; auto o=overlap ? overlaps.fetch_add(1)+1 : overlaps.load();
  if(fd>=0 && (n==1 || n%1000==0 || overlap)) {
   char b[256];int len=snprintf(b,sizeof b,"{\"calls\":%llu,\"overlaps\":%llu,\"untracked\":%llu,\"key_hash\":\"%016llx\",\"key_length\":%zu,\"writer\":%d,\"prior_readers\":%u,\"prior_writers\":%u,\"overlap\":%s}\n",n,o,untracked.load(),(unsigned long long)h,key.size(),writer?1:0,nr,nw,overlap?"true":"false");
   if(len>0 && len<(int)sizeof b) {auto ignored=write(fd,b,len);(void)ignored;}
  }
 }
 ~Scope() {if(slot) {if(writer) {--local_writers[index];slot->writers.fetch_sub(1);}else {--local_readers[index];slot->readers.fetch_sub(1);}}}
};
}
#define R "_ZN10airsim_ros21record_frequency_dataERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE"
#define G "_ZN10airsim_ros19get_topic_frequencyERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE"
#define O "_ZN10airsim_ros27get_topic_frequency_one_secERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE"
extern "C" void r(const std::string&) __asm__(R);
extern "C" float g(const std::string&) __asm__(G);
extern "C" float o(const std::string&) __asm__(O);
extern "C" void r(const std::string &key) {using Fn=void(*)(const std::string&);static Fn f=(Fn)resolve(R);Scope s(key,true);f(key);}
extern "C" float g(const std::string &key) {using Fn=float(*)(const std::string&);static Fn f=(Fn)resolve(G);Scope s(key,false);return f(key);}
extern "C" float o(const std::string &key) {using Fn=float(*)(const std::string&);static Fn f=(Fn)resolve(O);Scope s(key,false);return f(key);}
