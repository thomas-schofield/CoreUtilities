#include "Usage.H"
#include <LoggingServices/api/cpp/LoggingIntf.H>
#include "AnotherInclude/AnotherInclude.H"

namespace
{
    static void usageConstructor() __attribute__((constructor(101)));

    void usageConstructor()
    {

    }
}