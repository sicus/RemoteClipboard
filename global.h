#ifdef linux
const int OPERATING_SYSTEM = 1;
#elif defined WIN32
const int OPERATING_SYSTEM = 2;
#else
const int OPERATING_SYSTEM = 0;
#endif
