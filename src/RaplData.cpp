#include "RaplData.h"

using std::cout;
using std::cerr;
using std::endl;

RNET_POWER::RaplData::RaplData(const char * DomainName)
{
    this->m_DomainName = (char *) malloc(64 * sizeof(char));
    this->m_Units = (char *) malloc(16 * sizeof(char));
    strcpy(this->m_DomainName, DomainName);
    this->m_Value = 0;
    this->m_Scale = 0.0;
    this->attr_ptr = (struct perf_event_attr *) malloc(sizeof(struct perf_event_attr));
    this->perf_fd = 0;
}

RNET_POWER::RaplData::~RaplData()
{
    free(this->m_DomainName);
    free(this->m_Units);
    free(this->attr_ptr);
}

void RNET_POWER::RaplData::Init()
{
    FILE *fff;
    char filename[128];
    int type;
    int config = 0;

    fff = fopen(TYP_PTH, "r");
    if(fff != NULL)
    {
        if(fscanf(fff, "%d", &type) > 0)
        {
            cout << "Found type=" << type << endl;
        }
        fclose(fff);
    }
    else
        return;

    sprintf(filename, EVT_FMT, this->m_DomainName);
    fff = fopen(filename, "r");
    if(fff != NULL)
    {
        if(fscanf(fff, "event=%x", &config) > 0)
        {
            cout << "Found config=" << config << endl;
        }
        fclose(fff);
    }
    else
    {
        cerr << this->m_DomainName << " not found" << endl;
        this->attr_ptr = NULL;
        return;
    }

    sprintf(filename, SCL_FMT, this->m_DomainName);
    fff = fopen(filename, "r");
    if(fff != NULL)
    {
        if(fscanf(fff, "%lf", &this->m_Scale) > 0)
        {
            cout << "Found scale=" << this->m_Scale << endl;
        }
        fclose(fff);
    }
    else
        return;

    sprintf(filename, UNT_FMT, this->m_DomainName);
    fff = fopen(filename, "r");
    if(fff != NULL)
    {
        if(fscanf(fff, "%s", this->m_Units) > 0)
        {
            cout << "Found units=" << this->m_Units << endl;
        }
        fclose(fff);
    }
    else
        return;

    this->attr_ptr->type = type;
    this->attr_ptr->config = config;
}

int RNET_POWER::RaplData::Open()
{
    this->perf_fd = 0;
    if(this->attr_ptr == NULL)
        return -1;
    this->perf_fd = syscall(__NR_perf_event_open, this->attr_ptr, -1, 0, -1, 0);
    if(this->perf_fd < 0)
    {
        if(errno == EACCES)
        {
            cerr << "Permission denied" << endl;
            return -1;
        }
        else
        {
            cerr << "Error opening: " << strerror(errno) << endl;
            return -1;
        }
    }
    return 0;
}

void RNET_POWER::RaplData::Read()
{
    long long value;
    if(this->perf_fd > 0)
    {
        if(read(this->perf_fd, &value, 8) > 0)
        {
            this->m_Value = value;
        }
        close(this->perf_fd);
    }
}

void RNET_POWER::RaplData::Println()
{
    cout << this->m_DomainName << ": Energy consumed: " << (double)this->m_Value * this->m_Scale << " " << this->m_Units << endl;
}
