#ifndef GLERROR_H
#define GLERROR_H

#include "GL/glew.h"
#include <string>

class GlError
{
public:

    GlError();
    GlError(const GlError& error);

    GlError& operator = (const GlError& glerror);

    bool hasOccured();

    GLenum getErrorFlag() const
    {
        return _errorFlag;
    }

    std::string toString(const char* context) const;

    void reset();

private:
    GLenum _errorFlag;
};

#endif // GLERROR_H
