#ifndef _DELEGATES_METHODS_H_
#define _DELEGATES_METHODS_H_

#include "base.h"
#include <typeinfo>

namespace delegates_ns
{
	template < class T, class FType >
	class MethodD;

	template< class R, class T, class ...Args >
	class MethodD< T, R ( Args... ) > 
		: public IDelegate< R ( Args... ) >
	{
	public:
		using Delegate = IDelegate< R ( Args... ) >;
		typedef R		( T::*FType )( Args... );


		MethodD( void ) = delete;
		MethodD( T* obj, FType func ) 
			: object( obj )
			, function( func )
		{ }
		MethodD( const MethodD& other ) 
			: object( other.object )
			, function( other.function )
		{ }
		virtual ~MethodD( void )
		{
			this->object	= nullptr;
			this->function	= nullptr;
		}


		virtual size_t		getHashKey	( void ) const override
		{
			return 	typeid( function )	.hash_code() ^ 
					~typeid( object )	.hash_code() ^ 
					( _mptr_to_int ( nullptr, function ) + size_t( object ) );
		}

		virtual Delegate&	copy( void ) const override
		{
			return *new MethodD( *this );
		}

		virtual R			operator( ) ( Args... args ) const override
		{
			return ( object->*function )( args... );
		}


		MethodD&			operator=	( const MethodD& functionD )
		{
			if ( this != &functionD )
			{
				this->function	= functionD.function;
				this->object	= functionD.object;
			}
			return *this;
		}
		MethodD&			operator=	( const FType function )
		{
			this->function = function;
			return *this;
		}
		MethodD&			operator=	( T* object )
		{
			this->object = object;
			return *this;
		}

		friend bool			operator==	( const MethodD& left, const T* right )
		{
			return left.object == right;
		}
		friend bool			operator==	( const T* left, const MethodD& right )
		{
			return left == right.object;
		}
		friend bool			operator==	( const MethodD& left, const FType right )
		{
			return left.function == right;
		}
		friend bool			operator==	( const FType left, const MethodD& right )
		{
			return right.function == left;
		}


		T*				object;
		FType			function;


	private:
		size_t				_mptr_to_int( void * mark, ... ) const
		{
			size_t	value = 0;
			void*	ptr = ( char* )&mark + sizeof( mark );

			memcpy( &value, ptr, sizeof( size_t ) );

			return value;
		}
	};
}

#endif //_DELEGATES_METHODS_H_