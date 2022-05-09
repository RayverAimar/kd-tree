#ifndef KDTREE_INCLUDED
#define KDTREE_INCLUDED

#include "Point.h"
#include "KDNode.h"
#include <stdexcept>
#include <cmath>
#include <stack>
#include <map>

using namespace std;

template <size_t N, typename ElemType>
class KDTree {
public:

    // Constructor & Destructor
    KDTree();
    ~KDTree();
    
    //Primary Functions
    bool find(KDNode<N,ElemType>**&, Point<N,ElemType>&, size_t&);
    bool insert(Point<N, ElemType>&);
    bool erase(Point<N, ElemType>&);

    //Secondary Functions
    double nearest_neighbor(Point<N,ElemType>&, KDNode<N,ElemType>*&);
    void nearest_neighbor(KDNode<N,ElemType>*,size_t,Point<N,ElemType>&, KDNode<N,ElemType>*&, double&);

    void k_nearest_neighbor(KDNode<N,ElemType>*, size_t, Point<N,ElemType>&, map<double, Point<N,ElemType>*>&, size_t, double);
    void k_nearest_neighbor(size_t,Point<N,ElemType>&);

    void range_query(KDNode<N,ElemType>*,size_t,Point<N,ElemType>&, map<double, Point<N,ElemType>*>&, double);
    void range_query(double,Point<N,ElemType>&);
    
    //Auxiliar Functions
    bool empty() const;

    //Getters
    size_t dimension() const;
    size_t size() const;
    KDNode<N,ElemType>* get_root() const;
    KDNode<N,ElemType>** get_next(KDNode<N, ElemType>**); 
    
    //Others
    map<double, Point<N,ElemType>*> K_nearest;
    map<double, Point<N,ElemType>*> R_query;

private:
    size_t _size, _dimension;
    KDNode<N, ElemType>* root;
    
};


template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree() {
    root = nullptr;
    _size=0;
    _dimension=N;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::~KDTree() {
    
}
template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::dimension() const {
    return N;
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::size() const {
    return _size;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::empty() const {
    return !_size;
}

template <std::size_t N, typename ElemType>
KDNode<N,ElemType>* KDTree<N, ElemType>::get_root() const 
{
    return root;
}

template <size_t N, typename ElemType>
KDNode<N,ElemType>** KDTree<N, ElemType>::get_next(KDNode<N,ElemType>** p)
{
    KDNode<N,ElemType>** l = &((*p)->m_children[0]);
    KDNode<N,ElemType>** r = &((*p)->m_children[1]);
    while((*l)->m_children[0] && (*r)->m_children[1]){
        l = &((*l)->m_children[0]);
        r = &((*r)->m_children[1]);
    }
    return (!!(*l))? l : r;
}


template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::find(KDNode<N,ElemType>**& cur_node, Point<N,ElemType>& point, size_t &depth)
{
    cur_node = &root;
    size_t cur_depth = -1;
    for(
        cur_node = &root;
        *cur_node && (*cur_node)->_point!=point;
        cur_depth=(*cur_node)->_depth, 
        cur_node = &((*cur_node)->m_children[ point[cur_depth%_dimension] > (*cur_node)->_point[cur_depth%_dimension]])
        );
    depth= cur_depth;
    return !!* cur_node;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::insert(Point<N, ElemType>& point)
{
    size_t cur_depth;
    KDNode<N, ElemType>** p;
    if(find(p, point, cur_depth))
    {
        return 0;
    }
    KDNode<N,ElemType>* t = new KDNode<N,ElemType>(point,++cur_depth);
    *p = t;
    return 1;
}
template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::erase(Point<N, ElemType>& p)
{
    size_t cur_depth;
    KDNode<N, ElemType>** cur_node;
    if(!find(cur_node, p, cur_depth))
    {
        return 0;
    }

    if ((*cur_node)->m_children[0] && (*cur_node)->m_children[1]) 
    {
        KDNode<N,ElemType>** q = get_next(cur_node);
        (*cur_node)->_point = (*q)->_point;
        cur_node = q;
    }
    KDNode<N,ElemType>* t = *cur_node;
    *cur_node = (*cur_node)->m_children[!(*cur_node)->m_children[0]];
    delete t;
    return 1;
}


template <size_t N, typename ElemType>
double KDTree<N, ElemType>::nearest_neighbor(Point<N,ElemType>& key, KDNode<N,ElemType>* &p)
{
    double best_distance = numeric_limits<double>::infinity();
    nearest_neighbor(root,size_t(0),key,p, best_distance);
    return Distance(p->_point,key);
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::nearest_neighbor(KDNode<N,ElemType>* cur_node, size_t depth, Point<N,ElemType>& key, KDNode<N,ElemType>*& candidate_node, double& best_distance){
    if (!cur_node) return;
    
    if (Distance(cur_node->_point, key) < best_distance)
    {
        best_distance = Distance(cur_node->_point, key);
        candidate_node = cur_node;
    }

    int axis = depth % _dimension;
    bool right = false;
    
    if (key[axis] < cur_node->_point[axis]) 
    {
        right = true;
        nearest_neighbor(cur_node->m_children[0], ++depth, key, candidate_node, best_distance);
    } 
    else 
    {
        right = false;
        nearest_neighbor(cur_node->m_children[1], ++depth, key, candidate_node, best_distance);
    }

    if (fabs(cur_node->_point[axis] - key[axis]) < best_distance) 
    {
        if (right) nearest_neighbor(cur_node->m_children[0], ++depth, key, candidate_node, best_distance);
        else nearest_neighbor(cur_node->m_children[1], ++depth,key,  candidate_node, best_distance);
    }
}


template <size_t N, typename ElemType>
void KDTree<N, ElemType>::k_nearest_neighbor(size_t k, Point<N,ElemType>& key)
{
    double best_distance = numeric_limits<double>::infinity();
    k_nearest_neighbor(root,size_t(0),key,K_nearest,k, best_distance);
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::k_nearest_neighbor(   KDNode<N,ElemType>* cur_node, size_t depth, Point<N,ElemType>& key, map<double, Point<N,ElemType>*>& m, size_t k, double best_distance)
{
    if (!cur_node) return;
    
    if (Distance(cur_node->_point, key) < best_distance)
    {
        best_distance = Distance(cur_node->_point, key);
        
        if(m.size() < k){
            m.insert({ Distance(cur_node->_point, key),&cur_node->_point });
        }
        else
        {
            auto _begin = m.begin(),  _end = m.end();
            if (_begin->first > best_distance)
            {
                _end--;
                m.erase(_end);
                m.insert({ Distance(cur_node->_point, key),&cur_node->_point });
            }
        }
    }
    
    int axis = depth % _dimension;
    bool right = false;
    
    if (key[axis] < cur_node->_point[axis]) 
    {
        right = true;
        k_nearest_neighbor(cur_node->m_children[0], ++depth, key, m,k,best_distance);
    } 
    else 
    {
        right = false;
        k_nearest_neighbor(cur_node->m_children[1], ++depth, key, m,k,best_distance);
    }

    if (fabs(cur_node->_point[axis] - key[axis]) < best_distance) 
    {
        if (right) k_nearest_neighbor(cur_node->m_children[0], ++depth, key, m,k,best_distance);
        else k_nearest_neighbor(cur_node->m_children[1], ++depth, key, m,k,best_distance);
    }
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::range_query(  KDNode<N,ElemType>* cur_node, size_t depth, Point<N,ElemType>& key, map<double,Point<N,ElemType>*>& m,  double radius)
{
    if (!cur_node) return;
    
    if (Distance(cur_node->_point, key) < radius){
        m.insert({ Distance(cur_node->_point, key), &cur_node->_point });
    }
        
    int axis = depth % _dimension;
    bool right = false;
    
    if (key[axis] < cur_node->_point[axis]) 
    {
        right = true;
        range_query(cur_node->m_children[0], ++depth, key, m,radius);
    } 
    else 
    {
        right = false;
        range_query(cur_node->m_children[1], ++depth, key, m,radius);
    }

    if (fabs(cur_node->_point[axis] - key[axis]) < radius) 
    {
        if (right) range_query(cur_node->m_children[0], ++depth, key, m,radius);
        else range_query(cur_node->m_children[1], ++depth, key, m,radius);
    }
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::range_query(double radius,Point<N,ElemType>& key)
{
    range_query(root,0,key,R_query, radius);
}

#endif // KDTREE_INCLUDED