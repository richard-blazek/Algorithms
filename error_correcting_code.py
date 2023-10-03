def _euclid(a, b):
    if not b:
        return a, (a // a if a else a), b

    aa, ab = b // b, b - b
    ba, bb = b - b, b // b
    while b:
        ratio = a // b
        aa, ab, ba, bb = ba, bb, aa - ba * ratio, ab - bb * ratio
        a, b = b, a - b * ratio
    return a, aa, ab

class ResidualClass:
    def __init__(self, remainder, divisor):
        self._remainder = remainder % divisor
        self._divisor = divisor

    def __add__(self, other):
        other_rem = other._remainder if isinstance(other, ResidualClass) else other
        return ResidualClass(other_rem + self._remainder, self._divisor)

    def __mul__(self, other):
        other_rem = other._remainder if isinstance(other, ResidualClass) else other
        return ResidualClass(other_rem * self._remainder, self._divisor)

    def __neg__(self):
        return ResidualClass(-self._remainder, self._divisor)

    def __sub__(self, other):
        return self + (-other)

    def inverse(self):
        _, inv_number, _ = _euclid(self._remainder, self._divisor)
        return ResidualClass(inv_number, self._divisor)

    def __truediv__(self, other):
        return self * other.inverse()

    def __floordiv__(self, other):
        return self / other

    def __mod__(self, other):
        return self - self

    def __repr__(self):
        return f'ResidualClass({repr(self._remainder)}, {repr(self._divisor)})'
    
    def __str__(self):
        return str(self._remainder)

    def __bool__(self):
        return bool(self._remainder)

    def __eq__(self, other):
        return isinstance(other, ResidualClass) and self._remainder == other._remainder and self._divisor == other._divisor
    
    def __ne__(self, other):
        return not (self == other)
    
    def __abs__(self):
        return self


class Polynomial:
    def __init__(self, coefficients: list = []):
        max_nonzero = max((i for i, c in enumerate(coefficients) if c), default=-1)
        self._coefficients = coefficients[:max_nonzero+1]

    def degree(self):
        return len(self._coefficients) - 1

    def __add__(self, other):
        result = [a + b for a, b in zip(self._coefficients, other._coefficients)]
        result += self._coefficients[len(result):]
        result += other._coefficients[len(result):]
        return Polynomial(result)

    def __neg__(self):
        return Polynomial([-c for c in self._coefficients])

    def __sub__(self, other):
        return self + (-other)

    def __mul__(self, other):
        if not self._coefficients or not other._coefficients:
            return Polynomial()
        result = [None] * (len(self._coefficients) + len(other._coefficients) - 1)
        for i in range(len(self._coefficients)):
            for j in range(len(other._coefficients)):
                product = self._coefficients[i] * other._coefficients[j]
                result[i + j] = product if result[i + j] is None else result[i + j] + product
        return Polynomial(result)

    def __repr__(self):
        return f'Polynomial({repr(self._coefficients)})'

    def __str__(self):
        if not self._coefficients:
            return '0'
        result = ''
        for i, c in enumerate(self._coefficients):
            if c:
                plus = '+' if c == abs(c) else ''
                power = '' if i == 0 else 'x' if i == 1 else f'x^{i}'
                coef = str(c) if c != c // c or i == 0 else ''
                result = f'{plus}{coef}{power}' + result
        return f'({result})'

    def __bool__(self):
        return len(self._coefficients) > 0

    def __abs__(self):
        return Polynomial(list(map(abs, self._coefficients)))

    def __divmod__(self, other):
        result = [None] * max(len(self._coefficients) - len(other._coefficients) + 1, 0)
        a = self._coefficients.copy()
        b = other._coefficients
        for i in range(len(result) - 1, -1, -1):
            result[i] = a[len(b) - 1 + i] / b[-1]
            for j in range(len(b)):
                a[i + j] -= result[i] * b[j]

        return (Polynomial(result), Polynomial(a))

    def __floordiv__(self, other):
        return divmod(self, other)[0]

    def __mod__(self, other):
        return divmod(self, other)[1]

    def evaluate(self, point):
        if not self._coefficients:
            return point - point
        result = self._coefficients[0]
        x = point
        for c in self._coefficients[1:]:
            result += c * x
            x *= point
        return result

def _enumerate_polynomials(degree, zero, one):
    current = Polynomial([zero] * degree + [one])
    x = Polynomial([zero, one])
    while current.degree() == degree:
        yield current
        current += Polynomial([one])
        divisor = x
        while not (current % divisor):
            current += divisor
            divisor *= x

def _is_divisible_by_any(polynomial: Polynomial, divisors):
    for divisor in divisors:
        if not (polynomial % divisor):
            return True
    return False

def _get_irreducible_polynomial(degree, zero, one):
    divisors = list(_enumerate_polynomials(1, zero, one)) + list(_enumerate_polynomials(2, zero, one))
    for p in _enumerate_polynomials(degree, zero, one):
        if not _is_divisible_by_any(p, divisors):
            return p

def get_galois_field_divisor(degree: int, prime: int):
    return _get_irreducible_polynomial(degree, ResidualClass(0, prime), ResidualClass(1, prime))
