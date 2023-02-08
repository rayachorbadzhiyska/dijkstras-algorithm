#ifndef DIJKSTRAEXCEPTION_H
#define DIJKSTRAEXCEPTION_H

#include <stdexcept>

/*!
 *  Represents a custom Dijkstra exception.
 */
class DijkstraException : public std::runtime_error
{
public:
    DijkstraException(const char* msg) : std::runtime_error(msg) {};
    ~DijkstraException() throw() {} ;
};

#endif // DIJKSTRAEXCEPTION_H
