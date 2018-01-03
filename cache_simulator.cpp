#include <iostream>
#include  <iomanip>
#include "cache.h"
using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(64*1024*1024)
#define		CACHE_SIZE		(64*1024)
#define testsize 1000000

enum cacheResType { MISS = 0, HIT = 1 };

//unsigned int m_w = 0xABABAB55;    /* must not be zero, nor 0x464fffff */
//unsigned int m_z = 0x05080902;    /* must not be zero, nor 0x9068ffff */
//
//unsigned int rand_()
//{
//	m_z = 36969 * (m_z & 65535) + (m_z >> 16);
//	m_w = 18000 * (m_w & 65535) + (m_w >> 16);
//	return (m_z << 16) + m_w;  /* 32-bit result */
//}

unsigned int memGen1()
{
	static unsigned int addr = 0;
	return (addr++) % (DRAM_SIZE);
}

unsigned int memGen2()
{
	static unsigned int addr = 0;
	return  rand_() % (128 * 1024);
}

unsigned int memGen3()
{
	return rand_() % (DRAM_SIZE);
}

unsigned int memGen4()
{
	static unsigned int addr = 0;
	return (addr++) % (1024);
}

unsigned int memGen5()
{
	static unsigned int addr = 0;
	return (addr++) % (1024 * 64);
}

unsigned int memGen6()
{
	static unsigned int addr = 0;
	return (addr += 256) % (DRAM_SIZE);
}


// Cache Simulator


void simulator1(int cachesize, int blocksize, int ways) {

	Cache direct_mapped(cachesize, blocksize, ways,0);
	unsigned int addr;
	int miss = 0, hit = 0;
	for (int i = 0; i < testsize; i++) {
		addr=memGen1();
		if (direct_mapped.insertAddr_Set(addr)) {
			hit++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Hit)\n";
		}
		else {
			miss++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Miss)\n";
		}
	}
	cout << "Hits = " << dec <<hit << endl;
	cout << "Misses = " << dec <<miss << endl;
	cout << "Miss rate = " << (miss / (float)testsize) * 100 <<"%"<< endl;
	cout << "Hit rate = " << (hit / (float)testsize) * 100 << "%" <<endl;
}

void simulator2(int cachesize, int blocksize, int ways) {

	Cache direct_mapped(cachesize, blocksize, ways, 0);
	Cache set4(cachesize, blocksize, ways*2, 0);
	Cache set8(cachesize, blocksize, ways*4, 0);
	Cache set16(cachesize, blocksize, ways * 8, 0);

	unsigned int addr;
	int arr[8] = { 0,0,0,0,0,0,0,0 };
	int r;
	for (int i = 0; i < testsize; i++) {
		r = rand_() % 6 + 1;
	//	cout << "rand= " << r << endl;
		switch (r) {
		case 1:addr = memGen1(); break;
		case 2:addr = memGen2(); break;
		case 3:addr = memGen3(); break;
		case 4:addr = memGen4(); break;
		case 5:addr = memGen5(); break;
		case 6:addr = memGen6(); break;
		default: break;
		}
		
		if (direct_mapped.insertAddr_Set(addr)) {
			//hit++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Hit)\n";
			arr[0]++;
		}
		else {
			//miss++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Miss)\n";
			arr[1]++;
		}

		if (set4.insertAddr_Set(addr)) 
			arr[2]++;
		else 
			arr[3]++;

		if (set8.insertAddr_Set(addr))
			arr[4]++;
		else
			arr[5]++;

		if (set16.insertAddr_Set(addr))
			arr[6]++;
		else
			arr[7]++;
		
	}
	//cout << "Hits = " << dec << hit << endl;
	//cout << "Misses = " << dec << miss << endl;
	//cout << "Miss rate = " << (miss / (float)testsize) * 100 << "%" << endl;
	cout << "2-way Hit rate = " << (arr[0] / (float)testsize) * 100 << "%" << endl;
	cout << "4-way Hit rate = " << (arr[2] / (float)testsize) * 100 << "%" << endl;
	cout << "8-way Hit rate = " << (arr[4] / (float)testsize) * 100 << "%" << endl;
	cout << "16-way Hit rate = " << (arr[6] / (float)testsize) * 100 << "%" << endl;
}

void simulatordirect(int cachesize, int blocksize, int ways) {

	Cache c1(cachesize, blocksize, ways, 0);
	Cache c2(cachesize, blocksize, ways, 0);
	Cache c3(cachesize, blocksize, ways, 0);
	Cache c4(cachesize, blocksize, ways, 0);
	Cache c5(cachesize, blocksize, ways, 0);
	Cache c6(cachesize, blocksize, ways, 0);
	unsigned int addr[6];
	int hits[6] = { 0,0,0,0,0,0 };
	int misses[6] = { 0, 0, 0, 0, 0, 0 };
	int miss = 0, hit = 0;
	for (int i = 0; i < testsize; i++) {
		addr[0] = memGen1();
		addr[1] = memGen2();
		addr[2] = memGen3();
		addr[3] = memGen4();
		addr[4] = memGen5();
		addr[5] = memGen6();
		if (c1.insertAddr_direct(addr[0])) {
			hits[0]++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Hit)\n";
		}
		else {
			misses[0]++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Miss)\n";
		}

		if (c2.insertAddr_direct(addr[1]))
			hits[1]++;

		else
			misses[1]++;

		if (c3.insertAddr_direct(addr[2]))
			hits[2]++;

		else
			misses[2]++;

		if (c4.insertAddr_direct(addr[3]))
			hits[3]++;
		else
			misses[3]++;

		if (c5.insertAddr_direct(addr[4]))
			hits[4]++;
		else
			misses[4]++;

		if (c6.insertAddr_direct(addr[5]))
			hits[5]++;
		else
			misses[5]++;

	}
	//cout << "Hits = " << dec << hit << endl;
	//cout << "Misses = " << dec << miss << endl;
	//	cout << "Miss rate = " << (miss / (float)testsize) * 100 << "%" << endl;
	for (int i = 0; i<6; i++)
		cout << "Memgen " << i + 1 << " Hit rate = " << (hits[i] / (float)testsize) * 100 << "%" << endl;
}

void simulatorSet(int cachesize, int blocksize, int ways) {

	Cache c1(cachesize, blocksize, ways, 0);
	Cache c2(cachesize, blocksize, ways, 0);
	Cache c3(cachesize, blocksize, ways, 0);
	Cache c4(cachesize, blocksize, ways, 0);
	Cache c5(cachesize, blocksize, ways, 0);
	Cache c6(cachesize, blocksize, ways, 0);
	unsigned int addr[6];
	int hits[6] = { 0,0,0,0,0,0 };
	int misses[6] = { 0, 0, 0, 0, 0, 0 };
	int miss = 0, hit = 0;
	for (int i = 0; i < testsize; i++) {
		addr[0] = memGen1();
		addr[1] = memGen2();
		addr[2] = memGen3();
		addr[3] = memGen4();
		addr[4] = memGen5();
		addr[5] = memGen6();
		if (c1.insertAddr_Set(addr[0])) {
			hits[0]++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Hit)\n";
		}
		else {
			misses[0]++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Miss)\n";
		}

		if (c2.insertAddr_Set(addr[1]))
			hits[1]++;

		else
			misses[1]++;

		if (c3.insertAddr_Set(addr[2]))
			hits[2]++;

		else
			misses[2]++;

		if (c4.insertAddr_Set(addr[3]))
			hits[3]++;
		else
			misses[3]++;

		if (c5.insertAddr_Set(addr[4]))
			hits[4]++;
		else
			misses[4]++;

		if (c6.insertAddr_Set(addr[5]))
			hits[5]++;
		else
			misses[5]++;

	}
	//cout << "Hits = " << dec << hit << endl;
	//cout << "Misses = " << dec << miss << endl;
	//	cout << "Miss rate = " << (miss / (float)testsize) * 100 << "%" << endl;
	for (int i = 0; i<6; i++)
		cout << "Memgen " << i + 1 << " Hit rate = " << (hits[i] / (float)testsize) * 100 << "%" << endl;
}

void simulator5(int cachesize, int blocksize, int ways) {

	Cache direct_mapped(cachesize, blocksize, ways, 0);
	unsigned int addr;
	int miss = 0, hit = 0;
	for (int i = 0; i < testsize; i++) {
		addr = memGen5();
		if (direct_mapped.insertAddr_Set(addr)) {
			hit++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Hit)\n";
		}
		else {
			miss++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Miss)\n";
		}
	}
	cout << "Hits = " << dec << hit << endl;
	cout << "Misses = " << dec << miss << endl;
	cout << "Miss rate = " << (miss / (float)testsize) * 100 << "%" << endl;
	cout << "Hit rate = " << (hit / (float)testsize) * 100 << "%" << endl;
}

void simulator6(int cachesize, int blocksize, int ways) {

	Cache c1(cachesize, blocksize, ways, 1);
	Cache c2(cachesize, blocksize, ways, 1);
	Cache c3(cachesize, blocksize, ways, 1);
	Cache c4(cachesize, blocksize, ways, 1);
	Cache c5(cachesize, blocksize, ways, 1);
	Cache c6(cachesize, blocksize, ways, 1);
	unsigned int addr[6];
	int hits[6] = { 0,0,0,0,0,0 };
	int misses[6]={ 0, 0, 0, 0, 0, 0 };
	int miss = 0, hit = 0;
	for (int i = 0; i < testsize; i++) {
		addr[0] = memGen1();
		addr[1] = memGen2();
		addr[2] = memGen3();
		addr[3] = memGen4();
		addr[4] = memGen5();
		addr[5] = memGen6();
		if (c1.insertAddr_fullyRAND(addr[0])) {
			hits[0]++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Hit)\n";
		}
		else {
			misses[0]++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Miss)\n";
		}

		if (c2.insertAddr_fullyRAND(addr[1])) 
			hits[1]++;
		
		else 
			misses[1]++;

		if (c3.insertAddr_fullyRAND(addr[2]))
			hits[2]++;

		else
			misses[2]++;

		if (c4.insertAddr_fullyRAND(addr[3]))
			hits[3]++;
		else
			misses[3]++;

		if (c5.insertAddr_fullyRAND(addr[4]))
			hits[4]++;
		else
			misses[4]++;

		if (c6.insertAddr_fullyRAND(addr[5]))
			hits[5]++;
		else
			misses[5]++;
		
	}
	//cout << "Hits = " << dec << hit << endl;
	//cout << "Misses = " << dec << miss << endl;
//	cout << "Miss rate = " << (miss / (float)testsize) * 100 << "%" << endl;
	for(int i=0;i<6;i++)
	cout << "Memgen " << i+1  <<" Hit rate = " << (hits[i] / (float)testsize) * 100 << "%" << endl;
}

void simulatorFIFO(int cachesize, int blocksize, int ways) {

	Cache c1(cachesize, blocksize, ways, 1);
	Cache c2(cachesize, blocksize, ways, 1);
	Cache c3(cachesize, blocksize, ways, 1);
	Cache c4(cachesize, blocksize, ways, 1);
	Cache c5(cachesize, blocksize, ways, 1);
	Cache c6(cachesize, blocksize, ways, 1);
	unsigned int addr[6];
	int hits[6] = { 0,0,0,0,0,0 };
	int misses[6] = { 0, 0, 0, 0, 0, 0 };
	int miss = 0, hit = 0;
	for (int i = 0; i < testsize; i++) {
		addr[0] = memGen1();
		addr[1] = memGen2();
		addr[2] = memGen3();
		addr[3] = memGen4();
		addr[4] = memGen5();
		addr[5] = memGen6();
		if (c1.insertAddr_fully(addr[0])) {
			hits[0]++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Hit)\n";
		}
		else {
			misses[0]++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Miss)\n";
		}

		if (c2.insertAddr_fully(addr[1]))
			hits[1]++;

		else
			misses[1]++;

		if (c3.insertAddr_fully(addr[2]))
			hits[2]++;

		else
			misses[2]++;

		if (c4.insertAddr_fully(addr[3]))
			hits[3]++;
		else
			misses[3]++;

		if (c5.insertAddr_fully(addr[4]))
			hits[4]++;
		else
			misses[4]++;

		if (c6.insertAddr_fully(addr[5]))
			hits[5]++;
		else
			misses[5]++;

	}
	//cout << "Hits = " << dec << hit << endl;
	//cout << "Misses = " << dec << miss << endl;
	//	cout << "Miss rate = " << (miss / (float)testsize) * 100 << "%" << endl;
	for (int i = 0; i<6; i++)
		cout << "Memgen " << i + 1 << " Hit rate = " << (hits[i] / (float)testsize) * 100 << "%" << endl;
}

void simulatorLFU(int cachesize, int blocksize, int ways) {

	Cache c1(cachesize, blocksize, ways, 1);
	Cache c2(cachesize, blocksize, ways, 1);
	Cache c3(cachesize, blocksize, ways, 1);
	Cache c4(cachesize, blocksize, ways, 1);
	Cache c5(cachesize, blocksize, ways, 1);
	Cache c6(cachesize, blocksize, ways, 1);
	unsigned int addr[6];
	int hits[6] = { 0,0,0,0,0,0 };
	int misses[6] = { 0, 0, 0, 0, 0, 0 };
	int miss = 0, hit = 0;
	for (int i = 0; i < testsize; i++) {
		addr[0] = memGen1();
		addr[1] = memGen2();
		addr[2] = memGen3();
		addr[3] = memGen4();
		addr[4] = memGen5();
		addr[5] = memGen6();
		if (c1.insertAddr_fullyLFU(addr[0])) {
			hits[0]++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Hit)\n";
		}
		else {
			misses[0]++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Miss)\n";
		}

		if (c2.insertAddr_fullyLFU(addr[1]))
			hits[1]++;

		else
			misses[1]++;

		if (c3.insertAddr_fullyLFU(addr[2]))
			hits[2]++;

		else
			misses[2]++;

		if (c4.insertAddr_fullyLFU(addr[3]))
			hits[3]++;
		else
			misses[3]++;

		if (c5.insertAddr_fullyLFU(addr[4]))
			hits[4]++;
		else
			misses[4]++;

		if (c6.insertAddr_fullyLFU(addr[5]))
			hits[5]++;
		else
			misses[5]++;

	}
	//cout << "Hits = " << dec << hit << endl;
	//cout << "Misses = " << dec << miss << endl;
	//	cout << "Miss rate = " << (miss / (float)testsize) * 100 << "%" << endl;
	for (int i = 0; i<6; i++)
		cout << "Memgen " << i + 1 << " Hit rate = " << (hits[i] / (float)testsize) * 100 << "%" << endl;
}

void simulatorLRU(int cachesize, int blocksize, int ways) {

	Cache c1(cachesize, blocksize, ways, 1);
	Cache c2(cachesize, blocksize, ways, 1);
	Cache c3(cachesize, blocksize, ways, 1);
	Cache c4(cachesize, blocksize, ways, 1);
	Cache c5(cachesize, blocksize, ways, 1);
	Cache c6(cachesize, blocksize, ways, 1);
	unsigned int addr[6];
	int hits[6] = { 0,0,0,0,0,0 };
	int misses[6] = { 0, 0, 0, 0, 0, 0 };
	int miss = 0, hit = 0;
	for (int i = 0; i < testsize; i++) {
		addr[0] = memGen1();
		addr[1] = memGen2();
		addr[2] = memGen3();
		addr[3] = memGen4();
		addr[4] = memGen5();
		addr[5] = memGen6();
		if (c1.insertAddr_fullyLRU(addr[0])) {
			hits[0]++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Hit)\n";
		}
		else {
			misses[0]++;
			//cout << "0x" << setfill('0') << setw(8) << hex << addr << " (Miss)\n";
		}

		if (c2.insertAddr_fullyLRU(addr[1]))
			hits[1]++;

		else
			misses[1]++;

		if (c3.insertAddr_fullyLRU(addr[2]))
			hits[2]++;

		else
			misses[2]++;

		if (c4.insertAddr_fullyLRU(addr[3]))
			hits[3]++;
		else
			misses[3]++;

		if (c5.insertAddr_fullyLRU(addr[4]))
			hits[4]++;
		else
			misses[4]++;

		if (c6.insertAddr_fullyLRU(addr[5]))
			hits[5]++;
		else
			misses[5]++;

	}
	//cout << "Hits = " << dec << hit << endl;
	//cout << "Misses = " << dec << miss << endl;
	//	cout << "Miss rate = " << (miss / (float)testsize) * 100 << "%" << endl;
	for (int i = 0; i<6; i++)
		cout << "Memgen " << i + 1 << " Hit rate = " << (hits[i] / (float)testsize) * 100 << "%" << endl;
}

//void FULLSIMULATOR(int cachesize, int linesize, int ways) {
//	cout << "MEMGEN1" << endl;
//	simulator1(cachesize * 1024, linesize, ways);
//	cout << endl << endl;
//
//	cout << "MEMGEN2" << endl;
//	simulator2(cachesize * 1024, linesize, ways);
//	cout << endl << endl;
//
//	cout << "MEMGEN3" << endl;
//	simulator3(cachesize * 1024, linesize, ways);
//	cout << endl << endl;
//
//	cout << "MEMGEN4" << endl;
//	simulator4(cachesize * 1024, linesize, ways);
//	cout << endl << endl;
//
//	cout << "MEMGEN5" << endl;
//	simulator5(cachesize * 1024, linesize, ways);
//	cout << endl << endl;
//
//	cout << "MEMGEN6" << endl;
//	simulator6(cachesize * 1024, linesize, ways);
//	cout << endl << endl;
//}



char *msg[2] = { "Miss","Hit" };

int main()
{
	int inst = 0;
	cacheResType r;
	srand(unsigned(time(NULL)));

	cout << "Cache Simulator\n";
	int cs;
	int ls;
	int w;
	int t;

	cout << "Enter the cache type. (0 for direct, 1 for fully associative, 2 for set associative" << endl;
	cin >> t;

	cout << "Enter the cache size" << endl;
	cin >> cs;
	cout << "Enter the line size" << endl;
	cin >> ls;
	if (t == 2) {
		cout << "Enter the number of ways" << endl;
		cin >> w;
	}
	int m;
	if (t == 1) {
		cout << "Choose the replacement policy (0 for FIFO, 1 for LFU, 2 for LRU, 3 for Random)" << endl;
		cin >> m;
	}

	if (t == 0)
		simulator1(cs * 1024, ls, 1);
	else if (t == 1) {
		switch (m) {
		case 0:
			simulatorFIFO(cs * 1024, ls, 1);
			break;
		case 1:
			simulatorLFU(cs * 1024, ls, 1);
			break;
		case 2:
			simulatorLRU(cs * 1024, ls, 1);
			break;
		case 3:
			simulator6(cs * 1024, ls, 1);
			break;
		default:simulator6(cs * 1024, ls, 1); break;
		}
	}
	else if (t == 2)
		simulatorSet(cs * 1024, ls, w);
	else if (t == 1)
		simulatordirect(cs * 1024, ls, 1);
	//FULLSIMULATOR(64, 32, 16);
	//simulator6(16*1024, 32, 1);
	system("pause");
	return 0;
}