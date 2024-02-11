#include <iostream>
#include <chrono>
#include "Tests/ProofOfConceptTest.h"
#include "Tests/AltairExample.h"
#include "Tests/RandomDomainND.h"
#include "Tests/MultithreadingTest.h"
using namespace std::chrono;

int main() {
    ProofOfConceptTest().run();
    AltairExample().run();
    RandomDomainND(10).run();
    MultithreadingTest().run();
    return 0;
}
