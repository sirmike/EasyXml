#ifndef SINGLETON
#define SINGLETON

template<typename T> class Singleton
{
    public:
        static T& Instance()
        {
            static T theSingleInstance;
            return theSingleInstance;
        }
};

#endif
