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
    RandomDomainND<2,10>().run();
    MultithreadingTest<2,10000, 0>(0.2).run();
    cout << "The tests have finished running." << endl;
    //TemplateNodeCreationTest().run();
    return 0;
}
