#ifndef _DELEGATES_FUNCTIONS_H_
#define _DELEGATES_FUNCTIONS_H_

#include "base.h"
#include <typeinfo>

namespace delegates_ns
{

	template < class FType >
	class FunctionD;

	template< class R, class ...Args >
	class FunctionD< R ( Args... ) > 
		: public IDelegate< R ( Args... ) >
	{
	public:
		using Delegate = IDelegate< R ( Args... ) >;
		typedef R			( *FType )( Args... );

		FunctionD( void ) = delete;
		FunctionD( FType func ) 
			: function( func )
		{ }
		FunctionD( const FunctionD& other ) 
			: function( other.function )
		{ }
		virtual ~FunctionD( void )
		{
			this->function = nullptr;
		}


		virtual size_t		getHashKey( void ) const override
		{
			return typeid( function ).hash_code() ^ size_t( function );
		}
		
		virtual Delegate&	copy( void ) const override
		{
			return *new FunctionD( *this );
		}

		virtual R			operator( ) ( Args... args ) const override
		{
			return function( args... );
		}
		

		FunctionD&			operator=	( const FunctionD& functionD )
		{
			if ( this != &functionD )
			{
				this->function = functionD.function;
			}
			return *this;
		}
		FunctionD&			operator=	( const FType function )
		{
			this->function = function;
			return *this;
		}
		
		friend bool			operator==	( const FunctionD& left, const FType right )
		{
			return left.function == right;
		}
		friend bool			operator==	( const FType left, const FunctionD& right )
		{
			return right.function == left;
		}
		

		FType				function;
	};
}

#endif //_DELEGATES_FUNCTIONS_H_