#define DO_NOT_SUBMIT_FOR_HOMEWORK_SIX
#ifdef DO_NOT_SUBMIT_FOR_HOMEWORK_SIX

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Domain {
public:
  Domain(string lbl) : m_label(lbl) {}
  string label() const { return m_label; }
  const vector<Domain *> &subdomains() const { return m_subdomains; }
  void add(Domain *d) { m_subdomains.push_back(d); }
  ~Domain();

private:
  string m_label;
  vector<Domain *> m_subdomains;
};

Domain::~Domain() {
  for (size_t k = 0; k < m_subdomains.size(); k++)
    delete m_subdomains[k];
}

#endif // DO_NOT_SUBMIT_FOR_HOMEWORK_SIX

// NOLINTNEXTLINE(misc-no-recursion)
void listAll(const Domain *d, const std::string &path) // two-parameter overload
{
  if (d->subdomains().empty()) {
    std::cout << path << "\n";
    return;
  }

  for (Domain *subdomain : d->subdomains()) {
    std::string updated_path{subdomain->label() + (path.empty() ? "" : ".") +
                             path};
    listAll(subdomain, updated_path);
  }
}

#ifdef DO_NOT_SUBMIT_FOR_HOMEWORK_SIX

void listAll(const Domain *d) // one-parameter overload
{
  if (d != nullptr)
    listAll(d, d->label());
}

int main() {
  Domain *d1 = new Domain("ucla");
  d1->add(new Domain("cs"));
  d1->add(new Domain("ee"));
  d1->add(new Domain("math"));
  Domain *d2 = new Domain("caltech");
  d2->add(new Domain("math"));
  d2->add(new Domain("cs"));
  Domain *d3 = new Domain("edu");
  d3->add(d1);
  d3->add(d2);
  Domain *d4 = new Domain("com");
  d4->add(new Domain("microsoft"));
  d4->add(new Domain("apple"));
  Domain *d5 = new Domain("biz");
  Domain *root = new Domain("");
  root->add(d3);
  root->add(d4);
  root->add(d5);
  listAll(root);
  cout << "====" << endl;
  listAll(d2);
  cout << "====" << endl;
  listAll(d5);
  delete root;
}

#endif // DO_NOT_SUBMIT_FOR_HOMEWORK_SIX
