#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <assert.h>
#include <gmpxx.h>

int ForceMap( std::vector<bool>& CareMask, std::vector<int>& Output ){
	std::vector<int> OAssign;
	std::set<int> Unused;
	
	OAssign.resize( Output.size(), -1 );
	for( int i=0; i<Output.size(); i++ ){
		if( CareMask[i] )
			OAssign[i] = Output[i];
		else
			Unused.insert( Output[i] );
	}

	std::set<int>::iterator itr;
	for( int i=0; i<Output.size(); i++ ){
		if( CareMask[i] )
			continue;
		itr = Unused.find(i);
		if( itr == Unused.end() )
			continue;
		OAssign[i] = i;
		Unused.erase(i);
	}

	itr = Unused.begin();
	for( int i=0; i<Output.size(); i++ ){
		if( CareMask[i] )
			continue;
		if( OAssign[i] != -1 )
			continue;
		if( itr == Unused.end() )
			continue;
		OAssign[i] = *itr;
		itr++;
	}
	assert( itr == Unused.end() );

	for( int i=0; i<Output.size(); i++ ){
		assert( OAssign[i]>=0 );
		if( CareMask[i] ){
			assert( OAssign[i] == Output[i] );
		}
	}
	Output = OAssign;
}

void Write( int dim, std::ostream& ostr, std::vector<int>& Output ){
	ostr<< dim<<" 2\n";
	for( int i=0; i<Output.size(); i++ ){
		ostr.fill('0');
		ostr.width(dim);
		ostr<< mpz_class( i );
		ostr<<" ";
		ostr.fill('0');
		ostr.width(dim);
		ostr<< mpz_class( Output[i] );
		ostr<<std::endl;
	}
}

int main( int argc, char * argv[]){
	size_t dim = 2;
	size_t size = 1<<2;
	size_t comb = 1<<size;
	std::vector<int> domain(size);
	for( int i=0; i<size; i++ )
		domain[i] = i;
	char * fprefix = NULL;
	if( argc>1 )
		fprefix = argv[1];
	int count = 0;
	std::reverse( domain.begin(), domain.end() );
	do {
		count++;
		for( int i=0; i<comb; i++ ){
			if( i == 0 ) continue;	// No DC, the same as 1 DC
			if( i == comb-1 ) continue; // all DC
			std::vector<bool> CareSet( size );
			for( int j=0; j<size; j++ ){
				CareSet[j] = (i&(1<<j))? false: true;
			}
			std::vector<int> Output;
			Output = domain;
			ForceMap( CareSet, Output );
			//std::cout<< count<< "_"<< i<<std::endl;

			//std::cout << (fname +".cmp").c_str()<<std::endl;
			if( fprefix ){
				std::string fname;
				fname = fprefix;
				fname+= "/";
				fname+= mpz_class(count).get_str(10) + "_";
				fname+= mpz_class(i).get_str(10);

				std::fstream ostr1( (fname+".cmp").c_str()
					, std::ios::out );
				ostr1 << dim<<" 2"<<std::endl;
				for( int k=0; k<Output.size(); k++ ){
					ostr1.fill('0');
					ostr1.width(dim);
					ostr1 << mpz_class(k).get_str(2) <<" ";
					ostr1.fill('0');
					ostr1.width(dim);
					ostr1 << mpz_class(Output[k]).get_str(2)<<std::endl;
				}
				ostr1.close();

				std::fstream ostr2( (fname+".incmp").c_str()
					, std::ios::out );
				ostr2 << dim<<" 2"<<std::endl;
				for( int k=0; k<Output.size(); k++ ){
					if( !CareSet[k] )
						continue;
					ostr2.fill('0');
					ostr2.width(dim);
					ostr2 << mpz_class(k).get_str(2) <<" ";
					ostr2.fill('0');
					ostr2.width(dim);
					ostr2 << mpz_class(Output[k]).get_str(2) <<std::endl;
				}
				ostr2.close();
			}
		}
	} while( std::prev_permutation( domain.begin(), domain.end() ) );
	std::cout << "count="<<count<<std::endl;
}


