// FunctionExport
//------------------------------------------------------------------------------
#pragma once
#ifndef FBUILD_FUNCTIONS_FUNCTIONEXPORT_H
#define FBUILD_FUNCTIONS_FUNCTIONEXPORT_H

// Includes
//------------------------------------------------------------------------------
#include "Function.h"

// FunctionAlias
//------------------------------------------------------------------------------
class FunctionExport : public Function
{
public:
    explicit        FunctionExport();
    inline virtual ~FunctionExport() {}

protected:
    virtual bool AcceptsHeader() const;
    virtual bool NeedsHeader() const;
    virtual bool NeedsBody() const;

    virtual bool ParseFunction( const BFFIterator & functionNameStart,
                                const BFFIterator * functionBodyStartToken,
                                const BFFIterator * functionBodyStopToken,
                                const BFFIterator * functionHeaderStartToken,
                                const BFFIterator * functionHeaderStopToken ) const;
};

//------------------------------------------------------------------------------
#endif // FBUILD_FUNCTIONS_FUNCTIONEXPORT_H
