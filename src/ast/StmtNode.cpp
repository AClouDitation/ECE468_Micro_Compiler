#include "../../inc/StmtNode.hpp"
#include <assert.h>

using namespace std;

StmtNode::StmtNode(FunctionDeclNode* farther): farther(farther) {}
StmtNode::~StmtNode() {}

