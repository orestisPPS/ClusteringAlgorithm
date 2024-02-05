//
// Created by hal9000 on 1/30/24.
//

#ifndef ALTAIRINTERVIEW_NODE_H
#define ALTAIRINTERVIEW_NODE_H
#include <vector>
#include <cmath>
#include <memory>
#include <stdexcept>
using namespace std;

class Node {
public:
    Node();
    
    ~Node();
    
    const shared_ptr<vector<double>>& getCoordinatesVector();
    
    unsigned int getId();
    
    void setCoordinatesVector(shared_ptr<vector<double>> coordinates);

    void setId(unsigned id);
    
    //double distanceTo(const shared_ptr<Node>& otherNode);
    
    
    
    
protected:

    
    shared_ptr<vector<double>> _coordinates;
    shared_ptr<unsigned> _id;
};


#endif //ALTAIRINTERVIEW_NODE_H
