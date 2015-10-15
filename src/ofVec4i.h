#pragma once

#include "ofConstants.h"

class ofVec4i {
public:
    int x, y, z, w;
    
    ofVec4i( int _x=0, int _y=0, int _z=0, int _w=0 );
    
    // Getters and Setters.
    //
    void set( int _x, int _y, int _z, int _w );
    void set( const ofVec4i& vec );
    
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
    bool operator==( const ofVec4i& vec ) const;
    bool operator!=( const ofVec4i& vec ) const;
    bool match( const ofVec4i& vec, int tollerance=0.0001) const;
    
    
    // Additions and Subtractions.
    //
    ofVec4i  operator+( const ofVec4i& vec ) const;
    ofVec4i& operator+=( const ofVec4i& vec );
    ofVec4i  operator-( const int f ) const;
    ofVec4i& operator-=( const int f );
    ofVec4i  operator-( const ofVec4i& vec ) const;
    ofVec4i& operator-=( const ofVec4i& vec );
    ofVec4i  operator+( const int f ) const;
    ofVec4i& operator+=( const int f );
    ofVec4i  operator-() const;
    
    
    // Scalings
    //
    ofVec4i  operator*( const ofVec4i& vec ) const;
    ofVec4i& operator*=( const ofVec4i& vec );
    ofVec4i  operator*( const int f ) const;
    ofVec4i& operator*=( const int f );
    ofVec4i  operator/( const ofVec4i& vec ) const;
    ofVec4i& operator/=( const ofVec4i& vec );
    ofVec4i  operator/( const int f ) const;
    ofVec4i& operator/=( const int f );
    
    friend ostream& operator<<(ostream& os, const ofVec4i& vec);
    friend istream& operator>>(istream& is, const ofVec4i& vec);
    
    
    ofVec4i  getScaled( const int length ) const;
    ofVec4i& scale( const int length );
    
    
    // Distance between two points.
    //
    int distance( const ofVec4i& pnt) const;
    int squareDistance( const ofVec4i& pnt ) const;
    
    
    // Linear interpolation.
    //
    /**
     * p==0.0 results in this point, p==0.5 results in the
     * midpoint, and p==1.0 results in pnt being returned.
     */
    ofVec4i   getInterpolated( const ofVec4i& pnt, int p ) const;
    ofVec4i&  interpolate( const ofVec4i& pnt, int p );
    ofVec4i   getMiddle( const ofVec4i& pnt ) const;
    ofVec4i&  middle( const ofVec4i& pnt );
    ofVec4i&  average( const ofVec4i* points, int num );
    
    
    // Normalization
    //
    ofVec4i  getNormalized() const;
    ofVec4i& normalize();
    
    
    // Limit length.
    //
    ofVec4i  getLimited(int max) const;
    ofVec4i& limit(int max);
    
    
    // Length
    //
    int length() const;
    int squareLength() const;
    /**
     * Dot Product.
     */
    int dot( const ofVec4i& vec ) const;
    
    
    
    
    //---------------------------------------
    // this methods are deprecated in 006 please use:
    
    // getScaled
    ofVec4i rescaled( const int length ) const;
    
    // scale
    ofVec4i& rescale( const int length );
    
    // getNormalized
    ofVec4i normalized() const;
    
    // getLimited
    ofVec4i limited(int max) const;
    
    // squareLength
    int lengthSquared() const;
    
    // use squareDistance
    int  distanceSquared( const ofVec4i& pnt ) const;
    
    // use getInterpolated
    ofVec4i 	interpolated( const ofVec4i& pnt, int p ) const;
    
    // use getMiddle
    ofVec4i 	middled( const ofVec4i& pnt ) const;
};




// Non-Member operators
//
//
ofVec4i operator+( int f, const ofVec4i& vec );
ofVec4i operator-( int f, const ofVec4i& vec );
ofVec4i operator*( int f, const ofVec4i& vec );
ofVec4i operator/( int f, const ofVec4i& vec );







/////////////////
// Implementation
/////////////////

inline ofVec4i::ofVec4i( int _x,
                          int _y,
                          int _z,
                          int _w ):x(_x), y(_y), z(_z), w(_w) {}

// Getters and Setters.
//
//
inline void ofVec4i::set( int _x, int _y, int _z, int _w ) {
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

inline void ofVec4i::set( const ofVec4i& vec ) {
    x = vec.x;
    y = vec.y;
    z = vec.z;
    w = vec.w;
}


// Check similarity/equality.
//
//
inline bool ofVec4i::operator==( const ofVec4i& vec ) const {
    return (x == vec.x) && (y == vec.y) && (z == vec.z) && (w == vec.w);
}

inline bool ofVec4i::operator!=( const ofVec4i& vec ) const {
    return (x != vec.x) || (y != vec.y) || (z != vec.z) || (w != vec.w);
}

inline bool ofVec4i::match( const ofVec4i& vec, int tollerance) const {
    return (fabs(x - vec.x) < tollerance)
    && (fabs(y - vec.y) < tollerance)
    && (fabs(z - vec.z) < tollerance)
    && (fabs(w - vec.w) < tollerance);
}




// Additions and Subtractions.
//
//
inline ofVec4i ofVec4i::operator+( const ofVec4i& vec ) const {
    return ofVec4i( x+vec.x, y+vec.y, z+vec.z, w+vec.w);
}

inline ofVec4i& ofVec4i::operator+=( const ofVec4i& vec ) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    w += vec.w;
    return *this;
}

inline ofVec4i ofVec4i::operator-( const int f ) const {
    return ofVec4i( x-f, y-f, z-f, w-f );
}

inline ofVec4i& ofVec4i::operator-=( const int f ) {
    x -= f;
    y -= f;
    z -= f;
    w -= f;
    return *this;
}

inline ofVec4i ofVec4i::operator-( const ofVec4i& vec ) const {
    return ofVec4i( x-vec.x, y-vec.y, z-vec.z, w-vec.w );
}

inline ofVec4i& ofVec4i::operator-=( const ofVec4i& vec ) {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    w -= vec.w;
    return *this;
}

inline ofVec4i ofVec4i::operator+( const int f ) const {
    return ofVec4i( x+f, y+f, z+f, w+f );
}

inline ofVec4i& ofVec4i::operator+=( const int f ) {
    x += f;
    y += f;
    z += f;
    w += f;
    return *this;
}

inline ofVec4i ofVec4i::operator-() const {
    return ofVec4i( -x, -y, -z, -w );
}


// Scalings
//
//
inline ofVec4i ofVec4i::operator*( const ofVec4i& vec ) const {
    return ofVec4i( x*vec.x, y*vec.y, z*vec.z, w*vec.w );
}

inline ofVec4i& ofVec4i::operator*=( const ofVec4i& vec ) {
    x *= vec.x;
    y *= vec.y;
    z *= vec.z;
    w *= vec.w;
    return *this;
}

inline ofVec4i ofVec4i::operator*( const int f ) const {
    return ofVec4i( x*f, y*f, z*f, w*f );
}

inline ofVec4i& ofVec4i::operator*=( const int f ) {
    x *= f;
    y *= f;
    z *= f;
    w *= f;
    return *this;
}

inline ofVec4i ofVec4i::operator/( const ofVec4i& vec ) const {
    return ofVec4i( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y, vec.z!=0 ? z/vec.z : z, vec.w!=0 ? w/vec.w : w  );
}

inline ofVec4i& ofVec4i::operator/=( const ofVec4i& vec ) {
    vec.x!=0 ? x/=vec.x : x;
    vec.y!=0 ? y/=vec.y : y;
    vec.z!=0 ? z/=vec.z : z;
    vec.w!=0 ? w/=vec.w : w;
    return *this;
}

inline ofVec4i ofVec4i::operator/( const int f ) const {
    if(f == 0) return ofVec4i(x, y, z, w);
    
    return ofVec4i( x/f, y/f, z/f, w/f );
}

inline ofVec4i& ofVec4i::operator/=( const int f ) {
    if(f == 0)return *this;
    
    x /= f;
    y /= f;
    z /= f;
    w /= f;
    return *this;
}


inline ostream& operator<<(ostream& os, const ofVec4i& vec) {
    os << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
    return os;
}

inline istream& operator>>(istream& is, ofVec4i& vec) {
    is >> vec.x;
    is.ignore(2);
    is >> vec.y;
    is.ignore(2);
    is >> vec.z;
    is.ignore(2);
    is >> vec.w;
    return is;
}


inline ofVec4i ofVec4i::rescaled( const int length ) const {
    return getScaled(length);
}

inline ofVec4i ofVec4i::getScaled( const int length ) const {
    int l = (int)sqrt(x*x + y*y + z*z + w*w);
    if( l > 0 )
        return ofVec4i( (x/l)*length, (y/l)*length,
                        (z/l)*length, (w/l)*length );
    else
        return ofVec4i();
}

inline ofVec4i& ofVec4i::rescale( const int length ) {
    return scale(length);
}

inline ofVec4i& ofVec4i::scale( const int length ) {
    int l = (int)sqrt(x*x + y*y + z*z + w*w);
    if (l > 0) {
        x = (x/l)*length;
        y = (y/l)*length;
        z = (z/l)*length;
        w = (w/l)*length;
    }
    return *this;
}



// Distance between two points.
//
//
inline int ofVec4i::distance( const ofVec4i& pnt) const {
    int vx = x-pnt.x;
    int vy = y-pnt.y;
    int vz = z-pnt.z;
    int vw = w-pnt.w;
    return (int)sqrt( vx*vx + vy*vy + vz*vz + vw*vw );
}

inline int ofVec4i::distanceSquared( const ofVec4i& pnt ) const {
    return squareDistance(pnt);
}

inline int ofVec4i::squareDistance( const ofVec4i& pnt ) const {
    int vx = x-pnt.x;
    int vy = y-pnt.y;
    int vz = z-pnt.z;
    int vw = w-pnt.w;
    return vx*vx + vy*vy + vz*vz + vw*vw;
}



// Linear interpolation.
//
//
/**
 * p==0.0 results in this point, p==0.5 results in the
 * midpoint, and p==1.0 results in pnt being returned.
 */
inline ofVec4i ofVec4i::interpolated( const ofVec4i& pnt, int p ) const{
    return getInterpolated(pnt,p);
}

inline ofVec4i ofVec4i::getInterpolated( const ofVec4i& pnt, int p ) const {
    return ofVec4i( x*(1-p) + pnt.x*p,
                    y*(1-p) + pnt.y*p,
                    z*(1-p) + pnt.z*p,
                    w*(1-p) + pnt.w*p );
}

inline ofVec4i& ofVec4i::interpolate( const ofVec4i& pnt, int p ) {
    x = x*(1-p) + pnt.x*p;
    y = y*(1-p) + pnt.y*p;
    z = z*(1-p) + pnt.z*p;
    w = w*(1-p) + pnt.w*p;
    return *this;
}

inline ofVec4i ofVec4i::middled( const ofVec4i& pnt ) const {
    return getMiddle(pnt);
}

inline ofVec4i ofVec4i::getMiddle( const ofVec4i& pnt ) const {
    return ofVec4i( (x+pnt.x)/2.0f, (y+pnt.y)/2.0f,
                    (z+pnt.z)/2.0f, (w+pnt.w)/2.0f );
}

inline ofVec4i& ofVec4i::middle( const ofVec4i& pnt ) {
    x = (x+pnt.x)/2.0f;
    y = (y+pnt.y)/2.0f;
    z = (z+pnt.z)/2.0f;
    w = (w+pnt.w)/2.0f;
    return *this;
}


// Average (centroid) among points.
// (Addition is sometimes useful for calculating averages too)
//
//
inline ofVec4i& ofVec4i::average( const ofVec4i* points, int num ) {
    x = 0.f;
    y = 0.f;
    z = 0.f;
    w = 0.f;
    for( int i=0; i<num; i++) {
        x += points[i].x;
        y += points[i].y;
        z += points[i].z;
        w += points[i].w;
    }
    x /= num;
    y /= num;
    z /= num;
    w /= num;
    return *this;
}



// Normalization
//
//
inline ofVec4i ofVec4i::normalized() const {
    return getNormalized();
}

inline ofVec4i ofVec4i::getNormalized() const {
    int length = (int)sqrt(x*x + y*y + z*z + w*w);
    if( length > 0 ) {
        return ofVec4i( x/length, y/length, z/length, w/length );
    } else {
        return ofVec4i();
    }
}

inline ofVec4i& ofVec4i::normalize() {
    int lenght = (int)sqrt(x*x + y*y + z*z + w*w);
    if( lenght > 0 ) {
        x /= lenght;
        y /= lenght;
        z /= lenght;
        w /= lenght;
    }
    return *this;
}



// Limit length.
//
//
inline ofVec4i ofVec4i::limited(int max) const {
    return getLimited(max);
}

inline ofVec4i ofVec4i::getLimited(int max) const {
    ofVec4i limited;
    int lengthSquared = (x*x + y*y + z*z + w*w);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        int ratio = max/(int)sqrt(lengthSquared);
        limited.set( x*ratio, y*ratio, z*ratio, w*ratio );
    } else {
        limited.set(x,y,z,w);
    }
    return limited;
}

inline ofVec4i& ofVec4i::limit(int max) {
    int lengthSquared = (x*x + y*y + z*z + w*w);
    if( lengthSquared > max*max && lengthSquared > 0 ) {
        int ratio = max/(int)sqrt(lengthSquared);
        x *= ratio;
        y *= ratio;
        z *= ratio;
        w *= ratio;
    }
    return *this;
}



// Length
//
//
inline int ofVec4i::length() const {
    return (int)sqrt( x*x + y*y + z*z + w*w );
}

inline int ofVec4i::lengthSquared() const {
    return squareLength();
}

inline int ofVec4i::squareLength() const {
    return (int)(x*x + y*y + z*z + w*w);
}



/**
 * Dot Product.
 */
inline int ofVec4i::dot( const ofVec4i& vec ) const {
    return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
}





// Non-Member operators
//
//
inline ofVec4i operator+( int f, const ofVec4i& vec ) {
    return ofVec4i( f+vec.x, f+vec.y, f+vec.z, f+vec.w );
}

inline ofVec4i operator-( int f, const ofVec4i& vec ) {
    return ofVec4i( f-vec.x, f-vec.y, f-vec.z, f-vec.w );
}

inline ofVec4i operator*( int f, const ofVec4i& vec ) {
    return ofVec4i( f*vec.x, f*vec.y, f*vec.z, f*vec.w );
}

inline ofVec4i operator/( int f, const ofVec4i& vec ) {
    return ofVec4i( f/vec.x, f/vec.y, f/vec.z, f/vec.w);
}