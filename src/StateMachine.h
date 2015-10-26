//
//  StateMachine.h
//  matador
//
//  Created by vesabios on 10/24/15.
//
//

#ifndef StateMachine_h
#define StateMachine_h


template <class T>
class State
{
public:
    
    virtual void enter(T*) {};
    virtual void update(T*) {};
    virtual void exit(T*) {};
    
protected:
    State() {};
    virtual ~State() {};
    
private:


};






template <class T>
class StateMachine
{
private:
    
    //a pointer to the agent that owns this instance
    T*          m_pOwner;
    
    State<T>*   m_pCurrentState;
    
    //a record of the last state the agent was in
    State<T>*   m_pPreviousState;
    
    //this state logic is called every time the FSM is updated
    State<T>*   m_pGlobalState;
    
public:
    
    StateMachine(T* owner):m_pOwner(owner),
    m_pCurrentState(NULL),
    m_pPreviousState(NULL),
    m_pGlobalState(NULL)
    {}
    
    //use these methods to initialize the FSM
    void SetCurrentState(State<T>* s){m_pCurrentState = s;}
    void SetGlobalState(State<T>* s) {m_pGlobalState = s;}
    void SetPreviousState(State<T>* s){m_pPreviousState = s;}
    
    //call this to update the FSM
    void update()const
    {
        //if a global state exists, call its execute method
        if (m_pGlobalState) m_pGlobalState->update(m_pOwner);
        
        //same for the current state
        if (m_pCurrentState) {
            //printf("fsm update\n");
            m_pCurrentState->update(m_pOwner);
        }
    }
    
    //change to a new state
    void ChangeState(State<T>* pNewState)
    {
        assert(pNewState &&
               "<StateMachine::ChangeState>: trying to change to a null state");
        
        //keep a record of the previous state
        m_pPreviousState = m_pCurrentState;
        
        //call the exit method of the existing state
        m_pCurrentState->exit(m_pOwner);
        
        //change state to the new state
        m_pCurrentState = pNewState;
        
        //call the entry method of the new state
        m_pCurrentState->enter(m_pOwner);
    }
    
    //change state back to the previous state
    void  RevertToPreviousState()
    {
        ChangeState(m_pPreviousState);
    }
    
    //accessors
    State<T>*  CurrentState()  const{return m_pCurrentState;}
    State<T>*  GlobalState()   const{return m_pGlobalState;}
    State<T>*  PreviousState() const{return m_pPreviousState;}
    
    //returns true if the current state’s type is equal to the type of the
    //class passed as a parameter.
    bool isInState(const State<T>& st)const;
};

#endif /* StateMachine_h */
