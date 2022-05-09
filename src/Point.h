/**
 * File: Point.h
 * -------------
 * A class representing a point in N-dimensional space. Unlike the other class
 * templates you've seen before, Point is parameterized over an integer rather
 * than a type. This allows the compiler to verify that the type is being used
 * correctly.
 */
#ifndef POINT_INCLUDED
#define POINT_INCLUDED

#include <algorithm>
#include <cmath>
#include <iostream>
 
using std::size_t; using std::cout;

template <size_t N, typename ElemType>
class Point {
public:

    //Constructors & Destructors
    Point();
    Point(ElemType*);
    ~Point();

    //Getters
    size_t size() const;

    //Primary Functions
    bool insert(size_t, ElemType);

    //Operators
    bool operator == (const Point<N,ElemType>&);
    bool operator != (const Point<N,ElemType>&);
    Point<N,ElemType>& operator = (const Point<N,ElemType>&);
    ElemType  operator []  (size_t) const;
    
    //Auxiliar Functions
    void print();

private:
    ElemType* coords;
    size_t _end, _begin, _size;
};

template <size_t N, typename ElemType>
Point<N, ElemType>::Point(): _end(N-1), _size(N), _begin(0){
    coords = new ElemType[N];
    for(size_t i = 0; i < _size; i++)
    {
        coords[i]=0;
    }
}

template <size_t N, typename ElemType>
Point<N, ElemType>::Point(ElemType* p): _end(N-1), _size(N), _begin(0){
    coords = new ElemType[N];
    for(size_t i = 0; i < _size; i++)
    {
        coords[i]=p[i];
    }
}

template <size_t N, typename ElemType>
Point<N, ElemType>::~Point(){
    delete[] coords;
}

template <size_t N, typename ElemType>
size_t Point<N, ElemType>::size() const {
    return _size;
}

template <size_t N, typename ElemType>
bool Point<N, ElemType>::insert(size_t i, ElemType value){
    if(i<N)
    {
        return coords[i]=value;
    }
    return 0;
}

template <size_t N, typename ElemType>
double Distance(const Point<N, ElemType>& one, const Point<N, ElemType>& two);

template <size_t N, typename ElemType>
bool Point<N, ElemType>::operator==(const Point<N, ElemType>& p){
    for(size_t i = 0; i < _size; i++){
        if(coords[i]!=p[i]){
            return 0;
        }
    }
    return 1;
}

template <size_t N, typename ElemType>
bool Point<N, ElemType>::operator!=(const Point<N, ElemType>& p){
    for(size_t i = 0; i < _size; i++){
        if(coords[i]==p[i]){
            return 0;
        }
    }
    return 1;
}

template <size_t N, typename ElemType>
Point<N, ElemType>& Point<N, ElemType>::operator = (const Point<N,ElemType>& point)
{   
    for (size_t i = 0; i < _size; ++i)
    {
        coords[i] = point[i];
    }
    return *this;
}

template <size_t N, typename ElemType>
ElemType Point<N, ElemType>::operator[] (size_t i) const{
    return coords[i];
}

template <size_t N, typename ElemType>
void Point<N, ElemType>::print(){
    cout<<"(";
    for(size_t i = 0; i < _size; i++){
        cout<<coords[i];
        if (i<_size-1)
        {
            cout<<", ";
        }
    }
    cout<<")\n";
}


template <size_t N, typename ElemType>
double Distance(const Point<N, ElemType>& one, const Point<N, ElemType>& two) {
    double result = 0.0;
    for (size_t i = 0; i < N; ++i)
        result += (one[i] - two[i]) * (one[i] - two[i]);
    
    return sqrt(result);
}

#endif // POINT_INCLUDED
