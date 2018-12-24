#include <iostream>
#include <cassert>
#include <vector>
#include <utility>

using namespace std;

string sub_1(string s) {
  const auto n = s.size();
  assert(n > 1 || n == 1 && s[0] > '0');
  for (auto i = 0; i < n; ++i) {
    auto j = n-i-1;
    if (s[j] == '0')
      s[j] = '9';
    else {
      --s[j];
      break;
    }
  }
  auto i = 0; 
  for (; s[i] == '0'; ++i);
  i = min(i, n - 1);
  return s.substr(i);
}

string add_1(string s) {
  const auto n = s.size();
  for (auto i = 0; i < n; ++i) {
    auto j = n - i - 1;
    if (s[j] != '9') {
      ++s[j];
      return s;
    }
    s[j] = '0';
  }
  return "1" + s;
}

string sub(string f, string s) {
  const auto n1 = f.size();
  const auto n2 = s.size();
  assert(n1 >= n2);
  char d = 0;
  int i = 0;
  for (; i < n2; ++i) {
    auto j1 = n1 - i - 1;
    auto j2 = n2 - i - 1;
    auto v1 = f[j1] - '0';
    auto v2 = s[j2] - '0';
    if (v2 + d > v1) {
      d = 1;
      v1 += 10;
    } else {
      d = 0;
    }
    f[j1] = v1 - v2 - d + '0';
  }
  for (; d != 0 && i < n1; ++i) {
    j = n1 - i - 1;
    if (f[j] != 0) {
      d = 0;
      --f[j];
    } else
      f[j] = '9';
  }
  i = 0;
  for (; i < n1 && f[i] == '0'; ++i);
  i = min(i, n1 - 1);
  return f.substr(i);
}

int cmp(const string &f, const string &s) {
  const auto n1 = f.size();
  const auto n2 = s.size();
  if (n1 > n2)
    return 1;
  if (n1 < n2)
    return -1;
  for (auto i = 0; i < n1; ++i) {
    if (f[i] > s[i])
      return 1;
    if (f[i] < s[i])
      return -1;
  }
  return 0;
}

vector<bool> test_border(const string &s, const vector<int> &num_zeros, char c) {
  const auto k = s.size();
  const auto n = num_zeros.size();
  vector<bool> res(n, false);
  auto i = 1;
  for (; i < n; ++i) {
    for (auto j = num_zeros[i-1]; j < num_zeros[i] && j < k; ++j)
      if (s[n-j-1] != c)
        break;
    if (has_left)
      break;
  }
  for (; i < n; ++i)
    res[i] = true;
  return res;
}

vector<pair<int, string>> solve(
    const string &l,
    const string &r,
    int k, bool has_left, bool has_right,
    const vector<int> &num_zeros,
    const vector<bool> &has_left_array,
    const vector<bool> &has_right_array) {
  
  if (k == 0)
    return vector<pair<int, string>>(
        make_pair(0, string(1, r.back() - l.back() + 1 + '0'));
  string k_left, k_right;
  const size_t l_size = l.size();
  const size_t r_size = r.size();
  size_t start = num_zeros[k];
  size_t stop = num_zeros[k+1];
  if (start > l_size)
    k_left = has_left?"1":"0";
  else {
    auto j1 = l_size - start;
    auto j2 = l_size - min(stop, l_size);
    k_left = l.substr(j2, j1 - j2);
    if (has_left)
      k_left = add_1(k_left);
  }
  if (start > r_size)
    k_right = has_right?"0":"1";
  else {
    auto j1 = r_size - start;
    auto j2 = r_size - min(stop, r_size);
    k_right = r.substr(j2, j1 - j2);
    if (!has_right)
      k_right = add_1(k_right);
  }
  auto cmp_res = cmp(k_left, k_right);

  if (cmp_res == 1) {
    // the left border is higher than the right one
    // so nothing to do on this level
    return solve(l, r, k - 1,
        has_left && has_left_array[k-1],
        has_right && has_right_array[k-1],
        num_zeros, has_left_array, has_right_array);
  }

  vector<pair<int, string>> res;
  if (has_left) {
    // create right border
    auto right = k_left + string(start, '9');
    auto left_res = solve(l, right, k-1,
        has_left && has_left_array[k-1], false,
        num_zeros, has_left_array, has_right_array);
    res.insert(res.end(), left_res.begin(), left_res.end());
  }
  if (cmp_res == -1)
    res.emplace_back(k, k_right - k_left);
  if (has_right) {
    auto left = k_right + string(start, '0');
    auto right_res = solve(left, r, k-1,
        false, has_right && has_right_array[k-1],
        num_zeros, has_left_array, has_right_array);
    res.insert(res.end(), right_res.begin(), right_res.end());
  }
  return res;
}

int main(int argc, char *argv[]) {
  string l, r;
  cin >> l >> r;
  l = sub_1(l);
  r = sub_1(r);
  const auto n = r.size();
  // number of zeros in the step size
  vector<int> num_zeros(1, 0);
  num_zeros.push_back(1);
  while (num_zeros.back() < n)
    num_zeros.push_back(num_zeros.back() * 2);
  auto has_left = test_border(l, num_zeros, '0');
  auto has_right = test_border(r, num_zeros, '9');
  auto k = num_zeros.size() - 2;
  auto res = solve(l, r, k, has_left[k], has_right[k], num_zeros, has_left, has_right);
  cout << res.size() << endl;
  for (auto &el: res)
    cout << el.first << ' ' << el.second << endl;
  return 0;
}
