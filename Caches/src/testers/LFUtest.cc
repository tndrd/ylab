#include "tester.hpp"
#include "LFU.hpp"

int main (int argc, char* argv[])
{
  return tester<caches::LFU<int>>(argc, argv);
}
