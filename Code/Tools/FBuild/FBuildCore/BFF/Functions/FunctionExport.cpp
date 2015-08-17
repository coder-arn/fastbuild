// FunctionExport
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Tools/FBuild/FBuildCore/PrecompiledHeader.h"

#include "FunctionExport.h"
#include "Tools/FBuild/FBuildCore/FLog.h"
#include "Tools/FBuild/FBuildCore/BFF/BFFIterator.h"
#include "Tools/FBuild/FBuildCore/BFF/BFFParser.h"
#include "Tools/FBuild/FBuildCore/BFF/BFFStackFrame.h"

// CONSTRUCTOR
//------------------------------------------------------------------------------
FunctionExport::FunctionExport()
: Function( "Export" )
{
}

// AcceptsHeader
//------------------------------------------------------------------------------
/*virtual*/ bool FunctionExport::AcceptsHeader() const
{
    return true;
}

// NeedsHeader
//------------------------------------------------------------------------------
/*virtual*/ bool FunctionExport::NeedsHeader() const
{
    return true;
}

// NeedsBody
//------------------------------------------------------------------------------
/*virtual*/ bool FunctionExport::NeedsBody() const
{
    return false;
}

//------------------------------------------------------------------------------
/*virtual*/ bool FunctionExport::ParseFunction(const BFFIterator & functionNameStart,
                                               const BFFIterator * functionBodyStartToken, 
                                               const BFFIterator * functionBodyStopToken,
                                               const BFFIterator * functionHeaderStartToken,
                                               const BFFIterator * functionHeaderStopToken ) const
{
    (void)functionNameStart;
    (void)functionBodyStartToken;
    (void)functionBodyStopToken;

    if ( functionHeaderStartToken && functionHeaderStopToken && 
         ( functionHeaderStartToken->GetDistTo( *functionHeaderStopToken ) >= 1 ) )
    {
        BFFIterator start( *functionHeaderStartToken );
        ASSERT( *start == BFFParser::BFF_FUNCTION_ARGS_OPEN );
        start++;
        start.SkipWhiteSpace();

		AStackString< BFFParser::MAX_VARIABLE_NAME_LENGTH > varName;
		if ( BFFParser::ParseVariableName( start, varName ) == false )
		{
			return false;
		}

        // we want 1 frame above this function
        BFFStackFrame * currentFrame = BFFStackFrame::GetCurrent()->GetParent();
        ASSERT( currentFrame );

        // look for variable name in local variables
        const BFFVariable * v = currentFrame->GetLocalVar( varName );
        if ( v == nullptr )
        {
            Error::Error_1009_UnknownVariable( start, this );
            return false;
        }

        // look for the scope containing the original variable
        BFFStackFrame * dstFrame = currentFrame->GetParent();
        for ( ; dstFrame; dstFrame = dstFrame->GetParent() )
        {
            const BFFVariable * dst = dstFrame->GetLocalVar( varName );
            if ( dst != nullptr )
            {
                break;
            }
        }

        if ( dstFrame == nullptr )
        {
            // copy local variable in parent scope
            BFFStackFrame::SetVar( v, currentFrame->GetParent() );
        }
        else
        {
            // copy local variable in the scope containing the previous definition
            BFFStackFrame::SetVar( v, dstFrame );
        }
    }

    return true;
}

//------------------------------------------------------------------------------
