#include "Node.h"

Node::Node()
:mId(BAD_NODE_ID),
 mYPos(0),
 mXPos(0)
{
}

Node::Node( const NODE_ID& id, const int x, const int y)
:mId(id),
 mXPos(x),
 mYPos(y)
{
}

Node::~Node()
{
}