#pragma once
#ifndef _ASIO_NET_BUFFER_
#define _ASIO_NET_BUFFER_
namespace xgc
{
	namespace net
	{
		class asio_NetBuffer
		{
		private:
			xgc_lpvoid	base_;
			xgc_size	size_;

			xgc_size	tag_;
			xgc_size	len_;

		public:
			asio_NetBuffer( xgc_size size )
				: base_( malloc( size ) )
				, size_( size )
				, tag_( 0 )
				, len_( 0 )
			{

			}

			~asio_NetBuffer()
			{
				free( base_ );

				base_ = xgc_nullptr;
				size_ = 0;

				tag_ = len_ = 0;
			}

			XGC_INLINE xgc_lpvoid base()
			{
				return base_;
			}

			XGC_INLINE xgc_lpcvoid base()const
			{
				return base_;
			}

			XGC_INLINE xgc_lpvoid begin()
			{
				return (char*)base_ + tag_;
			}

			XGC_INLINE xgc_lpcvoid begin()const
			{
				return (const char*)base_ + tag_;
			}

			XGC_INLINE xgc_lpvoid end()
			{
				return (char*)base_ + tag_ + len_;
			}

			XGC_INLINE xgc_lpcvoid end()const
			{
				return (const char*)base_ + tag_ + len_;
			}

			XGC_INLINE xgc_size capacity()const
			{
				return size_;
			}

			XGC_INLINE xgc_size length()const
			{
				return len_;
			}

			XGC_INLINE xgc_size space()const
			{
				return size_ - tag_ - len_;
			}

			XGC_INLINE xgc_size space( xgc_size max )const
			{
				return XGC_MIN( size_ - tag_ - len_, max );
			}

			XGC_INLINE xgc_void reset()
			{
				memmove( base_, begin(), len_ );
				tag_ = 0;
			}

			xgc_void push( xgc_size size )
			{
				XGC_ASSERT_RETURN( size < space(), XGC_NONE );
				len_ += size;
			}

			xgc_size put( xgc_lpvoid data, xgc_size size )
			{
				if( size > space() )
				{
					reset();
					XGC_ASSERT_RETURN( size <= space(), 0 );
				}

				memcpy( end(), data, size );
				push( size );

				return size;
			}

			xgc_size pop( xgc_size size )
			{
				xgc_size copy = XGC_MIN( size, len_ );

				tag_ += copy;
				len_ -= copy;

				return copy;
			}

			xgc_size get( xgc_lpvoid data, xgc_size size )
			{
				xgc_size copy = XGC_MIN( size, len_ );

				memcpy( data, begin(), copy );

				tag_ += copy;
				len_ -= copy;

				return copy;
			}
		};
	}
}

#endif // _ASIO_NET_BUFFER_