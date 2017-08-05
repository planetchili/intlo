#include <conio.h>
#include <fstream>
#include <random>
#include <vector>

namespace chili
{
	void printfixed( const char* s,int w )
	{
		int n = 0;
		for( ; *s != 0; s++ )
		{
			n++;
			_putch( *s );
		}
		for( ; n < w; n++ )
		{
			_putch( ' ' );
		}
	}
	void print( const char* s )
	{
		for( ; *s != 0; s++ )
		{
			_putch( *s );
		}
	}

	void strcpy( const char* pSrc,char* pDst,int maxBufSize )
	{
		int n = 0;
		for( ; *pSrc != 0 && (n + 1 < maxBufSize); pSrc++,pDst++,n++ )
		{
			*pDst = *pSrc;
		}
		*pDst = 0;
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

	int fib( int n )
	{
		if( n < 2 )
		{
			return n;
		}
		return fib( n - 1 ) + fib( n - 2 );
	}

	void strrev( char* pl )
	{
		// scan to start point
		char* pr = pl;
		for( ; *pr != 0; pr++ );
		pr--;

		for( ; pl < pr; pl++,pr-- )
		{
			const char temp = *pl;
			*pl = *pr;
			*pr = temp;
		}
	}

	void int2str( int val,char* buf,int size )
	{
		char* const pStart = buf;
		char* const pEnd = buf + size;
		for( ; val > 0 && (buf + 1 < pEnd); val /= 10,buf++ )
		{
			*buf = '0' + val % 10;
		}
		*buf = 0;
		strrev( pStart );
	}

	class Database
	{
	private:
		class Entry
		{
		public:
			Entry() = default;
			Entry( const char* name,int value )
				:
				value( value )
			{
				strcpy( name,this->name,sizeof( this->name ) );
			}
			void Print() const
			{
				printfixed( name,nameBufferSize - 1 );
				_putch( '|' );
				for( int n = 0; n < value; n++ )
				{
					_putch( '=' );
				}
				_putch( '\n' );
			}
			void Serialize( std::ofstream& out ) const
			{
				out.write( name,sizeof( name ) );
				out.write( reinterpret_cast<const char*>(&value),sizeof( value ) );
			}
			void Deserialize( std::ifstream& in )
			{
				in.read( name,sizeof( name ) );
				in.read( reinterpret_cast<char*>(&value),sizeof( value ) );
			}
		private:
			static constexpr int nameBufferSize = 10;
			char name[nameBufferSize];
			int value;
		};
	public:
		void Load( const char* filename )
		{
			std::ifstream in( filename,std::ios::binary );
			int nEntries;
			in.read( reinterpret_cast<char*>(&nEntries),sizeof( nEntries ) );
			entries.resize( nEntries );
			for( auto& e : entries )
			{
				e.Deserialize( in );
			}
		}
		void Save( const char* filename ) const
		{
			std::ofstream out( filename,std::ios::binary );
			const int size = entries.size();
			out.write( reinterpret_cast<const char*>(&size),sizeof( size ) );
			for( const Entry& e : entries )
			{
				e.Serialize( out );
			}
		}
		void Print() const
		{
			for( const Entry& e : entries )
			{
				e.Print();
			}
		}
		void Add( const char* name,int val )
		{
			entries.emplace_back( name,val );
		}
	private:
		std::vector<Entry> entries;
	};
}

void dodb()
{
	chili::Database db;
	char buffer[256];
	char buffer2[256];
	bool quitting = false;

	do
	{
		chili::print( "(l)oad (s)ave (a)dd (q)uit or (p)rint?" );
		char response = _getch();
		switch( response )
		{
		case 'l':
			chili::print( "\nEnter file name: " );
			chili::read( buffer,sizeof( buffer ) );
			db.Load( buffer );
			_putch( '\n' );
			break;
		case 's':
			chili::print( "\nEnter file name: " );
			chili::read( buffer,sizeof( buffer ) );
			db.Save( buffer );
			_putch( '\n' );
			break;
		case 'a':
			chili::print( "\nEnter name: " );
			chili::read( buffer,sizeof( buffer ) );
			chili::print( "\nEnter value: " );
			chili::read( buffer2,sizeof( buffer2 ) );
			db.Add( buffer,chili::str2int( buffer2 ) );
			_putch( '\n' );
			break;
		case 'p':
			chili::print( "\n\n     Beautiful Chart Bitches!" );
			chili::print( "\n     ------------------------\n\n" );
			db.Print();
			_putch( '\n' );
			break;
		case 'q':
			quitting = true;
			break;
		}
	}
	while( !quitting );
}

class SlaveA
{
public:
	SlaveA()
	{
		chili::print( "Default Constructing SlaveA\n" );
	}
	~SlaveA()
	{
		chili::print( "Destructing SlaveA\n" );
	}
	SlaveA( const SlaveA& source )
	{
		chili::print( "Copy Constructing SlaveA\n" );
	}
};

template<typename T>
class DynamicArray
{
public:
	DynamicArray( int size )
		:
		size( size ),
		pArray( new T[size] )
	{}
	DynamicArray( const DynamicArray& source )
	{
		*this = source;
	}
	DynamicArray& operator=( const DynamicArray& source )
	{
		// delete currently owned memory (if any)
		delete pArray;
		pArray = nullptr;
		// allocate new array of same size as source array and set size to source size
		pArray = new T[source.size];
		size = source.size;
		// copy values from source array to our new array
		for( int i = 0; i < size; i++ )
		{
			(*this)[i] = source[i];
		}
		return *this;
	}
	~DynamicArray()
	{
		// this was originally 'delete pArray' in the tutorial video
		// but that is dumb because it is allocated with 'new int[]'
		delete [] pArray;
		pArray = nullptr;
	}
	T& operator[]( int index )
	{
		return pArray[index];
	}
	const T& operator[]( int index ) const
	{
		return pArray[index];
	}
private:
	int size = 0;
	T* pArray = nullptr;
};

int main()
{
	dodb();
	return 0;
}