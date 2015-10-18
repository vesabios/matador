//
//  Map.hpp
//
//  Created by vesabios on 10/5/15.
//
//

#ifndef Map_h
#define Map_h
#include <stdio.h>
#include "ofMain.h"
#include "ObjectFactory.h"
#include "Materials.h"
#include "ofxDelaunay.h"



struct Tile {
    public:
    bool canWalk;
    Material * mat;
    int lastSeen = 100000;
    Tile()
    : canWalk(false)
    , mat(0)
    {} 
};

class Map {
    
    struct Edge
    {
        int src, dest, weight;
    };
    
    struct Graph
    {
        int V, E;
        struct Edge* edge;
    };
    
    
    struct find_edge
    {
        Edge e;
        find_edge(Edge e) : e(e) {}
        bool operator () ( const Edge& c ) const
        {
            return (c.src == e.src) && (c.dest==e.dest);
        }
    };
    
    struct Graph* createGraph(int V, int E)
    {
        struct Graph* graph = (struct Graph*) malloc( sizeof(struct Graph) );
        graph->V = V;
        graph->E = E;
        
        graph->edge = (struct Edge*) malloc( graph->E * sizeof( struct Edge ) );
        
        return graph;
    }
    
    struct subset
    {
        int parent;
        int rank;
    };
    
    int find(struct subset subsets[], int i)
    {
        if (subsets[i].parent != i)
            subsets[i].parent = find(subsets, subsets[i].parent);
        
        return subsets[i].parent;
    }
    
    void Union(struct subset subsets[], int x, int y)
    {
        int xroot = find(subsets, x);
        int yroot = find(subsets, y);
        
        if (subsets[xroot].rank < subsets[yroot].rank)
            subsets[xroot].parent = yroot;
        else if (subsets[xroot].rank > subsets[yroot].rank)
            subsets[yroot].parent = xroot;
        
        else
        {
            subsets[yroot].parent = xroot;
            subsets[xroot].rank++;
        }
    }
    
    static int edgeComp(const void* a, const void* b)
    {
        Edge* a1 = (Edge*)a;
        Edge* b1 = (Edge*)b;
        if (a1->weight > b1->weight) return 1;
        if (a1->weight == b1->weight) return 0;
        if (a1->weight < b1->weight) return -1;
    }
    
    
    
    int AREA_THRESHOLD = 60;
    
    

public :

    int width, height;

    Map(int width, int height);
    ~Map();
    
    ofFile f;
    
    int mapNumber = 1;


    int currentMaterial = (MaterialType)0;
    
    ofRectangle window = ofRectangle(0,0,80,50);

    void setWindowSeen(int x, int y);
    void setSeen(int x, int y);
    void setMat(int x, int y, Material * m );
    int getLastSeen(int x, int y);
    int getWindowLastSeen(int x, int y);

    DEBT traversable(int x, int y);
    float isWindowOpaque(int x, int y);

    void render();
    void setWindow(int x, int y);

    void placeMaterial(int x, int y, MaterialType mt);
    
    void save();
    void load();

    void drawRoom(ofRectangle r);
    void drawLine(ofVec2f p0, ofVec2f p1);

    Tile * getWindowTile(int x, int y);

    void generateMap();
    void makeRooms();
    void upkeep();

    ofVec2f startingPosition;
    
    void cycle(int v);


    vector<ofRectangle> rooms;
    
    ofxDelaunay triangulation;
    
    bool madeGraph = false;
    void KruskalMST(struct Graph* graph);
    
    vector<Edge> MSTedges;
    vector<Edge> edges;
    
    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }


protected :
    Tile *tiles;
    void setWall(int x, int y);
};



#endif /* Map_h */
