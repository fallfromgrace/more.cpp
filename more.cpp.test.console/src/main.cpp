#include <tchar.h>
#include <cstdio>
#include "concurrency\concurrent_queue.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	con::concurrent_queue<int> q;

	q.push(0);

	int v;
	q.pop(v);

	return 0;
}