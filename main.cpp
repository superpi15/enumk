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

int main(){
	size_t size = 4;
	size_t comb = 1<<size;
	std::vector<int> domain(size);
	for( int i=0; i<size; i++ )
		domain[i] = i;

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
		}
	} while( std::prev_permutation( domain.begin(), domain.end() ) );
	std::cout << "count="<<count<<std::endl;
}


