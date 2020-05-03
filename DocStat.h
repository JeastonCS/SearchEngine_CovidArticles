// Yash Sinha
//

#include <string>
using namespace std;
class DocStat {
public:
    string docID;
    double tfidf;

    DocStat();
    DocStat(string s);
    DocStat(string s, double stat);
    DocStat(const DocStat&);
    bool operator > (const DocStat& rhs);
    bool operator < (const DocStat& rhs);
    bool operator == (const DocStat& rhs);
};

