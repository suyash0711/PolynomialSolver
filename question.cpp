#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <cctype>
#include <stdexcept>
using namespace std;

// Convert string with given base -> long long (works up to base 16 safely, extendable if needed)
long long convertToDecimal(const string &val, int base) {
    long long result = 0;
    for (char c : val) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = tolower(c) - 'a' + 10;
        else throw invalid_argument("Invalid digit in base representation");

        if (digit >= base) throw invalid_argument("Digit out of range for base");
        result = result * base + digit;
    }
    return result;
}

// Multiply polynomial by (x - root)
vector<long long> multiplyPoly(const vector<long long> &poly, long long root) {
    int n = poly.size();
    vector<long long> newPoly(n + 1, 0);

    for (int i = 0; i < n; i++) {
        newPoly[i] += poly[i];
        newPoly[i + 1] -= root * poly[i];
    }
    return newPoly;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read JSON from stdin
    string input, line;
    while (getline(cin, line)) input += line;

    // Very simple parsing (not a full JSON parser, assumes valid input format)
    int n = 0, k = 0;
    vector<long long> roots;
    regex keyPattern("\"(\\d+)\"\\s*:\\s*\\{[^}]*\"base\"\\s*:\\s*\"(\\d+)\"[^}]*\"value\"\\s*:\\s*\"([^\"]+)\"");
    regex nkPattern("\"keys\"\\s*:\\s*\\{[^}]*\"n\"\\s*:\\s*(\\d+)[^}]*\"k\"\\s*:\\s*(\\d+)");

    smatch match;
    if (regex_search(input, match, nkPattern)) {
        n = stoi(match[1]);
        k = stoi(match[2]);
    }

    auto begin = sregex_iterator(input.begin(), input.end(), keyPattern);
    auto end = sregex_iterator();

    map<int, long long> indexedRoots;
    for (auto it = begin; it != end; ++it) {
        int idx = stoi((*it)[1]);
        int base = stoi((*it)[2]);
        string val = (*it)[3];
        long long root = convertToDecimal(val, base);
        indexedRoots[idx] = root;
    }

    // Take first k roots in ascending order of index
    int count = 0;
    for (auto &p : indexedRoots) {
        if (count == k) break;
        roots.push_back(p.second);
        count++;
    }

    // Build polynomial
    vector<long long> poly = {1}; // Start with P(x) = 1
    for (long long r : roots) {
        poly = multiplyPoly(poly, r);
    }

    // Output
    cout << "Polynomial coefficients (highest degree first):\n";
    for (size_t i = 0; i < poly.size(); i++) {
        cout << poly[i] << (i + 1 == poly.size() ? "\n" : " ");
    }

    return 0;
}
