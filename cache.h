#pragma once
#ifndef CACHE_H
#define CACHE_H
#include <cmath>
#include <iostream>
#include <time.h>
#include <queue>
using namespace std;


unsigned int m_w = 0xABABAB55;    /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05080902;    /* must not be zero, nor 0x9068ffff */

unsigned int rand_()
{
	m_z = 36969 * (m_z & 65535) + (m_z >> 16);
	m_w = 18000 * (m_w & 65535) + (m_w >> 16);
	return (m_z << 16) + m_w;  /* 32-bit result */
}

struct inst {
	bool v = false; //true ya3ni yes
			//false ya3ni no
	int tag = -1;
	int lru = 0;
	int lfu = 0;
};

class Cache {
private:
	int block_size;
	int cache_size;
	int block_no;
	int index_bits;
	int offset_bits;
	int tag_bits;
	int ways;
	queue <inst> fifo;
	inst **c;
	int f;
	int cnt = 0;
	int replaceFifo(inst w) {
		for (int i = 0; i < block_no; i++)
			if (c[i][0].tag == w.tag)
				return i;
		return -1;
	}

	int replaceLFU() {
		int min = 0;
		for (int i = 0; i < block_no; i++)
			if (c[i][0].lfu < c[min][0].lfu) 
				min = i;
		return min;
	}

	int replaceLRU() {
		int min = 0;
		for (int i = 0; i < block_no; i++)
			if (c[i][0].lru < c[min][0].lru)
				min = i;
		return min;
	}


public:
	Cache(int s, int bs, int w,int fu) {
		cache_size = s;
		block_size = bs;
		ways = w;
		f = fu;

		block_no = cache_size / (block_size * ways);
	
		cout <<"set # " <<block_no << endl;
		index_bits = log2(block_no);
		//cout << "index bits= " << index_bits << endl;
			
		offset_bits = log2(block_size);
		//cout << "index bits " << index_bits << endl;
		

		c = new inst* [block_no];
		for (int i = 0; i < block_no; i++)
			c[i] = new inst[ways];
	}

	bool insertAddr(int address) {
		if (ways == 1 && f==0)
			return insertAddr_direct(address);
		else if (ways == 2 || ways == 4 || ways == 8 || ways == 16)
			return insertAddr_Set(address);
		else
			return insertAddr_fully(address);
	}


	bool insertAddr_direct(int address) {
		int index = (address / (int)pow(2, offset_bits)) % (int)pow(2, index_bits);
		int tag = address / (int)pow(2, index_bits + offset_bits);
		if (c[index][0].v)
			if (c[index][0].tag == tag)
				return true;

		c[index][0].tag = tag;
		c[index][0].v = true;
		return false;
	}

	bool insertAddr_fully(int address) {
		//int index = (address / (int)pow(2, offset_bits)) % (int)pow(2, index_bits);
		int tag = address / (int)pow(2, offset_bits);
		//int w = rand() % block_no;

		int i = 0;

		for (i = 0; i < block_no; i++) {
			if (c[i][0].v) {
				if (c[i][0].tag == tag) 
					return true;
				
			}
			else
			{
				c[i][0].tag = tag;
				c[i][0].v = true;
				fifo.push(c[i][0]);
				return false;

			}
		}

		if (i == block_no) {
			int k = replaceFifo(fifo.front());
			while (k == -1) {
				fifo.pop();
				k = replaceFifo(fifo.front());
			}
			c[k][0].v = true;
			c[k][0].tag = tag;
			fifo.pop();
			fifo.push(c[k][0]);
			return false;
		}

}

	bool insertAddr_fullyLFU(int address) {
		
		int tag = address / (int)pow(2, offset_bits);
		
		int i = 0;

		for (i = 0; i < block_no; i++) {
			if (c[i][0].v) {
				if (c[i][0].tag == tag) {
					c[i][0].lfu++;
					return true;
				}

			}
			else
			{
				c[i][0].tag = tag;
				c[i][0].v = true;
				c[i][0].lfu++;
				return false;

			}
		}

		if (i == block_no) {
			int k = replaceLFU();
			while (k == -1) {
				k = replaceLFU();
			}
			c[k][0].v = true;
			c[k][0].tag = tag;
			c[k][0].lfu++;
			return false;
		}
}
	
bool insertAddr_Set(int address) {

	unsigned int index = (address / (int)pow(2, offset_bits)) % (int)pow(2, index_bits);
	int tag = address / (int)pow(2, index_bits + offset_bits);
	//cout << "index " << index << endl;
	
		for (int i = 0; i < ways; i++) {
			if (c[index][i].v) {
				if (c[index][i].tag == tag)
					return true;
				
				}
			
			else {
				c[index][i].tag = tag;
				c[index][i].v = true;
				return false;
			}
		}

			//if (i == ways - 1) {
				int r = rand_() % ways;
				//cout << "random " << r << endl;
				c[index][r].tag = tag;
				c[index][r].v = true;
				return false;
			//}
	}

bool insertAddr_fullyLRU(int address) {

	int tag = address / (int)pow(2, offset_bits);

	int i = 0;

	for (i = 0; i < block_no; i++) {
		if (c[i][0].v) {
			if (c[i][0].tag == tag) {
				cnt++;
				c[i][0].lru=cnt;
				return true;
			}

		}
		else
		{
			c[i][0].tag = tag;
			c[i][0].v = true;
			cnt++;
			c[i][0].lru = cnt;
			return false;

		}
	}

	if (i == block_no) {
		int k = replaceLRU();
		while (k == -1) {
			k = replaceLRU();
		}
		c[k][0].v = true;
		c[k][0].tag = tag;
		cnt++;
		c[k][0].lru = cnt;
		return false;
	}
}

bool insertAddr_fullyRAND(int address) {

	
	int tag = address / (int)pow(2, offset_bits);
	int i;
	for (i = 0; i < block_no; i++) {
		if (c[i][0].v) {
			if (c[i][0].tag == tag)
				return true;
		}
		else {
			c[i][0].v = true;
			c[i][0].tag = tag;
			return false;
		}
	}
		
			int k = rand_() % block_no;
			c[k][0].v = true;
			c[k][0].tag = tag;
			return false;

		
	

}

};
#endif // !CACHE_H
