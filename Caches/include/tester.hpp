#include "hit_count.hpp"

template<typename CacheT> int tester(int argc, char* argv[])
{
  assert(argv);
  
  int expected_hits = std::stoi(argv[1]);
  int result_hits   = count_hits_from_argv_params<CacheT>(argc-2, argv+2);

  if (result_hits != expected_hits)
  {
    std::cout << "Got " << result_hits << ", expected " << expected_hits << std::endl;
    return 1;
  }

  return 0;
}