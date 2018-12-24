def solve(l, r, k, num_digits):
    if k == 0:
        return [(0, r - l + 1)]
    step = int('1' + '0' * (num_digits[k] - 1))
    k_left = 0 if l==0 else ((l - 1) // step + 1) * step
    k_right = (r + 1) // step * step - 1
    res = []
    if k_left > k_right:
        return solve(l, r, k - 1, num_digits)
    if k_left != l:
        res += solve(l, min(k_left - 1, r), k - 1, num_digits)
    if k_left < k_right:
        res += [(k, (k_right + 1 - k_left) // step)]
    if k_right != r:
        res += solve(max(l, k_right + 1), r, k - 1, num_digits)
    return res

l, r = map(lambda x: int(x) - 1, [input(), input()])
l_str, r_str = map(str, [l, r])

num_zeros = [0, 1]

while num_zeros[-1] < len(r_str) - 1:
    num_zeros.append(num_zeros[-1] * 2)

num_digits = list(map(lambda x: x+1, num_zeros))

res = solve(l, r, len(num_digits) - 1, num_digits)
print(len(res))
for el in res:
    print(el[0], el[1])

