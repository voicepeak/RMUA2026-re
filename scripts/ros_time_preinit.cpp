// Diagnostic preload: invoke the shipped ROS time initializer before game startup.
extern "C" void _ZN3ros4Time4initEv();
__attribute__((constructor)) static void initialize_ros_time() {
    _ZN3ros4Time4initEv();
}
