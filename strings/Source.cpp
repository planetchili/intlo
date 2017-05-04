#include <conio.h>

void print( const char* s )
{
	for( ; *s != 0; s++ )
	{
		_putch( *s );
	}
}

void read( char* buf,int maxSize )
{
	const char* const pEnd = buf + maxSize;
	for( char c = _getch(); c != 13 && (buf + 1 < pEnd); c = _getch(),buf++ )
	{
		_putch( c );
		*buf = c;
	}
	*buf = 0;
}

int str2int( const char* s )
{
	// scan to start point
	const char* p = s;
	for( ; *p >= '0' && *p <= '9'; p++ );
	p--;

	int val = 0;
	int place = 1;
	// convert place values and accumulate
	for( ; p >= s; p-- )
	{
		val += (*p - '0') * place;
		place *= 10;
	}

	return val;
}

int main()
{
	print( "How many pubes? " );
	char answer[69];
	read( answer,69 );

	const int pubeCount = str2int( answer );

	print( "\n" );
	for( int n = 0; n < pubeCount; n++ )
	{
		print( "Pubes! " );
	}

	while( !_kbhit() );
	return 0;
}