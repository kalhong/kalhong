#include <map>

#include "Gem5Wrapper.h"
#include "Config.h"
#include "Request.h"
#include "MemoryFactory.h"
#include "Memory.h"
#include "DDR3.h"
#include "DDR4.h"
#include "LPDDR3.h"
#include "LPDDR4.h"
#include "GDDR5.h"
#include "WideIO.h"
#include "WideIO2.h"
#include "HBM.h"
#include "SALP.h"
//HJHJ
#include <iostream>
#include "Statistics.h"

using namespace ramulator;

static map<string, function<MemoryBase *(const Config&, int)> > name_to_func = {
    {"DDR3", &MemoryFactory<DDR3>::create}, {"DDR4", &MemoryFactory<DDR4>::create},
    {"LPDDR3", &MemoryFactory<LPDDR3>::create}, {"LPDDR4", &MemoryFactory<LPDDR4>::create},
    {"GDDR5", &MemoryFactory<GDDR5>::create}, 
    {"WideIO", &MemoryFactory<WideIO>::create}, {"WideIO2", &MemoryFactory<WideIO2>::create},
    {"HBM", &MemoryFactory<HBM>::create},
    {"SALP-1", &MemoryFactory<SALP>::create}, {"SALP-2", &MemoryFactory<SALP>::create}, {"SALP-MASA", &MemoryFactory<SALP>::create},
};

Gem5Wrapper::Gem5Wrapper(const Config& configs, string& statsPrefix, int cacheline)
{
    const string& std_name = configs["standard"];
    assert(name_to_func.find(std_name) != name_to_func.end() && "unrecognized standard name");
    mem = name_to_func[std_name](configs, cacheline);
    tCK = mem->clk_ns();
    stats_name = statsPrefix + "ramulator.stats";
    Stats::statlist.output(stats_name);
}

Gem5Wrapper::~Gem5Wrapper() {
    delete mem;
}

void Gem5Wrapper::tick()
{
    mem->tick();
}

void Gem5Wrapper::printall()
{
    Stats::statlist.output(stats_name);
    Stats::statlist.printall();
}

bool Gem5Wrapper::send(Request req)
{
    return mem->send(req);
}

void Gem5Wrapper::finish(void) {
    mem->finish();
}
