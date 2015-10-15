//
//    __      ______  _
//   /  )/  //  )/  //_|
//  (__\(__/(__/(  ((  |
//
//
//  QUOTIA CONFIDENTIAL
//
//  ___________________________________
//
//  [2013] - [2014] Quotient Logic, LLC
//  (D/B/A Quotia), All Rights Reserved.
//
//  NOTICE:  All information contained
//  herein is, and remains the property
//  of Quotia.
//
//  The intellectual and technical
//  concepts contained herein are
//  proprietary to Quotient Logic, LLC
//  and may be covered by U.S. and
//  Foreign Patents, patents in process,
//  and are protected by trade secret
//  or copyright law.
//
//  Dissemination of this information
//  or reproduction of this material
//  is strictly forbidden unless prior
//  written permission is obtained
//  from Quotia.
//  ___________________________________
//



#ifndef engine_FSM_h
#define engine_FSM_h

#pragma once

template<typename T>
class FSM
{
public:
    FSM() : _delta(0.0f), _time_in_cur_state(0.0f), _cur_state(-1), _last_state(-1)
    {
    }
    
    virtual void beginState( T state ) {}
    virtual void updateState( T state ) {}
    virtual void endState( T state ) {}
    
    void setState( T state )
    {
        endState( (T)_cur_state );
        _last_state = _cur_state;
        _cur_state = state;
        beginState( (T)_cur_state );
    }
    
    void resetTimer() {
        _time_in_cur_state = 0.0f;
    }
    
    void updateFSM( float delta_time )
    {
        if( _cur_state != -1 )
        {
            _delta = delta_time;
            _time_in_cur_state+=delta_time;
            updateState( (T)_cur_state );
        }
    }
    
    void setTimeInCurState(float t) {
        _time_in_cur_state = t;
    }
    
    float getDelta() { return _delta; }
    float getTimeInCurState() { return _time_in_cur_state; }
    T getState() { return (T)_cur_state; }
    T getLastState() { return (T)_last_state; }
    
    
private:
    float _delta;
    float _time_in_cur_state;
    int _cur_state;
    int _last_state;
};

#endif