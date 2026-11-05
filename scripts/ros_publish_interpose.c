#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* ABI probe only: the selected generated ROS message structs begin with
 * std_msgs::Header: uint32 seq, ROS time sec/nsec. Validate header stamps
 * against serialized subscriber messages before using the measurements. */
typedef void (*publish_fn)(const void *, const void *);
static int log_fd=-1;
static pthread_once_t setup_once=PTHREAD_ONCE_INIT;
static void setup(void) {
 const char *path=getenv("RMUA_PUBLISH_TRACE");
 if(path && *path) log_fd=open(path,O_WRONLY|O_CREAT|O_APPEND|O_CLOEXEC,0644);
}
static void record(const char *topic, const void *message) {
 pthread_once(&setup_once,setup);
 if(log_fd<0 || !message) return;
 uint32_t header[3];memcpy(header,message,sizeof header);
 struct timespec mt,wt;
 if(clock_gettime(CLOCK_MONOTONIC,&mt) || clock_gettime(CLOCK_REALTIME,&wt)) return;
 char buf[256];int n=snprintf(buf,sizeof buf,"{\"event\":\"hook_entry\",\"type\":\"%s\",\"mono_ns\":%lld,\"wall_ns\":%lld,\"seq\":%u,\"sec\":%u,\"nsec\":%u}\n",topic,(long long)mt.tv_sec*1000000000LL+mt.tv_nsec,(long long)wt.tv_sec*1000000000LL+wt.tv_nsec,header[0],header[1],header[2]);
 if(n>0 && n<(int)sizeof buf) {ssize_t ignored=write(log_fd,buf,(size_t)n);(void)ignored;}
}
#define DEFINE_PROBE(ID, TYPE, SYM) \
 static publish_fn real_##ID; static pthread_once_t once_##ID=PTHREAD_ONCE_INIT; \
 static void resolve_##ID(void) { *(void **)(&real_##ID)=dlsym(RTLD_NEXT,SYM); } \
 void ID(const void *,const void *) __asm__(SYM); \
 void ID(const void *pub,const void *msg) { \
  pthread_once(&once_##ID,resolve_##ID); \
  if(!real_##ID) {const char *s="RMUA publish interposer: original symbol missing\n";ssize_t ignored=write(STDERR_FILENO,s,strlen(s));(void)ignored;_exit(127);} \
  record(TYPE,msg); real_##ID(pub,msg); \
 }
DEFINE_PROBE(imu_publish,"imu","_ZNK3ros9Publisher7publishIN11sensor_msgs4Imu_ISaIvEEEEEvRKT_")
DEFINE_PROBE(lidar_publish,"lidar","_ZNK3ros9Publisher7publishIN11sensor_msgs12PointCloud2_ISaIvEEEEEvRKT_")
DEFINE_PROBE(pwm_publish,"rotor_pwm","_ZNK3ros9Publisher7publishIN10airsim_ros9RotorPWM_ISaIvEEEEEvRKT_")

typedef void (*callback_fn)(const void *);
static callback_fn real_pwm_callback;
static pthread_once_t once_callback=PTHREAD_ONCE_INIT;
#define CALLBACK_SYMBOL "_ZN10airsim_ros13commands_impl18ROTOR_PWM_CallbackERKN5boost10shared_ptrIKNS_9RotorPWM_ISaIvEEEEE"
static void resolve_callback(void) { *(void **)(&real_pwm_callback)=dlsym(RTLD_NEXT,CALLBACK_SYMBOL); }
void pwm_command_callback(const void *) __asm__(CALLBACK_SYMBOL);
void pwm_command_callback(const void *shared) {
 pthread_once(&once_callback,resolve_callback);
 if(!real_pwm_callback) {const char *s="RMUA callback interposer: original symbol missing\n";ssize_t ignored=write(STDERR_FILENO,s,strlen(s));(void)ignored;_exit(127);}
 const void *message=0;
 if(shared) memcpy(&message,shared,sizeof message);
 record("rotor_pwm_command_callback",message);
 real_pwm_callback(shared);
}
