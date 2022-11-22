#include "tester.hpp"
#include "LRU.hpp"

int main (int argc, char* argv[])
{
  return tester<caches::LRU<int>>(argc, argv);
}
