#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>

int main(){
	size_t size = 8;
	size_t comb = 256;
	std::vector<int> domain(size);
	for( int i=0; i<size; i++ )
		domain[i] = i;

	int count = 0;
	std::reverse( domain.begin(), domain.end() );
	do {
		count++;
		for( int i=0; i<comb; i++ ){
			if( i == 0 ) continue;	// No DC, the same as 1 DC
			if( i == 255 ) continue; // all DC
		}
	} while( std::prev_permutation( domain.begin(), domain.end() ) );
	std::cout << "count="<<count<<std::endl;
}

