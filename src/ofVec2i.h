#pragma once

#include "ofConstants.h"

class ofVec2i {
public:
    int x, y;
    
    ofVec2i( int _x=0, int _y=0);
    ofVec2i( const ofVec2f& vec);
    
    // Getters and Setters.
    //
    void set( int _x, int _y );
    void set( const ofVec2i& vec );
    void set( const ofVec2f& vec );
    
    int * getPtr() {
        return (int*)&x;
    }
    const int * getPtr() const {
        return (const int *)&x;
    }
    
    int& operator[]( int n ){
        return getPtr()[n];
    }
    
    int operator[]( int n ) const {
        return getPtr()[n];
    }
    
    
    // Check similarity/equality.
    //

    ofVec2i operator=( const ofVec2f& vec ) const;
    bool operator==( const ofVec2i& vec ) const;
    bool operator!=( const ofVec2i& vec ) const;
    bool match( const ofVec2i& vec, int tollerance=0.0001) const;
    
    
    // Additions and Subtractions.
    //
    ofVec2i  operator+( const ofVec2i& vec ) const;
    ofVec2i& operator+=( const ofVec2i& vec );
    ofVec2i  operator-( const int f ) const;
    ofVec2i& operator-=( const int f );
    ofVec2i  operator-( const ofVec2i& vec ) const;
    ofVec2i& operator-=( const ofVec2i& vec );
    ofVec2i  operator+( const int f ) const;
    ofVec2i& operator+=( const int f );
    ofVec2i  operator-() const;
    
    
    // Scalings
    //
    ofVec2i  operator*( const ofVec2i& vec ) const;
    ofVec2i& operator*=( const ofVec2i& vec );
    ofVec2i  operator*( const int f ) const;
    ofVec2i& operator*=( const int f );
    ofVec2i  operator/( const ofVec2i& vec ) const;
    ofVec2i& operator/=( const ofVec2i& vec );
    ofVec2i  operator/( const int f ) const;
    ofVec2i& operator/=( const int f );
    
    friend ostream& operator<<(ostream& os, const ofVec2i& vec);
    friend istream& operator>>(istream& is, const ofVec2i& vec);
    
    
    ofVec2i  getScaled( const int length ) const;
    ofVec2i& scale( const int length );
    
    
    // Distance between two points.
    //
    int distance( const ofVec2i& pnt) const;
    int squareDistance( const ofVec2i& pnt ) const;
    
    
    // Linear interpolation.
    //
    /**
     * p==0.0 results in this point, p==0.5 results in the
     * midpoint, and p==1.0 results in pnt being returned.
     */
    ofVec2i   getInterpolated( const ofVec2i& pnt, int p ) const;
    ofVec2i&  interpolate( const ofVec2i& pnt, int p );
    ofVec2i   getMiddle( const ofVec2i& pnt ) const;
    ofVec2i&  middle( const ofVec2i& pnt );
    ofVec2i&  average( const ofVec2i* points, int num );
    
    
    // Normalization
    //
    ofVec2i  getNormalized() const;
    ofVec2i& normalize();
    
    
    // Limit length.
    //
    ofVec2i  getLimited(int max) const;
    ofVec2i& limit(int max);
    
    
    // Length
    //
    int length() const;
    int squareLength() const;
    /**
     * Dot Product.
     */
    int dot( const ofVec2i& vec ) const;
    
    
    
    
    //---------------------------------------
    // this methods are deprecated in 006 please use:
    
    // getScaled
    ofVec2i rescaled( const int length ) const;
    
    // scale
    ofVec2i& rescale( const int length );
    
    // getNormalized
    ofVec2i normalized() const;
    
    // getLimited
    ofVec2i limited(int max) const;
    
    // squareLength
    int lengthSquared() const;
    
    // use squareDistance
    int  distanceSquared( const ofVec2i& pnt ) const;
    
    // use getInterpolated
    ofVec2i 	interpolated( const ofVec2i& pnt, int p ) const;
    
    // use getMiddle
    ofVec2i 	middled( const ofVec2i& pnt ) const;
};




// Non-Member operators
//
//
ofVec2i operator+( int f, const ofVec2i& vec );
ofVec2i operator-( int f, const ofVec2i& vec );
ofVec2i operator*( int f, const ofVec2i& vec );
ofVec2i operator/( int f, const ofVec2i& vec );







/////////////////
// Implementation
/////////////////

inline ofVec2i::ofVec2i( int _x,
                          int _y):x(_x), y(_y) {}


inline ofVec2i::ofVec2i( const ofVec2f& vec):x((int)vec.x), y((int)vec.y) {}



// Getters and Setters.
//
//
inline void ofVec2i::set( int _x, int _y ) {
    x = _x;
    y = _y;

}

inline void ofVec2i::set( const ofVec2i& vec ) {
    x = vec.x;
    y = vec.y;

}

inline void ofVec2i::set( const ofVec2f& vec ) {
    x = vec.x;
    y = vec.y;
    
}


// Check similarity/equality.
//
//

inline ofVec2i ofVec2i::operator=( const ofVec2f& vec ) const {
    return ofVec2i(vec.x, vec.y);
}

inline bool ofVec2i::operator==( const ofVec2i& vec ) const {
    return (x == vec.x) && (y == vec.y) ;
}

inline bool ofVec2i::operator!=( const ofVec2i& vec ) const {
    return (x != vec.x) || (y != vec.y) ;
}

inline bool ofVec2i::match( const ofVec2i& vec, int tollerance) const {
    return (fabs(x - vec.x) < tollerance)
    && (fabs(y - vec.y) < tollerance);
}




// Additions and Subtractions.
//
//
inline ofVec2i ofVec2i::operator+( const ofVec2i& vec ) const {
    return ofVec2i( x+vec.x, y+vec.y);
}

inline ofVec2i& ofVec2i::operator+=( const ofVec2i& vec ) {
    x += vec.x;
    y += vec.y;

    return *this;
}

inline ofVec2i ofVec2i::operator-( const int f ) const {
    return ofVec2i( x-f, y-f );
}

inline ofVec2i& ofVec2i::operator-=( const int f ) {
    x -= f;
    y -= f;
    return *this;
}

inline ofVec2i ofVec2i::operator-( const ofVec2i& vec ) const {
    return ofVec2i( x-vec.x, y-vec.y);
}

inline ofVec2i& ofVec2i::operator-=( const ofVec2i& vec ) {
    x -= vec.x;
    y -= vec.y;

    return *this;
}

inline ofVec2i ofVec2i::operator+( const int f ) const {
    return ofVec2i( x+f, y+f );
}

inline ofVec2i& ofVec2i::operator+=( const int f ) {
    x += f;
    y += f;

    return *this;
}

inline ofVec2i ofVec2i::operator-() const {
    return ofVec2i( -x, -y );
}


// Scalings
//
//
inline ofVec2i ofVec2i::operator*( const ofVec2i& vec ) const {
    return ofVec2i( x*vec.x, y*vec.y );
}

inline ofVec2i& ofVec2i::operator*=( const ofVec2i& vec ) {
    x *= vec.x;
    y *= vec.y;

    return *this;
}

inline ofVec2i ofVec2i::operator*( const int f ) const {
    return ofVec2i( x*f, y*f );
}

inline ofVec2i& ofVec2i::operator*=( const int f ) {
    x *= f;
    y *= f;

    return *this;
}

inline ofVec2i ofVec2i::operator/( const ofVec2i& vec ) const {
    return ofVec2i( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y );
}

inline ofVec2i& ofVec2i::operator/=( const ofVec2i& vec ) {
    vec.x!=0 ? x/=vec.x : x;
    vec.y!=0 ? y/=vec.y : y;

    return *this;
}

inline ofVec2i ofVec2i::operator/( const int f ) const {
    if(f == 0) return ofVec2i(x, y);
    
    return ofVec2i( x/f, y/f );
}

inline ofVec2i& ofVec2i::operator/=( const int f ) {
    if(f == 0)return *this;
    
    x /= f;
    y /= f;

    return *this;
}


inline ostream& operator<<(ostream& os, const ofVec2i& vec) {
    os << vec.x << ", " << vec.y ;
    return os;
}

inline istream& operator>>(istream& is, ofVec2i& vec) {
    is >> vec.x;
    is.ignore(2);
    is >> vec.y;
    return is;
}


inline ofVec2i ofVec2i::rescaled( const int length ) const {
    return getScaled(length);
}

inline ofVec2i ofVec2i::getScaled( const int length ) const {
    int l = (int)sqrt(x*x + y*y );
    if( l > 0 )
        return ofVec2i( (x/l)*length, (y/l)*length
                       );
    else
        return ofVec2i();
}

inline ofVec2i& ofVec2i::rescale( const int length ) {
    return scale(length);
}

inline ofVec2i& ofVec2i::scale( const int length ) {
    int l = (int)sqrt(x*x + y*y);
    if (l > 0) {
        x = (x/l)*length;
        y = (y/l)*length;

    }
    return *this;
}



// Distance between two points.
//
//
inline int ofVec2i::distance( const ofVec2i& pnt) const {
    int vx = x-pnt.x;
    int vy = y-pnt.y;

    return (int)sqrt( vx*vx + vy*vy );
}

inline int ofVec2i::distanceSquared( const ofVec2i& pnt ) const {
    return squareDistance(pnt);
}

inline int ofVec2i::squareDistance( const ofVec2i& pnt ) const {
    int vx = x-pnt.x;
    int vy = y-pnt.y;

    return vx*vx + vy*vy ;
}



// Linear interpolation.
//
//
/**
 * p==0.0 results in this point, p==0.5 results in the
 * midpoint, and p==1.0 results in pnt being returned.
 */
inline ofVec2i ofVec2i::interpolated( const ofVec2i& pnt, int p ) const{
    return getInterpolated(pnt,p);
}

inline ofVec2i ofVec2i::getInterpolated( const ofVec2i& pnt, int p ) const {
    return ofVec2i( x*(1-p) + pnt.x*p,
                    y*(1-p) + pnt.y*p
);
}

inline ofVec2i& ofVec2i::interpolate( const ofVec2i& pnt, int p ) {
    x = x*(1-p) + pnt.x*p;
    y = y*(1-p) + pnt.y*p;

    return *this;
}

inline ofVec2i ofVec2i::middled( const ofVec2i& pnt ) const {
    return getMiddle(pnt);
}

inline ofVec2i ofVec2i::getMiddle( const ofVec2i& pnt ) const {
    return ofVec2i( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f );
}

inline ofVec2i& ofVec2i::middle( const ofVec2i& pnt ) {
    x = (x+pnt.x)/2.0f;
    y = (y+pnt.y)/2.0f;

    return *this;
}


// Average (centroid) among points.
// (Addition is sometimes useful for calculating averages too)
//
//
inline ofVec2i& ofVec2i::average( const ofVec2i* points, int num ) {
    x = 0.f;
    y = 0.f;

    for( int i=0; i<num; i++) {
        x += points[i].x;
        y += points[i].y;

    }
    x /= num;
    y /= num;

    return *this;
}



// Normalization
//
//
inline ofVec2i ofVec2i::normalized() const {
    return getNormalized();
}

inline ofVec2i ofVec2i::getNormalized() const {
    int length = (int)sqrt(x*x + y*y );
    if( length > 0 ) {
        return ofVec2i( x/length, y/length );
    } else {
        return ofVec2i();
    }
}

inline ofVec2i& ofVec2i::normalize() {
    int lenght = (int)sqrt(x*x + y*y);
    if( lenght > 0 ) {
        x /= lenght;
        y /= lenght;

    }
    return *this;
}



// Limit length.
//
//
inline ofVec2i ofVec2i::limited(int max) const {
    return getLimited(max);
}

inline ofVec2i ofVec2i::getLimited(int max) const {
    ofVec2i limited;
    int lengthSquared = (x*x + y*y );
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        int ratio = max/(int)sqrt(lengthSquared);
        limited.set( x*ratio, y*ratio );
    } else {
        limited.set(x,y);
    }
    return limited;
}

inline ofVec2i& ofVec2i::limit(int max) {
    int lengthSquared = (x*x + y*y );
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        int ratio = max/(int)sqrt(lengthSquared);
        x *= ratio;
        y *= ratio;

    }
    return *this;
}



// Length
//
//
inline int ofVec2i::length() const {
    return (int)sqrt( x*x + y*y  );
}

inline int ofVec2i::lengthSquared() const {
    return squareLength();
}

inline int ofVec2i::squareLength() const {
    return (int)(x*x + y*y );
}



/**
 * Dot Product.
 */
inline int ofVec2i::dot( const ofVec2i& vec ) const {
    return x*vec.x + y*vec.y ;
}





// Non-Member operators
//
//
inline ofVec2i operator+( int f, const ofVec2i& vec ) {
    return ofVec2i( f+vec.x, f+vec.y );
}

inline ofVec2i operator-( int f, const ofVec2i& vec ) {
    return ofVec2i( f-vec.x, f-vec.y);
}

inline ofVec2i operator*( int f, const ofVec2i& vec ) {
    return ofVec2i( f*vec.x, f*vec.y );
}

inline ofVec2i operator/( int f, const ofVec2i& vec ) {
    return ofVec2i( f/vec.x, f/vec.y);
}