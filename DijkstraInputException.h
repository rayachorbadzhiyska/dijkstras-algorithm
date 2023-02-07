#ifndef DIJKSTRAINPUTEXCEPTION_H
#define DIJKSTRAINPUTEXCEPTION_H

#include "DijkstraException.h"

/*!
 *  Represents a Dijkstra input exception.
 */
class DijkstraInputException : public DijkstraException
{
public:
    DijkstraInputException(const char* msg) : DijkstraException(msg) {};
    ~DijkstraInputException() throw() {} ;
};

#endif // DIJKSTRAINPUTEXCEPTION_H
