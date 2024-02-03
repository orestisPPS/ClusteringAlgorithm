//
// Created by hal9000 on 1/30/24.
//

#ifndef ALTAIRINTERVIEW_NODECLOUD_H
#define ALTAIRINTERVIEW_NODECLOUD_H

#include "Node.h"
#include <map>
class NodeCloud {
public:
    explicit NodeCloud(shared_ptr<vector<shared_ptr<Node>>> nodesVector);
    
    ~NodeCloud();
    
    
protected:
    shared_ptr<vector<shared_ptr<Node>>> _nodesVector;
};


#endif //ALTAIRINTERVIEW_NODECLOUD_H
