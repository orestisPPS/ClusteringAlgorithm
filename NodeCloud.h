//
// Created by hal9000 on 1/30/24.
//

#ifndef ALTAIRINTERVIEW_NODECLOUD_H
#define ALTAIRINTERVIEW_NODECLOUD_H

#include "Node.h"
#include <map>
class NodeCloud {
public:
    NodeCloud();
    
    ~NodeCloud();
    
    
protected:
    unsigned _numberOfNodes;
    
    unique_ptr<shared_ptr<Node>> _nodesVector;
    
    unique_ptr<map<pair<unsigned, unsigned>, unsigned>> _coordinatesToIdMap; 
};


#endif //ALTAIRINTERVIEW_NODECLOUD_H
