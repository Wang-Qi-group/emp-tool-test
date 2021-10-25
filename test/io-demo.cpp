#include "emp-tool/emp-tool.h"
#include <iostream>
#include <cstdio>

using namespace std;
using namespace emp;

int port, party;

void print128_num(__m128i var)
{
    uint32_t val[4];
    memcpy(val, &var, sizeof(val));

    char buffer[50];
    sprintf(buffer, "%i%i%i%i", val[0], val[1], val[2], val[3]);
    std::cout << buffer << std::endl;
}

void print_block_array(block *b0, int length)
{
    cout << "block array:" << endl;
    for (block* p = b0; p != b0 + length; ++p)
    {
        print128_num(*p);
    }
}


template <typename T>
void test(T *io)
{
    if (party == ALICE)
    {
        cout << "Hello I'm Alice" << endl;
        // 定义一个 length 为 4 的 block 数组并把每个 block 按 32 bit 一组初始化为 0
        int length = 4;
        block *b0 = new block[length];
        for (block *p = b0; p != b0 + length; ++p)
        {
            *p = _mm_set1_epi32(0);
        }
        // 发送 block array
        cout << "Here is the block array I'm going to send" << endl;
        print_block_array(b0, length);
        io->send_block(b0, length);

        delete[] b0;
    }
    else{// party == BOB
        cout << "I'm Bob" << endl;
        // 创建一个 block array 来接收 io 发送的数据
        int length = 4;
        block *r = new block[length];
        // 接收并打印
        cout << "Here is the block array I received" << endl;
        io->recv_block(r, length);
        print_block_array(r, length);

        delete[] r;
    }
}


int main(int argc, char **argv)
{
    parse_party_and_port(argv, &party, &port);
	cout <<"NetIO\n";
    cout << party << endl;
    cout << port << endl;

	NetIO * io = new NetIO(party == ALICE ? nullptr:"127.0.0.1", port);
    test<NetIO>(io);

    delete io;
}