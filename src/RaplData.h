#ifndef RAPLDATA_H
#define RAPLDATA_H

#include <iostream>
#include <string>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>

using std::string;

namespace POWERAPI
{
#define TYP_PTH "/sys/bus/event_source/devices/power/type"
#define EVT_FMT "/sys/bus/event_source/devices/power/events/%s"
#define SCL_FMT "/sys/bus/event_source/devices/power/events/%s.scale"
#define UNT_FMT "/sys/bus/event_source/devices/power/events/%s.unit"

    class RaplData
    {
        public:
            RaplData(const char * DomainName);
            virtual ~RaplData();
            char * GetDomainName()
            {
                return m_DomainName;
            }
            long long GetValue()
            {
                return m_Value;
            }
            double GetScale()
            {
                return m_Scale;
            }
            char * GetUnits()
            {
                return m_Units;
            }
            void Init();
            int Open();
            void Read();
            void Println();
        protected:
        private:
            char * m_DomainName;
            long long m_Value;
            double m_Scale;
            char * m_Units;
            struct perf_event_attr * attr_ptr;
            int perf_fd;
    };
}
#endif // RAPLDATA_H
