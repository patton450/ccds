#ifndef CCDS_RBUFFER_h
#define CCDS_RBUFFER_H

struct _rbuffer{
    void **         buffer;
    size_t          capacity;
    
    size_t          start, end;
    size_t          length;

    bool            open;

    //internal events to prevent overwriting/reading garbage
    ccds_condv *    nempty_cv;
    ccds_condv *    nfull_cv;

    //when one of these events happen signal to notify user
    ccds_condv *    empty_cv;
    ccds_condv *    full_cv;
    ccds_condv *    closed_cv;
}

typedef struct _rbuffer rbuffer;

rbuffer * rbuffer_new(size_t cap, memcfg * mem, ccds_error * e);
void rbuffer_free(rbuffer * rb, ccds_error * e);

//adds element signals empty
bool    rbuffer_add_front(rbuffer * rb, void * data, ccds_error * e);
bool    rbuffer_add_back(rbuffer * rb, void * data, ccds_error * e);

//removes element signals full
void *  rbuffer_remove_front(rbuffer * rb,  ccds_error * e);
void *  rbuffer_remove_back(rbuffer * rb, ccds_error * e);

//sets open to false, signals open
bool    rbuffer_close(rbuffer * rb, ccds_error * e);

/* Events */
//waits on open then waits on empty
void    rbuffer_finished(rbuffer * rb, ccds_error * e);

void    rbuffer_closed(rbuffer * rb, ccds_error * e);
void    rbuffer_empty(rbuffer * rb, ccds_error * e);
void    rbuffer_full(rbuffer * rb, ccds_error * e);
#endif
