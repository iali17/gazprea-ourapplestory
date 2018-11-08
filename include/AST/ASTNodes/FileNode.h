//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_FILENODE_H
#define GAZPREABASE_FILENODE_H
#include "BaseNodes/BasicBlockNode.h"

class FileNode : public BasicBlockNode {
public:
    FileNode(std::vector<ASTNode *> *, int line);

    explicit FileNode(int line);
};
#endif //GAZPREABASE_FILENODE_H
