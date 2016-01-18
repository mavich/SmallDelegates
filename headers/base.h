#ifndef _DELEGATES_BASE_H_
#define _DELEGATES_BASE_H_


namespace delegates_ns
{
	template < class FType >
	class IDelegate;

	template < class R, class ...Args >
	class IDelegate < R ( Args... ) >
	{
	public:
		virtual ~IDelegate( void ) { };


		virtual size_t      getHashKey( void ) const = 0;
		
		virtual IDelegate&   copy( void ) const = 0;

		virtual R           operator( ) ( Args... args ) const = 0;


		friend bool			operator==( const IDelegate& left, const IDelegate& right )
		{
			return left.getHashKey() == right.getHashKey();
		}
	};
}

#endif //_DELEGATES_BASE_H_