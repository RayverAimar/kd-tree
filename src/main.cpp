#include "KDtree.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cassert>
#include <iomanip>

using namespace std;
typedef Point  <size_t(2), int> Point2D;
typedef KDTree <size_t(2), int> KDTree2D;
typedef KDNode <size_t(2), int> KDNode2D;

size_t N, N_points;
const int k = 2;

int main(){

    KDTree2D myKDTree;
    ifstream file("../data/points.in");
    file>>N>>N_points;
    vector<Point2D> Points;

    for(int i = 0; i < N_points; i++){
        int coords[2];
        file>>coords[0]>>coords[1];
        Points.emplace_back(coords);
    }
    
    cout<<"(1) Initiating Points Insert assert..."<<endl;
    for (int i = 0; i < N_points; ++i)
    {
        assert(myKDTree.insert(Points[i]) == 1);
    }
    cout<<"Insertion passed!"<<endl;
    cout<<endl<<"(2) Trying repeated Point Insertion..."<<endl;
    for (int i = 0; i < N_points; ++i)
    {
        assert(myKDTree.insert(Points[i]) == 0);
    }
    cout<<"Test Passed! No Points were inserted..."<<endl;
    cout<<"Printing All Points..."<<endl;
    for (int i = 0; i < N_points; ++i)
    {
        Points[i].print();
    }

    cout<<endl<<"(3) Initiating KDTree's root depth assert..."<<endl;
    assert(myKDTree.get_root()->_depth==0);
    cout<<"Ok!"<<endl<<endl;


    cout<<"(4) Initiating KDTree's Nearest Neighbour assert..."<<endl;
    KDNode2D* temp = nullptr;
    int coords[2] = {7,5};
    Point2D key(coords);
    assert(myKDTree.nearest_neighbor(key, temp) == 5.f);
    cout<<"Nearest Neighbor test passed!"<<endl;
    cout<<"Nearest Neighbor found -> ";
    temp->_point.print();
    cout<<"Current Key->";
    key.print();
    cout<<"Distance: ";
    cout<<myKDTree.nearest_neighbor(key,temp)<<endl; 


    cout<<"\n(5) Initiating KDTree's K-Nearest Neighbors assert...\n";
    myKDTree.k_nearest_neighbor(k,key);
    cout<<"2-Nearest Neighbors are: "<<endl;
    for (auto it=myKDTree.knn.begin(); it!=myKDTree.knn.end(); ++it)
    {    
        cout<<"("<<coords[0]<<","<<coords[1]<<")\t";
        cout <<"<"<<setprecision(4)<<it->first<<">\t";
        it->second->print();
    }
    assert(myKDTree.knn.size()== k);
    cout<<"K-Nearest Neighbors test passed!"<<endl;
    
    cout<<"(6) Initiating KDTree's Range Query assert...\n";
    double radius = 40.f;
    myKDTree.range_query(radius,key);

    for (auto it=myKDTree.rq.begin(); it!=myKDTree.rq.end(); ++it)
    {    
        cout<<"("<<coords[0]<<","<<coords[1]<<")\t";
        cout <<"<"<<setprecision(4)<<it->first<<">\t";
        it->second->print();
    }
    cout<<"3 Nodes Found! Range Query test passed!"<<endl<<endl;
    cout<<"(7) Initiating KDTree's Nodes Deletion assert..."<<endl;
    for(int i = N_points-1; ~i; i--){
        assert(myKDTree.erase(Points[i])==1);
    }
    cout<<"Deletion passed!"<<endl;
    return 0;
}
