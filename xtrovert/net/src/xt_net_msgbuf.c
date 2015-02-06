/************************************************************************************
* xt_file_api.c: Implementation File
*
* DESCRIPTION  :   
*
* AUTHOR	    :	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		    :	Sep. 24, 2010
*
* Copyright (c) 2010-?. All Rights Reserved.
*
* REMARKS:
*		
*      
************************************************************************************/

#include "net/inc/xt_net_msgbuf.h"
#include "xt_mem_api.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#pragma warning (disable: 4200) /* Ignore:  warning C4200: nonstandard extension used : zero-sized array in struct/union */
#pragma warning (disable: 4189) /* local variable is initialized but not referenced */
#endif

#define xt_counter_max					(((bytes_counter_t)(~0))>>1) /*01111111,11111111*/
#define xt_ovflow_mask					(~(xt_counter_max)) /* 10000000,00000000 */
#define xt_counter_overflow(c)			(xt_ovflow_mask&(c))

#define xt_net_msgbuf_calc_size(b)			(sizeof(xt_net_msgbuf_t)+(b))
#define xt_net_msgbuf_data_size(buf_ptr)	(((buf_ptr)->tail+(buf_ptr)->capacity-(buf_ptr)->head)%(buf_ptr)->capacity)

#define xt_net_msgbuf_reset_overflow(buf_ptr)							\
					do													\
					{													\
						(buf_ptr)->read_bytes &= xt_counter_max;		\
						(buf_ptr)->write_bytes &= xt_counter_max;		\
					} while (0)

void xt_net_msgbuf_clear( xt_net_msgbuf_t *msgbuf )
{
	msgbuf->head = 0;
	msgbuf->tail = 0;
	msgbuf->read_bytes = 0;
	msgbuf->write_bytes = 0;
}

xt_net_msgbuf_t* xt_net_msgbuf_create( xt_size_t bytes )
{
	xt_net_msgbuf_t *ret = NULL;
	ret = (xt_net_msgbuf_t*)xt_mem_alloc(xt_net_msgbuf_calc_size(bytes));

	if ( ret )
	{
		xt_net_msgbuf_clear( ret );
		ret->capacity = bytes;
	}

	return ret;
}

void xt_net_msgbuf_release( xt_net_msgbuf_t *msgbuf )
{
	xt_net_msgbuf_clear( msgbuf );

	xt_free( msgbuf );
}

char* xt_net_msgbuf_getwrite( xt_net_msgbuf_t *msgbuf, xt_size_t *buf_len )
{
	char *ret = NULL;
	xt_size_t free_len = 0;
	xt_size_t tmp_head = msgbuf->head;

	if ( tmp_head == msgbuf->tail )
	{
		if ( xt_counter_overflow(msgbuf->write_bytes) )
		{
			return NULL; /* Waiting until read_bytes gets overflow. */
		}
		/* if full */
		if ( msgbuf->read_bytes < msgbuf->write_bytes )
		{
			return NULL;
		}
		/* if empty, OK! */
		free_len = msgbuf->capacity - msgbuf->tail;
	}
	else /* len>0 && len<msgbuf->size */
	{
		if ( tmp_head < msgbuf->tail )
		{
			free_len = msgbuf->capacity - msgbuf->tail;
		}
		else /* else msgbuf->tail < msgbuf->head */
		{
			free_len = tmp_head - msgbuf->tail; /* msgbuf->capacity - ((msgbuf->tail+msgbuf->capacity-tmp_head)%(msgbuf->capacity)); */
		}
	}

	ret = &msgbuf->buf[msgbuf->tail];
	*buf_len = free_len;

	return ret;
}

void xt_net_msgbuf_writeend( xt_net_msgbuf_t *msgbuf, xt_size_t bytes )
{
	/* Assumption: bytes > 0 && bytes<free_len */
	xt_size_t tmp_tail = msgbuf->tail;

	tmp_tail += bytes;
	tmp_tail %= msgbuf->capacity; /* in case overflow */

	msgbuf->write_bytes += (bytes_counter_t)bytes;

	msgbuf->tail = tmp_tail;
}

char* xt_net_msgbuf_getread( xt_net_msgbuf_t *msgbuf, xt_size_t *buf_len )
{
	char *ret = NULL;
	xt_size_t data_len = 0;
	xt_size_t tmp_tail = msgbuf->tail;

	if ( tmp_tail == msgbuf->head ) /* Buffer is full or empty */
	{
		/* if empty */
		if ( msgbuf->read_bytes >= msgbuf->write_bytes )
		{
			if ( xt_counter_overflow(msgbuf->read_bytes) ) /* msgbuf->write_bytes must be overflow! */
			{
				xt_net_msgbuf_reset_overflow(msgbuf);
			}
			return NULL;
		}
		/* if full, OK! */
		data_len = msgbuf->capacity - msgbuf->head;
	}
	else /* data_len>0 && data_len<msgbuf->size */
	{
		if ( tmp_tail < msgbuf->head )
		{
			data_len = msgbuf->capacity - msgbuf->head;
		}
		else /* msgbuf->head < else msgbuf->tail */
		{
			data_len = tmp_tail - msgbuf->head; /* (tmp_tail+msgbuf->capacity-msgbuf->head)%(msgbuf->capacity); */
		}
	}

	ret = &msgbuf->buf[msgbuf->head];
	*buf_len = data_len;

	return ret;
}

void xt_net_msgbuf_readend( xt_net_msgbuf_t *msgbuf, xt_size_t bytes )
{
	/* Assumption: bytes > 0 && bytes<data_len */
	xt_size_t tmp_head = msgbuf->head;

	tmp_head += bytes;
	tmp_head %= msgbuf->capacity; /* in case overflow */

	msgbuf->read_bytes += (bytes_counter_t)bytes;

	msgbuf->head = tmp_head;
}

/******************************************************************************************/
#if defined(_DEBUG)

#include "xt_time_api.h"

xt_net_msgbuf_t *g_msgbuffer = NULL;
int g_go_0 = 1;
int g_go_1 = 1;

void wirte_bytes( char *buf, unsigned char *start_val, xt_size_t n )
{
	xt_size_t i = 0;
	for ( i=0; i<n; ++i )
	{
		buf[i] = (*start_val);
		++(*start_val);
	}
}

void read_bytes( char *buf, unsigned char *arr, xt_size_t len )
{
	xt_size_t i = 0;
	for ( i=0; i<len; ++i )
	{
		arr[i] = buf[i];
	}
}

enum
{
	___consumer = 0,
	___producer,
};

#define test_logstr_len				((XT_NET_MSG_BUF_SIZE_TCP)*4)
static void test_msgbuf_check( int flag, unsigned char *start_val, xt_size_t n, TCHAR *log_str )
{
	xt_size_t i = 0;
	xt_size_t mis = _INVALID_SIZE_T_VALUE_;
	xt_size_t log_bytes = 0;
	unsigned char c = *start_val;
	const TCHAR *title = NULL;
	TCHAR num[9] = {0};
	int num_bytes = 0;

	char *crush = NULL;

	if ( flag == ___consumer )
	{
		title = _T("read:");
	}
	else
	{
		title = _T("write:");
	}

	for ( i=0; i<n; ++i )
	{
		num_bytes = _sntprintf( num, 9, _T("%d,"), *start_val );
		if ( num_bytes<=0 || num_bytes+log_bytes>=test_logstr_len )
		{
			_tprintf( _T("%s crush\n"), title );
			memcpy( crush, start_val, 3 );
		}
		_tcsncpy( &(log_str[log_bytes]), num, num_bytes );
		log_bytes += num_bytes;
		log_str[log_bytes] = 0;
		if ( c!=(*start_val) && mis==_INVALID_SIZE_T_VALUE_ )
		{
			mis = i;
		}
		++c;
		++start_val;
	}

	if ( mis != _INVALID_SIZE_T_VALUE_ )
	{
		_tprintf( _T("!!!!!!!!!!!!!!!!!%s(%d,%d):[%s]\n"), title, n, mis, log_str );
		memcpy( crush, start_val, 3 );
	}
	else
	{
		_tprintf( _T("%s(%d):[%s]\n"), title, n, log_str );
	}
	
	return;
}

unsigned __stdcall test_thr_consumer( void *arg )
{
	xt_net_msgbuf_t **_msg_buf_ptr = (xt_net_msgbuf_t**)arg;
	xt_net_msgbuf_t *msg_buffer = NULL;
	char *buf = NULL;
	xt_size_t buf_len = 0;

	xt_size_t rand_bytes = 0;

	unsigned char *arr = NULL;
	xt_size_t cur_elem = 0;

	TCHAR *log = NULL;
	log = xt_mem_alloc( test_logstr_len );
	if ( log == NULL )
	{
		g_go_1 = 0;
		_tprintf( _T("log END!\n") );
		return 0;
	}

	arr = xt_mem_alloc( XT_NET_MSG_BUF_SIZE_TCP );
	if ( arr == NULL )
	{
		g_go_1 = 0;
		_tprintf( _T("arr END!\n") );
		return 0;
	}

	_tprintf( _T("test_thr_consumer Running!\n") );

	msg_buffer = xt_net_msgbuf_create( XT_NET_MSG_BUF_SIZE_TCP );
	if ( msg_buffer == NULL )
	{
		g_go_1 = 0;
		_tprintf( _T("test_thr_consumer END!\n") );
		return 0;
	}

	*_msg_buf_ptr = msg_buffer;

	_tprintf( _T("test_thr_consumer created!\n") );

	xt_srand();

	while(g_go_0)
	{
		buf = xt_net_msgbuf_getread( msg_buffer, &buf_len );
		if ( buf )
		{
			rand_bytes = rand();
			rand_bytes %= buf_len;
			/*_tprintf( _T("rand_bytes %= buf_len;...![%d]\n"), rand_bytes );*/
			if ( rand_bytes == 0 )
			{
				rand_bytes = buf_len;
			}
			/*rand_bytes = buf_len;*/
			read_bytes( buf, &(arr[cur_elem]), rand_bytes );
			xt_net_msgbuf_readend( msg_buffer, rand_bytes );

			test_msgbuf_check( ___consumer, &(arr[cur_elem]), rand_bytes, log );

			cur_elem += rand_bytes;
			cur_elem %= XT_NET_MSG_BUF_SIZE_TCP;
		}
		else
		{
			//_tprintf( _T("xt_net_msgbuf_getread NULL, [%d,%d] [%d,%d]\n"), msg_buffer->head, msg_buffer->tail, msg_buffer->read_bytes, msg_buffer->write_bytes );
		}
	}

	/* Waiting test_thr_producer END! */
	_tprintf( _T("Waiting test_thr_producer END!\n") );
	xt_msleep( 3000 );

	xt_free( arr );
	xt_free( log );

	xt_net_msgbuf_release( msg_buffer );

	return 0;
}

unsigned __stdcall test_thr_producer( void *arg )
{
	xt_net_msgbuf_t **_msg_buf_ptr = (xt_net_msgbuf_t**)arg;
	xt_net_msgbuf_t *msg_buffer = NULL;
	char *buf = NULL;
	xt_size_t buf_len = 0;

	xt_size_t rand_bytes = 0;
	unsigned char val = 0;

	TCHAR *log = NULL;
	log = xt_mem_alloc( test_logstr_len );
	if ( log == NULL )
	{
		g_go_1 = 0;
		_tprintf( _T("log END!\n") );
		return 0;
	}

	_tprintf( _T("test_thr_producer Running!\n") );

	while(g_go_1)
	{
		/* waiting g_msgbuffer to be created in consumer. */
		/*_msg_buf_ptr = (xt_net_msgbuf_t **)&msg_buf_ptr;*/
		if ( (*_msg_buf_ptr) == NULL )
		{
			continue;
		}
		break;
	}

	msg_buffer = *_msg_buf_ptr;

	xt_srand();

	while(g_go_1)
	{
		buf = xt_net_msgbuf_getwrite( msg_buffer, &buf_len );
		/*_tprintf( _T("xt_net_msgbuf_getwrite...![%d]\n"), buf_len );*/
		if ( buf )
		{
			rand_bytes = rand();
			rand_bytes %= buf_len;
			if ( rand_bytes == 0 )
			{
				rand_bytes = buf_len;
			}
			/*rand_bytes = buf_len;*/
			wirte_bytes( buf, &val, rand_bytes );

			xt_net_msgbuf_writeend( msg_buffer, rand_bytes );

			test_msgbuf_check( ___producer, (unsigned char*)buf, rand_bytes, log );
		}
		else
		{
			//_tprintf( _T("xt_net_msgbuf_getwrite NULL, [%d,%d] [%d,%d]\n"), msg_buffer->head, msg_buffer->tail, msg_buffer->read_bytes, msg_buffer->write_bytes );
		}
	}

	xt_free( log );

	g_go_0 = 0;
	return 0;
}

#include <process.h>    /* _beginthread, _endthread */
void test_net_msgbuf(void)
{
	HANDLE hThread;
	unsigned threadID;

	hThread = (HANDLE)_beginthreadex( NULL, 0, &test_thr_consumer, (void*)&g_msgbuffer, 0, &threadID );
	if(hThread == NULL)
		return;

	hThread = (HANDLE)_beginthreadex( NULL, 0, &test_thr_producer, (void*)&g_msgbuffer, 0, &threadID );
	if(hThread == NULL)
		return;
}
#endif /* (_DEBUG) */
