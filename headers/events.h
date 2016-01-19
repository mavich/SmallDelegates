#ifndef _DELEGATES_EVENTS_H_
#define _DELEGATES_EVENTS_H_

#include "base.h"
#include <typeinfo>

namespace delegates_ns
{

	template< class ...Args >
	class Event 
		: public IDelegate< void ( Args... ) >
	{
	public:
		using Delegate = IDelegate< void ( Args... ) >;


		Event( void )
			: first_( nullptr )
			, last_( nullptr )
		{ }
		Event( const Event& other )
		{
			if( other.first_ )
			{
				this->first_    = new _Node( *other.first_ );
				this->last_     = this->first_->addRange( other.first_->next );

			} else {

				this->last_ = this->first_ = nullptr;
			}
		}
		Event( Event&& other ) 
			: first_( other.first_ )
			, last_( other.last_ )
		{
			other.first_= nullptr;
			other.last_ = nullptr;
		}
		virtual ~Event( void )
		{
			delete this->first_;
			this->first_	= nullptr;
			this->last_		= nullptr;
		}


		virtual size_t		getHashKey( void ) const override
		{
			size_t hash = typeid( *this ).hash_code();
			if ( first_ )
			{
				_Node* d = first_;

				while ( d )
				{
					hash	+=	d->targetDelegate->getHashKey();
					d		=	d->next;
				}
			}
			return hash;
		}
		
		virtual Delegate&	copy( void ) const override
		{
			return *new Event( *this );
		}

		virtual void		operator( ) ( Args... args ) const override
		{
			_Node* d = first_;

			while ( d )
			{
				( *d->targetDelegate )( args... );
				d = d->next;
			}
		}


		Event&				operator=	( const Event& other )
		{
			if ( this == &other )
			{
				return *this;

			}
			delete first_;
			return *new( this ) Event( other );
		}
		Event&				operator=	( Event&& other )
		{
			if ( this == &other )
			{
				return *this;
			}
			return *new( this ) Event( other );
		}

		Event&				operator+=	( const Delegate& delegate )
		{
			if ( this != &delegate )
			{
				size_t hash = delegate.getHashKey();
				if ( first_ )
				{
					_Node* d = first_;

					while ( d )
					{
						if ( hash == d->targetDelegate->getHashKey() )
						{
							return *this;
						}
						d = d->next;
					}
					last_ = last_->add( delegate );

				} else {
					
					last_ = first_ = new _Node( delegate );
				}
			}
			return *this;
		}
		
		Event&				operator-=	( const Delegate& delegate )
		{
			if ( this != &delegate )
			{
				size_t hash = delegate.getHashKey();
				if ( first_ )
				{
					_Node*	d	= first_;
					_Node*	pre = nullptr;
					
					while ( d )
					{
						if ( hash == d->targetDelegate->getHashKey() )
						{
							if ( d->next )
							{
								( pre ? pre->next : first_ ) = d->next;

							} else {
								
								if ( pre ) 
								{
									pre->next	= nullptr;
									last_		= pre;

								} else {

									last_ = first_ = nullptr;
								}
							}
							d->next	= nullptr;
							delete d;
							return *this;
						}
						pre = d;
						d	= d->next;
					}
				}
			}
			return *this;
		}


		Event&				add( const Delegate& delegate )
		{
			 return *this += delegate;
		}

		Event&				remove( const Delegate& delegate )
		{
			return *this -= delegate;
		};


	private:
		struct _Node final
		{
			_Node*		next;
			Delegate*	targetDelegate;

			_Node	( const Delegate& delegate )
				: next( nullptr )
				, targetDelegate( &delegate.copy() ) 
			{ }
			_Node	( const _Node& other )
				: next( nullptr )
				, targetDelegate( &other.targetDelegate->copy() )
			{ }
			~_Node	( void )
			{
				delete next;
				this->next = nullptr;
				delete	this->targetDelegate;
				this->targetDelegate = nullptr;
			}

			_Node&		operator=( const _Node& other ) = delete;

			_Node*		add( const Delegate& delegate )
			{
				if ( next )
				{
					return nullptr;

				}
				return next = new _Node( delegate );
			}

			_Node*		addRange( const _Node* other )
			{
				_Node* n = this;
				while ( other )
				{
					n->next = new _Node( *other );
					other	= other->next;
					if ( !other )
					{
						n = n->next;
					}
				}
				return n;
			}
		};
		
		_Node*				first_;
		_Node*				last_;
	};
}

#endif //_DELEGATES_EVENTS_H_