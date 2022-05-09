#ifndef KD_NODE
#define KD_NODE

#include "Point.h"

template <size_t N, typename ElemType>
class KDNode {
public:
    KDNode();
    KDNode(Point<N, ElemType>&, size_t);
    ~KDNode();
    Point<N,ElemType> _point;
    KDNode<N,ElemType>* m_nodes[2];
    size_t _depth;

};

template <size_t N, typename ElemType>
KDNode<N,ElemType>::KDNode()
{
    _depth = 0;
    m_nodes[0] = m_nodes[1] = nullptr;
}

template <size_t N, typename ElemType>
KDNode<N,ElemType>::KDNode(Point<N,ElemType>& point, size_t depth):_point(point), _depth(depth)
{
    m_nodes[0] = m_nodes[1] = nullptr;
}

template <size_t N, typename ElemType>
KDNode<N,ElemType>::~KDNode(){

}

#endif //KD_NODE