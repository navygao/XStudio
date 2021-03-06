///
/// CopyRight ? 2016 X Studio
/// \file ring.h
/// \date 八月 2016
///
/// \author albert.xu windxu@126.com
/// \brief 环缓冲
/// 

#pragma once
#include "defines.h"
#include "exports.h"

namespace xgc
{
	namespace common
	{
		enum COMMON_API buffer_exception_code
		{
			buffer_copy_overflow = -1,
		};

		#if defined _WINDOWS
		class COMMON_API std::exception;
		#endif
		
		class COMMON_API buffer_exception : std::exception
		{
		public:
			buffer_exception()
				: exception()
			{
			}

			buffer_exception(char const* const _Message, int _Code) noexcept
				: exception() 
			{
			}

			buffer_exception(buffer_exception const& _Other)
				: exception( _Other )
			{
			}

			buffer_exception& operator=(buffer_exception const& _Other)
			{
				if (this == &_Other)
					return *this;

				return *this;
			}

			virtual ~buffer_exception() throw()
			{
				
			}

			virtual const char * what() const noexcept
			{
				return std::exception::what();
			}
		private:
			// 错误码
			int code;
			xgc::string message;

		};

		///
		/// \brief 静态缓冲区
		///
		/// \author albert.xu
		/// \date 2015/12/18 14:12
		///
		template<xgc_size size_>
		class static_buffer
		{
		protected:
			/// 对齐原则，LEN尽量是_header大小的倍数
			xgc_char data_[size_];
		public:
			///
			/// \brief 构造
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:11
			///
			static_buffer()
			{
				memset( data_, 0, size_ );
			}

			///
			/// \brief 通过指针和长度构造
			///
			/// \author albert.xu
			/// \date 2016/09/13 15:27
			///
			static_buffer( xgc_lpcvoid data, xgc_size size )
			{
				if( size == -1 )
					size = XGC_MIN( capacity(), size );

				XGC_ASSERT_THROW( size <= capacity(), buffer_exception( buffer_copy_overflow ) );

				memcpy( base(), data, size );
			}

			///
			/// \brief 通过指针和长度构造
			///
			/// \author albert.xu
			/// \date 2016/09/13 15:27
			///
			template< class _Tx, xgc_size size >
			static_buffer( _Tx (&data)[size] )
			{
				static_assert( size * sizeof( _Tx ) <= size_, "buffer copy overflow" );

				memcpy( base(), (xgc_byte*)data, size * sizeof( _Tx ) );
			}

			///
			/// \brief 通过buffer对象构造
			///
			/// \author albert.xu
			/// \date 2015/12/18 16:14
			///
			template< class _Bx >
			static_buffer( const _Bx &buffer )
			{
				XGC_ASSERT_THROW( buffer.capacity() <= capacity(), buffer_exception( buffer_copy_overflow ) );

				memcpy( base(), buffer.base(), buffer.capacity() );
			}

			///
			/// \brief 
			///
			/// \author albert.xu
			/// \date 2015/12/18 16:14
			///
			template< class _Bx >
			static_buffer( const _Bx &buffer, xgc_size size, xgc_size offset = 0 )
			{
				XGC_ASSERT_THROW( offset + size <= buffer.capacity(), buffer_exception( buffer_copy_overflow ) );

				if( size == -1 )
					size = XGC_MIN( capacity(), buffer.capacity() - offset );

				XGC_ASSERT_THROW( size <= capacity(), buffer_exception( buffer_copy_overflow ) );

				memcpy( base(), buffer.base() + offset, size );
			}

			///
			/// \brief 获取缓冲基地址
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:09
			///
			XGC_INLINE xgc_lpstr base()
			{
				return data_;
			}

			///
			/// \brief 获取缓冲基地址
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:09
			///
			XGC_INLINE xgc_lpcstr base() const
			{
				return data_;
			}

			///
			/// \brief 获取缓冲区大小
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:10
			///
			XGC_INLINE xgc_size capacity()const
			{
				return size_;
			}
		};

		class COMMON_API reference_buffer
		{
		private:
			xgc_lpcstr data_;
			xgc_size   size_;

		public:
			reference_buffer()
				: data_( xgc_nullptr )
				, size_( 0 )
			{

			}

			///
			/// \brief 缓冲区构造
			///
			/// \author albert.xu
			/// \date 2016/09/18 16:55
			///
			reference_buffer( xgc_lpcvoid data, xgc_size size )
				: data_( (xgc_lpcstr)data )
				, size_( size )
			{

			}

			///
			/// \brief 数组构造
			///
			/// \author albert.xu
			/// \date 2016/09/18 16:55
			///
			template< class _Tx, xgc_size _S >
			reference_buffer( _Tx( &A )[_S] )
				: data_( (xgc_lpcstr) A )
				, size_( _S * sizeof( _Tx ) )
			{

			}

			///
			/// \brief 拷贝构造
			///
			/// \author albert.xu
			/// \date 2016/09/14 14:05
			///
			template< class _Bx >
			reference_buffer( const _Bx& buffer )
				: data_( buffer.base() )
				, size_( buffer.capacity() )
			{
			}

			///
			/// \brief 获取缓冲基地址
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:09
			///
			xgc_lpcstr base()
			{
				return data_;
			}

			///
			/// \brief 获取缓冲基地址
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:09
			///
			xgc_lpcstr base() const
			{
				return data_;
			}

			///
			/// \brief 获取缓冲区大小
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:10
			///
			xgc_size capacity()const
			{
				return size_;
			}
		};

		class COMMON_API shared_memory_buffer
		{
		private:
			/// 共享内存句柄
			#if defined _WINDOWS
			xgc_handle shared_memory_;
			#endif

			xgc_char shared_file_name[XGC_MAX_FNAME];
			xgc_char shared_file_path[XGC_MAX_PATH];
			/// 缓冲区首地址
			xgc_lpstr data_;
			xgc_size  size_;
		public:
			///
			/// \brief 默认构造
			///
			/// \author albert.xu
			/// \date 2016/09/14 13:23
			///
			shared_memory_buffer();

			///
			/// \brief 构造共享内存
			///
			/// \author albert.xu
			/// \date 2016/09/12 12:06
			///
			shared_memory_buffer( xgc_lpcstr shared_memory_name, xgc_size size );

			///
			/// \brief 拷贝构造
			///
			/// \author albert.xu
			/// \date 2015/12/18 16:31
			///
			shared_memory_buffer( const shared_memory_buffer& buffer );

			///
			/// \brief 右值拷贝构造
			///
			/// \author albert.xu
			/// \date 2015/12/18 16:31
			///
			shared_memory_buffer( shared_memory_buffer&& buffer );

			///
			/// \brief 析构函数
			///
			/// \author albert.xu
			/// \date 2015/12/18 16:34
			///
			~shared_memory_buffer();

			///
			/// \brief 创建共享内存
			///
			/// \author albert.xu
			/// \date 2015/12/18 16:29
			///
			xgc_long create( xgc_lpcstr shared_memory_name, xgc_size size, xgc_lpcstr file_path = xgc_nullptr );

			///
			/// \brief 销毁共享内存对象
			///
			/// \author albert.xu
			/// \date 2015/12/18 16:35
			///
			xgc_void destroy();

			///
			/// \brief 创建共享内存
			///
			/// \author albert.xu
			/// \date 2015/12/18 16:29
			///
			xgc_bool is_create()const
			{
				return data_ != xgc_nullptr;
			}

			///
			/// \brief 获取缓冲基地址
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:09
			///
			xgc_lpstr base()
			{
				return data_;
			}

			///
			/// \brief 获取缓冲基地址
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:09
			///
			xgc_lpcstr base() const
			{
				return data_;
			}

			///
			/// \brief 获取缓冲区大小
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:10
			///
			xgc_size capacity()const
			{
				return size_;
			}
		};

		///
		/// \brief 分离的缓冲记录器
		///
		/// \author albert.xu
		/// \date 2016/09/13 16:31
		///
		template< class _Buf >
		class separate_buffer_recorder : public _Buf
		{
			typedef _Buf _Base;
		protected:
			/// 写指针位置
			xgc_size wd_;
			/// 读指针位置
			xgc_size rd_;

		public:
			///
			/// \brief 默认构造
			///
			/// \author albert.xu
			/// \date 2016/09/14 20:14
			///
			separate_buffer_recorder()
				: _Buf()
				, wd_( 0 )
				, rd_( 0 )
			{

			}

			///
			/// \brief 拷贝构造
			///
			/// \author albert.xu
			/// \date 2016/09/18 15:54
			///
			template< class _Tx >
			separate_buffer_recorder( const _Tx &buffer )
				: _Buf( buffer )
				, wd_( 0 )
				, rd_( 0 )
			{

			}

			///
			/// \brief 拷贝构造
			///
			/// \author albert.xu
			/// \date 2016/09/18 15:54
			///
			template< class _Tx >
			separate_buffer_recorder( const _Tx &buffer, xgc_size wd, xgc_size rd )
				: _Buf( buffer )
				, wd_( wd )
				, rd_( rd )
			{

			}

			///
			/// \brief 拷贝构造
			///
			/// \author albert.xu
			/// \date 2016/09/13 15:46
			///
			template< template < class, template< class > class > class _Tx, class _Ty, template< class > class _Rec >
			separate_buffer_recorder( const _Tx< _Ty, _Rec > &buffer )
				: _Buf( buffer )
				, wd_( buffer.wd() )
				, rd_( buffer.rd() )
			{

			}

			///
			/// \brief 拷贝构造
			///
			/// \author albert.xu
			/// \date 2016/09/13 15:46
			///
			template< template < class, template< class > class > class _Tx, class _Ty, template< class > class _Rec >
			separate_buffer_recorder( const _Tx< _Ty, _Rec > &buffer, xgc_size wd, xgc_size rd )
				: _Buf( buffer )
				, wd_( wd )
				, rd_( rd )
			{

			}

			///
			/// \brief 开始位置
			///
			/// \author albert.xu
			/// \date 2016/09/13 16:01
			///
			xgc_lpcstr begin()
			{
				return _Base::base();
			}

			///
			/// \brief 开始位置
			///
			/// \author albert.xu
			/// \date 2016/09/13 16:01
			///
			xgc_lpcstr begin()const
			{
				return _Base::base();
			}

			///
			/// \brief 开始位置
			///
			/// \author albert.xu
			/// \date 2016/09/13 16:01
			///
			xgc_lpcstr end()
			{
				return _Base::base() + _Base::capacity();
			}

			///
			/// \brief 开始位置
			///
			/// \author albert.xu
			/// \date 2016/09/13 16:01
			///
			xgc_lpcstr end()const
			{
				return _Base::base() + _Base::capacity();
			}

			///
			/// \brief 操作的缓冲区长度
			///
			/// \author albert.xu
			/// \date 2016/09/13 16:03
			///
			xgc_size len()const
			{
				return _Base::capacity();
			}

			///
			/// \brief 读偏移
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:11
			///
			xgc_size wd()const
			{
				return wd_;
			}

			///
			/// \brief 写偏移
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:11
			///
			xgc_size rd()const
			{
				return rd_;
			}

			///
			/// \brief 设置读偏移
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:12
			///
			xgc_void set_rd( xgc_size val )
			{
				rd_ = val;
			}

			///
			/// \brief 设置写偏移
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:11
			///
			xgc_void set_wd( xgc_size val )
			{
				wd_ = val;
			}
		};

		///
		/// \brief 一体的缓冲记录器
		///
		/// \author albert.xu
		/// \date 2016/09/13 16:31
		///
		template< class _Buf >
		class union_buffer_recorder : public _Buf
		{
			typedef _Buf _Base;
		protected:
			struct recorder
			{
				/// 写指针位置
				xgc_size wd_;
				/// 读指针位置
				xgc_size rd_;
			};

		public:
			///
			/// \brief 默认构造
			///
			/// \author albert.xu
			/// \date 2016/09/14 20:13
			///
			union_buffer_recorder()
				: _Buf()
			{
				memset( (xgc_lpvoid)_Base::base(), 0, sizeof( recorder ) );
			}

			///
			/// \brief 拷贝构造
			///
			/// \author albert.xu
			/// \date 2016/09/13 15:46
			///
			template< class _Buf_Other >
			union_buffer_recorder( const _Buf_Other &buffer )
				: _Buf( buffer )
			{
			}

			///
			/// \brief 拷贝构造
			///
			/// \author albert.xu
			/// \date 2016/09/13 15:46
			///
			template< class _Buf_Other >
			union_buffer_recorder( const _Buf_Other &buffer, xgc_size wd, xgc_size rd )
				: _Buf( buffer )
			{
				set_wd( wd );
				set_rd( rd );
			}

			///
			/// \brief 开始位置
			///
			/// \author albert.xu
			/// \date 2016/09/13 16:01
			///
			xgc_lpcstr begin()
			{
				return _Base::base() + sizeof( recorder );
			}

			///
			/// \brief 开始位置
			///
			/// \author albert.xu
			/// \date 2016/09/13 16:01
			///
			xgc_lpcstr begin()const
			{
				return _Base::base() + sizeof( recorder );
			}

			///
			/// \brief 开始位置
			///
			/// \author albert.xu
			/// \date 2016/09/13 16:01
			///
			xgc_lpcstr end()
			{
				return _Base::base() + _Base::capacity();
			}

			///
			/// \brief 开始位置
			///
			/// \author albert.xu
			/// \date 2016/09/13 16:01
			///
			xgc_lpcstr end()const
			{
				return _Base::base() + _Base::capacity();
			}

			///
			/// \brief 操作的缓冲区长度
			///
			/// \author albert.xu
			/// \date 2016/09/13 16:03
			///
			xgc_size len()const
			{
				return _Base::capacity() - sizeof(recorder);
			}

			///
			/// \brief 读偏移
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:11
			///
			xgc_size wd()const
			{
				return ( (recorder*) _Base::base() )->wd_;
			}

			///
			/// \brief 写偏移
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:11
			///
			xgc_size rd()const
			{
				return ( (recorder*) _Base::base() )->rd_;
			}

			///
			/// \brief 设置写偏移
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:11
			///
			xgc_void set_wd( xgc_size val )
			{
				( (recorder*) _Base::base() )->wd_ = val;
			}

			///
			/// \brief 设置读偏移
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:12
			///
			xgc_void set_rd( xgc_size val )
			{
				( (recorder*) _Base::base() )->rd_ = val;
			}
		};

		///
		/// \brief 一个recorder的中间层
		///
		/// \author albert.xu
		/// \date 2016/09/14 19:35
		///
		template< class _Buf, template< class > class _Rec >
		class buffer_recorder : public _Rec< _Buf >
		{
			typedef _Rec<_Buf> _Base;
		public:
			///
			/// \brief 默认构造
			///
			/// \author albert.xu
			/// \date 2016/09/13 15:46
			///
			buffer_recorder()
				: _Rec< _Buf >()
			{
			}

			///
			/// \brief 拷贝构造
			///
			/// \author albert.xu
			/// \date 2016/09/13 15:46
			///
			template< class _Buf_Other >
			buffer_recorder( const _Buf_Other &object )
				: _Rec< _Buf >( object )
			{
			}

			///
			/// \brief 拷贝构造
			///
			/// \author albert.xu
			/// \date 2016/09/13 15:46
			///
			template< class _Buf_Other >
			buffer_recorder( const _Buf_Other &object, xgc_size wd, xgc_size rd )
				: _Rec< _Buf >( object, wd, rd )
			{
			}


			///
			/// \brief 获取读指针位置
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:09
			///
			xgc_lpcstr rd_ptr()const
			{
				return _Base::begin() + _Base::rd();
			}

			///
			/// \brief 获取写指针位置
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:10
			///
			xgc_lpcstr wd_ptr()const
			{
				return _Base::begin() + _Base::wd();
			}

			///
			/// \brief 增加读偏移
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:12
			///
			xgc_void plus_rd( xgc_long add )
			{
				_Base::set_rd( _Base::rd() + add );
			}

			///
			/// \brief 增加写偏移
			///
			/// \author albert.xu
			/// \date 2015/12/18 14:12
			///
			xgc_void plus_wd( xgc_long add )
			{
				_Base::set_wd( _Base::wd() + add );
			}


			///
			/// \brief 重置读写指针
			///
			/// \author albert.xu
			/// \date 2016/03/01 14:04
			///
			xgc_void reset()
			{
				_Base::set_rd( 0 );
				_Base::set_wd( 0 );
			}

			///
			/// \brief 重置读写指针
			///
			/// \author albert.xu
			/// \date 2016/03/01 14:04
			///
			xgc_void reset( xgc_size rd, xgc_size wd )
			{
				_Base::set_rd( rd );
				_Base::set_wd( wd );
			}
		};

		template< class _Buf, template< class > class _Rec = separate_buffer_recorder >
		class linear_buffer : public buffer_recorder< _Buf, _Rec >
		{
			typedef buffer_recorder< _Buf, _Rec > _Base;
		public:
			linear_buffer()
				: buffer_recorder< _Buf, _Rec >()
			{

			}

			///
			/// \brief 拷贝构造
			///
			/// \author albert.xu
			/// \date 2016/09/14 14:19
			///
			template< class _Buf_Other >
			linear_buffer( const _Buf_Other & rhs )
				: buffer_recorder< _Buf, _Rec >( rhs )
			{

			}

			///
			/// \brief 拷贝构造
			///
			/// \author albert.xu
			/// \date 2016/09/14 14:19
			///
			template< class _Buf_Other >
			linear_buffer( const _Buf_Other & rhs, xgc_size wd, xgc_size rd )
				: buffer_recorder< _Buf, _Rec >( rhs, wd, rd )
			{

			}

			///
			/// \brief 返回未写入空间大小
			///
			/// \author albert.xu
			/// \date 2016/03/01 15:32
			///
			xgc_size space()const
			{
				XGC_ASSERT_RETURN( _Base::end() >= _Base::wd_ptr(), 0 );
				return _Base::end() - _Base::wd_ptr();
			}

			///
			/// \brief 返回未读取空间大小
			///
			/// \author albert.xu
			/// \date 2016/03/01 15:32
			///
			xgc_size leave()const
			{
				XGC_ASSERT_RETURN( _Base::wd_ptr() >= _Base::rd_ptr(), 0 );
				return _Base::wd_ptr() - _Base::rd_ptr();
			}

			///
			/// \brief 读取缓冲取数据
			///
			/// \author albert.xu
			/// \date 2015/12/18 15:52
			///
			xgc_long read_some( xgc_lpvoid data, xgc_size size )
			{
				if( _Base::wd() <= _Base::rd() )
					return -1;

				xgc_long cpy = (xgc_long) XGC_MIN( _Base::wd() - _Base::rd(), size );
				memcpy( data, _Base::rd_ptr(), cpy );
				_Base::plus_rd( cpy );

				return cpy;
			}

			///
			/// \brief 写入缓冲数据
			///
			/// \author albert.xu
			/// \date 2015/12/18 15:53
			///
			xgc_long write_some( xgc_lpcvoid data, xgc_size size )
			{
				XGC_ASSERT( _Base::wd() < _Base::len() );

				xgc_long cpy = (xgc_long) XGC_MIN( size, _Base::len() - _Base::wd() );

				memcpy( (xgc_lpvoid)_Base::wd_ptr(), data, cpy );
				_Base::plus_wd( cpy );

				return cpy;
			}
		};

		template< class _Buf, template< class > class _Rec = separate_buffer_recorder >
		class ring_buffer : public buffer_recorder< _Buf, _Rec >
		{
			typedef buffer_recorder< _Buf, _Rec > _Base;
		public:
			///
			/// \brief 默认构造函数
			///
			/// \author albert.xu
			/// \date 2016/09/14 17:42
			///
			ring_buffer()
				: buffer_recorder< _Buf, _Rec >()
			{
			}

			///
			/// \brief 拷贝构造函数
			///
			/// \author albert.xu
			/// \date 2016/09/14 17:42
			///
			template< class _Buf_Other >
			ring_buffer( const _Buf_Other & rhs )
				: buffer_recorder< _Buf, _Rec >( rhs )
			{
			}

			///
			/// \brief 拷贝构造函数
			///
			/// \author albert.xu
			/// \date 2016/09/14 17:42
			///
			template< class _Buf_Other >
			ring_buffer( const _Buf_Other & rhs, xgc_size wd, xgc_size rd )
				: buffer_recorder< _Buf, _Rec >( rhs, wd, rd )
			{
			}

			///
			/// \brief 析构函数
			///
			/// \author albert.xu
			/// \date 2016/09/14 17:41
			///
			~ring_buffer()
			{
			}

			///
			/// \brief 返回未写入空间大小
			///
			/// \author albert.xu
			/// \date 2016/03/01 15:32
			///
			xgc_size space()const
			{
				if( _Base::wd() >= _Base::rd() )
				{
					return ( _Base::end() - _Base::wd_ptr() ) + ( _Base::rd_ptr() - _Base::begin() ) - 1;
				}
				else
				{
					return _Base::rd_ptr() - _Base::wd_ptr() - 1;
				}
			}

			///
			/// \brief 返回未读取空间大小
			///
			/// \author albert.xu
			/// \date 2016/03/01 15:32
			///
			xgc_size leave()const
			{
				if( _Base::rd() > _Base::wd() )
				{
					return ( _Base::end() - _Base::rd_ptr() ) + ( _Base::wd_ptr() - _Base::begin() );
				}
				else
				{
					return _Base::wd_ptr() - _Base::rd_ptr();
				}
			}

			xgc_size read_some( xgc_lpvoid data, xgc_size size )
			{
				xgc_size part_1 = 0;
				xgc_size part_2 = 0;

				xgc_size copy_1 = 0;
				xgc_size copy_2 = 0;

				if( _Base::rd() > _Base::wd() )
				{
					// <-------w       r------->
					part_1 = _Base::end() - _Base::rd_ptr();
					copy_1 = XGC_MIN( part_1, size );

					memcpy( data, _Base::rd_ptr(), copy_1 );
					_Base::set_rd( ( _Base::rd() + copy_1 ) % _Base::len() );
				}

				if( _Base::rd() < _Base::wd() )
				{
					// <       r-------w       >
					part_2 = _Base::wd_ptr() - _Base::rd_ptr();
					copy_2 = XGC_MIN( part_2, size - copy_1 );

					memcpy( (xgc_byte*)data + copy_1, _Base::rd_ptr(), copy_2 );
					_Base::plus_rd( (xgc_long) copy_2 );
				}

				return copy_1 + copy_2;
			}

			xgc_size write_some( xgc_lpcvoid data, xgc_size size )
			{
				size = XGC_MIN( size, _Base::len() );

				xgc_size part_1 = 0;
				xgc_size part_2 = 0;

				xgc_size copy_1 = 0;
				xgc_size copy_2 = 0;

				// 注意，写指针不能越过读指针
				if( _Base::wd() >= _Base::rd() )
				{
					// <       r-------w       >
					part_1 = _Base::len() - _Base::wd();
					copy_1 = size < part_1 ? size : _Base::rd() ? part_1 : part_1 - 1;

					XGC_ASSERT_RETURN( copy_1 <= part_1, 0 );

					memcpy( (xgc_lpvoid)_Base::wd_ptr(), data, copy_1 );
					_Base::set_wd( ( _Base::wd() + copy_1 ) % _Base::len() );
				}

				if( size == copy_1 )
					return size;

				if( _Base::wd() < _Base::rd() )
				{
					// <-------w       r------->
					part_2 = _Base::rd_ptr() - _Base::wd_ptr();
					copy_2 = XGC_MIN( part_2 - 1, size - copy_1 );

					XGC_ASSERT_RETURN( copy_2 <= part_2, copy_1 );

					memcpy( (xgc_lpvoid)_Base::wd_ptr(), (xgc_byte*)data + copy_1, copy_2 );
					_Base::plus_wd( (xgc_long)copy_2 );
				}

				return copy_1 + copy_2;
			}

			xgc_size write_overflow( xgc_lpcvoid data, xgc_size size )
			{
				xgc_size part_1 = 0;
				xgc_size part_2 = 0;

				xgc_size copy_1 = 0;
				xgc_size copy_2 = 0;

				size = XGC_MIN( size, _Base::len() );
				if( _Base::wd() >= _Base::rd() )
				{
					// <       r-------w       >
					part_1 = _Base::len() - _Base::wd();
					copy_1 = XGC_MIN( size, part_1 );

					XGC_ASSERT_RETURN( copy_1 <= part_1, 0 );

					memcpy( (xgc_lpvoid)_Base::wd_ptr(), data, copy_1 );

					_Base::set_wd( ( _Base::wd() + copy_1 ) % _Base::len() );
					if( part_1 == copy_1 && _Base::rd() == 0 )
						_Base::set_rd( 1 );
				}

				while( copy_1 + copy_2 < size )
				{
					copy_1 += copy_2;
					copy_2 = 0;

					// <-------w       r------->
					part_2 = _Base::len() - _Base::wd();
					copy_2 = XGC_MIN( _Base::len() - copy_1, size - copy_1 );

					memcpy( (xgc_lpvoid)_Base::wd_ptr(), (xgc_byte*)data + copy_1, copy_2 );

					_Base::set_wd( ( _Base::wd() + copy_2 ) % _Base::len() );
					if( copy_2 == part_2 )
						_Base::set_rd( 1 );
				}

				return copy_1 + copy_2;
			}
		};
	};  // end namespace common
};  // end namespace XGC