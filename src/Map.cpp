//
//  Map.cpp
//  emptyExample
//
//  Created by vesabios on 10/5/15.
//
//

#include "Map.h"
#include "Core.h"

//--------------------------------------------------------------
Map::Map(int width, int height) : width(width), height(height) {
    tiles=new Tile[width*height];
    
    startingPosition = ofVec2f(255,255);
    
    load();

    
    /*
    
    makeRooms();
    generateMap();
    
    for (int i=0; i<1000; i++) {
        
        int x = ofRandom(width);
        int y = ofRandom(height);
        
        tiles[x+y*width].canWalk = false;
        tiles[x+y*width].mat = mats[MStoneWall];
        
        
    }
     
     */
    
    
}

//--------------------------------------------------------------
Map::~Map() {
    
    delete [] tiles;
}


//--------------------------------------------------------------
void Map::KruskalMST(struct Graph* graph)
{
    
    int V = graph->V;
    struct Edge result[V];
    int e = 0;
    int i = 0;
    qsort(graph->edge, graph->E, sizeof(graph->edge[0]), Map::edgeComp);
    
    struct subset *subsets = (struct subset*) malloc( V * sizeof(struct subset) );
    
    for (int v = 0; v < V; ++v)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }
    
    while (e < V - 1)
    {
        struct Edge next_edge = graph->edge[i++];
        
        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);
        
        if (x != y)
        {
            result[e++] = next_edge;
            Union(subsets, x, y);
        }
    }
    
    
    MSTedges.clear();
    
    for (i = 0; i < e; ++i) {
        MSTedges.push_back(result[i]);
    }
    
    free(subsets);
    free(graph->edge);
    free(graph);
    
    return;
}



//--------------------------------------------------------------
void Map::makeRooms() {
    
    rooms.clear();
    
    for (int i=0; i<120; i++) {
        
        ofRectangle r = ofRectangle(int(ofRandom(100)+200),
                                    int(ofRandom(100)+200),
                                    int(ofRandom(8)+3),
                                    int(ofRandom(8)+3));
        
        rooms.push_back(r);
    }
    
}

//--------------------------------------------------------------
void Map::generateMap() {
    
    bool hitting = true;
    
    int failsafe = 0;
    
    while (hitting) {
        
        failsafe++;
        
        hitting = false;
        
        for (int i=0; i<rooms.size(); i++) {
            
            ofRectangle r = rooms[i];
            ofVec2f v = ofVec2f(0,0);
            int n = 0;
            
            for (int j=0; j<rooms.size(); j++) {
                
                if (j!=i) {
                    
                    ofRectangle s = rooms[j];
                    
                    if(r.intersects(s)) {
                        
                        if (failsafe<1000) {
                            hitting = true;
                        }
                        n++;
                        v+= r.getCenter()-s.getCenter();
                        
                    }
                }
            }
            
            if (abs(v.x)>abs(v.y)) {
                rooms[i].translate(ofVec2f(sgn(v.x),0));
            } else {
                rooms[i].translate(ofVec2f(0,sgn(v.y)));
            }
            
        }
        
    }
    
    
    edges.clear();
    triangulation.reset();
    
    for (int i=0; i<rooms.size(); i++) {
        
       if (rooms[i].getArea()>AREA_THRESHOLD) {
            
            //if (ofRandom(100)>93) {
                triangulation.addPoint(rooms[i].getCenter());
                
                startingPosition = rooms[i].getCenter();
                startingPosition.x = int(startingPosition.x);
                startingPosition.y = int(startingPosition.y);
                
                
            //}

        }
        
    }
    
    triangulation.triangulate();
    triangulation.triangleMesh.mergeDuplicateVertices();
    
    vector<ofIndexType> indices = triangulation.triangleMesh.getIndices();
    vector<ofVec3f> vertices = triangulation.triangleMesh.getVertices();
    
    int n = triangulation.getNumTriangles();
    
    for (int i=0; i<n; i++) {
        
        int p0 = indices[i*3];
        int p1 = indices[i*3+1];
        int p2 = indices[i*3+2];
        
        
        {
            Edge e;
            if (p0<p1) {
                e.src = p0;
                e.dest = p1;
            } else {
                e.src = p1;
                e.dest = p0;
            }
            
            e.weight = int(vertices[p0].distance(vertices[p1]));
            
            if (find_if(edges.begin(), edges.end(), find_edge(e)) == edges.end()) {
                edges.push_back(e);
            }
        }
        
        
        {
            Edge e;
            
            if (p1<p2) {
                e.src = p1;
                e.dest = p2;
            } else {
                e.src = p2;
                e.dest = p1;
            }
            
            e.weight = int(vertices[p1].distance(vertices[p2]));
            if (find_if(edges.begin(), edges.end(), find_edge(e)) == edges.end()) {
                edges.push_back(e);
            }
        }
        
        {
            Edge e;
            if (p2<p0) {
                e.src = p2;
                e.dest = p0;
            } else {
                e.src = p0;
                e.dest = p2;
            }
            
            e.weight = int(vertices[p2].distance(vertices[p0]));
            if (find_if(edges.begin(), edges.end(), find_edge(e)) == edges.end()) {
                edges.push_back(e);
            }
        }
        
    }
    
    
    struct Graph* graph = createGraph(vertices.size(), edges.size());
    
    for (int i=0; i<edges.size(); i++) {
        graph->edge[i].src = edges[i].src;
        graph->edge[i].dest = edges[i].dest;
        graph->edge[i].weight = edges[i].weight;
        
    }
    
    KruskalMST(graph);
    
    
    // now to render our geometry into the actual map
    
    for (int i=0; i<rooms.size(); i++) {
        if (rooms[i].getArea()>AREA_THRESHOLD) {
            drawRoom(rooms[i]);
        }
    }
    
    // render connecting corridors...
    for (int i=0; i<MSTedges.size(); i++) {
        
        Edge e = MSTedges[i];
        
        ofVec2f v0 = vertices[e.src];
        ofVec2f v1 = vertices[e.dest];
        
        ofVec2f delta = v1-v0;
        
        if (abs(delta.x)<abs(delta.y)) {
            drawLine(v0, v0+(delta * ofVec2f(1,0)));
            drawLine(v0+(delta * ofVec2f(1,0)), v1);
        } else {
            drawLine(v0, v0+(delta * ofVec2f(0,1)));
            drawLine(v0+(delta * ofVec2f(0,1)), v1);
        }
        
    }
    
    
}



void Map::cycle(int v) {
    
    if (v>0) {
        if (mapNumber<256) {
            mapNumber++;
            load();
            core->player->z = mapNumber;
        }
    } else if (v<0) {
        if (mapNumber>0) {
            mapNumber+=v;
            load();
            core->player->z = mapNumber;
            
        }
        
    }
    
}


void Map::save() {
    
    char buf[256];
    sprintf(buf, "map%d.bin", mapNumber);
    
    f.open(buf, ofFile::WriteOnly, true);
    
    for(int i = 0; i < height*width; i++) {
        
        f.write((const char *)&tiles[i].mat->mt, sizeof(unsigned char));

    }
    
    f.close();
    
}


void Map::load() {
    
    char buf[256];
    sprintf(buf, "map%d.bin", mapNumber);
    f.open(buf, ofFile::ReadOnly, true);
    if (f.exists()) {
        ofBuffer buffer(f);
        for (int i = 0; i < width*height; i++) {
            tiles[i].mat = materials.mats[buffer.getBinaryBuffer()[i]];
        }
    } else {
        for (int i=0; i<width*height; i++ ) {
            tiles[i].mat = materials.mats[Material::Void];
        }
    }
    
}


//--------------------------------------------------------------
void Map::drawRoom(ofRectangle r) {
    
    for (int x=r.getMinX(); x<r.getMaxX(); x++) {
        for (int y=r.getMinY(); y<r.getMaxY(); y++) {
            tiles[x+y*width].canWalk = true;
            tiles[x+y*width].mat = materials.mats[Material::StoneFloor];
        }
    }
    
}


//--------------------------------------------------------------
void Map::drawLine(ofVec2f p0, ofVec2f p1) {
    
    if (p0.x==p1.x) {
        if (p0.y<p1.y) {
            for (int i=p0.y; i<=p1.y; i++) {
                tiles[(int)p0.x+i*width].canWalk = true;
                tiles[(int)p0.x+i*width].mat = materials.mats[Material::StoneFloor];
            }
        } else {
            for (int i=p1.y; i<=p0.y; i++) {
                tiles[(int)p0.x+i*width].canWalk = true;
                tiles[(int)p0.x+i*width].mat = materials.mats[Material::StoneFloor];

            }
        }
    } else if (p0.y==p1.y) {
        if (p0.x<p1.x) {
            for (int i=p0.x; i<=p1.x; i++) {
                tiles[i+(int)p0.y*width].canWalk = true;
                tiles[i+(int)p0.y*width].mat = materials.mats[Material::StoneFloor];

            }
        } else {
            for (int i=p1.x; i<=p0.x; i++) {
                tiles[i+(int)p0.y*width].canWalk = true;
                tiles[i+(int)p0.y*width].mat = materials.mats[Material::StoneFloor];
                
            }
        }
    }
    
 
    
}


//--------------------------------------------------------------
void Map::placeMaterial(int x, int y, Material::MaterialType mt) {
    
    if (x>=0 && x<width && y>=0 && y<width) {
        Tile * t = &tiles[x+y*width];
        t->mat = materials.mats[currentMaterial];
        
    }
    
    
}

//--------------------------------------------------------------
DEBT Map::traversable(int x, int y) {
    
    Tile * t = &tiles[x+y*width];
    if (t!=NULL) {
        return t->mat->traversable();
    }
    return 1;
}


//--------------------------------------------------------------
float Map::isWindowOpaque(int x, int y) {

    //return !tiles[(x+(int)window.getMinX())+(y+(int)window.getMinY())*width].mat->isOpaque();

    
    Tile * t = getWindowTile(x,y);
    if (t!=NULL) {
        if (t->mat==NULL) {
            return 1.0f;
        } else {
            return t->mat->isOpaque();
        }
    }
    return 1.0f;
}

Tile * Map::getWindowTile(int x, int y) {
    return &tiles[(x+(int)window.getMinX())+(y+(int)window.getMinY())*width];
}


//--------------------------------------------------------------
void Map::setWall(int x, int y) {
    tiles[x+y*width].canWalk=false;
}

void Map::setWindow(int x, int y) {
    
    x = MAX(0,MIN(x, width - window.width));
    y = MAX(0,MIN(y, height - window.height));
 
    window.setPosition(x,y);
}

void Map::setSeen(int x, int y) {
    tiles[x+y*width].lastSeen=0;
}

void Map::setMat(int x, int y, Material * m ) {
    tiles[x+y*width].mat = m;
}

void Map::setWindowSeen(int x, int y) {
    setSeen(x+window.getMinX(), y+window.getMinY());

}

int Map::getLastSeen(int x, int y) {
    return tiles[x+y*width].lastSeen;
}

int Map::getWindowLastSeen(int x, int y) {
    return getLastSeen(x+window.getMinX(), y+window.getMinY());
}


void Map::upkeep() {
    for (int x=0; x < width; x++) {
        for (int y=0; y < height; y++) {
            tiles[x+y*width].lastSeen++;
        }
    }
}

//--------------------------------------------------------------
void Map::render() {

    for (int x=window.getMinX(); x < window.getMaxX(); x++) {
        for (int y=window.getMinY(); y < window.getMaxY(); y++) {

            // view space from world space
            int rx = x-window.getMinX();
            int ry = y-window.getMinY();
            
            if (traversable(x,y)>0) {
                ofRect(rx*8,ry*8,8,8);
                // render wall pixel
            } else {
                // render floor pixel
            }
        }
    }

}