//
// Created by hal9000 on 2/10/24.
//

#ifndef ALTAIRINTERVIEW_NODECLUSTER_H
#define ALTAIRINTERVIEW_NODECLUSTER_H
#include "Node.h"
#include <list>

class NodeCluster {
public:
    NodeCluster(unsigned int id);
    
    ~NodeCluster();
    
    list<shared_ptr<Node>>& getNodes();
    
    unsigned int getId();

private:
    unsigned int _id;

    unique_ptr<list<shared_ptr<Node>>> _nodes;

};


#endif //ALTAIRINTERVIEW_NODECLUSTER_H
