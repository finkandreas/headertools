#include <sys/time.h>
#include <string>
#include <iostream>

class time_measure {
// See main() function for usage example
public:
  time_measure(const std::string& prefix_text = "", bool bSilent = false)
    : m_prefix_text(prefix_text)
    , m_bSilent(bSilent)
  {
    restart();
  }

  virtual ~time_measure()
  {
    if (!m_bSilent) print_elapsed();
  }

  void restart() {
    gettimeofday(&m_start, 0);
  }

  void print_elapsed() {
    if (!m_prefix_text.empty()) std::cout << m_prefix_text << " ";
    else std::cout << "Elapsed time: ";
    std::cout << elapsed_usec()/1000.0 << "ms" << std::endl;
  }


  size_t elapsed_usec() {
    struct timeval end;
    gettimeofday(&end, 0);
    timersub(&end, &m_start, &end);
    return end.tv_sec*1000000 + end.tv_usec;
  }

private:
  struct timeval m_start;
  std::string m_prefix_text;
  bool m_bSilent;
};

/*
#include <unistd.h>
#include <vector>
int main()
{
  time_measure t("Sleeping");
  sleep(1);
  t.print_elapsed();
  time_measure t1("Vector calculations");
  std::vector<int> vec(10000);
  for (auto& i : vec) {
    i = (&i - vec.data());
  }
  unsigned long long sum=0;
  for (auto i : vec) sum += i;
  std::cout << "sum=" << sum << std::endl;
  return 0;
}
*/
